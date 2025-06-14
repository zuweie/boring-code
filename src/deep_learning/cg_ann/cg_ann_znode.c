#include "deep_learning/compute_graph2/cg_list.h"
#include "deep_learning/compute_graph2/cg_base.h"
#include "deep_learning/compute_graph2/cg_tensor.h"
#include "cg_ann.h"
#include "cg_ann_znode.h"

static ann_znode_t* __create_ann_znode(cg_ann_t* ann, ann_znode_type_t znode_type)
{
    ann_znode_t* znode = (ann_znode_t*) malloc (sizeof(ann_znode_t));
    // 需要将调用基类的构造函数进行初始化
    cg_znode_base_construct(znode);

    switch (znode_type)
    {
    case e_x:
        /* code */
        sprintf(znode->base.vertex.id, "x_%d", cg_list_size(ann->cg_base.znode_list));
        znode->gradient = cg_tensor_create(ann->cg_base.znode_alloc, )
        break;
    case e_y:
        sprintf(znode->base.vertex.id, "y_%d", cg_list_size(ann->cg_base.znode_list));
        break;
    case e_weight:
        sprintf(znode->base.vertex.id, "w_%d", cg_list_size(ann->cg_base.znode_list));
        break;
    case e_bais:
        sprintf(znode->base.vertex.id, "b_%d", cg_list_size(ann->cg_base.znode_list));
        break;
    case e_y_hat:
        sprintf(znode->base.vertex.id, "y-hat_%d", cg_list_size(ann->cg_base.znode_list));
        break;
    case e_loss:
        sprintf(znode->base.vertex.id, "J_%d", cg_list_size(ann->cg_base.znode_list));
        break;
    case e_auto:
    default:
        sprintf(znode->base.vertex.id, "z_%d", cg_list_size(ann->cg_base.znode_list));
        break;
    }
    znode->znode_type = znode_type;
    znode->gradient_version = 0;
    return znode;
}

ann_znode_t* cg_ann_znode_create(cg_ann_t* ann, int r, int c, ann_znode_type_t znode_type)
{
    ann_znode_t* znode = __create_ann_znode(ann, znode_type);
    switch (znode_type)
    {
        case e_x:
            znode->payload  = cg_tensor_create(&ann->alloc, 2, r, c);
            znode->gradient = NULL;
            break;
        case e_y:
            znode->payload = cg_tensor_create(&ann->alloc, 2, r, c);
            znode->gradient = NULL;
            break;
        case e_weight:
            znode->payload = cg_tensor_create(&ann->alloc, 2, r, c);
            znode->gradient = cg_tensor_create(&ann->alloc, 2, r, c);
            break;
        case e_bais:
            znode->payload = cg_tensor_create(&ann->alloc, 2, r, c);
            znode->gradient = cg_tensor_create(&ann->alloc, 2, r, c);
        default:
            znode->payload = NULL;
            znode->gradient = NULL;
            break;
    }
    return znode;
}

int cg_ann_znode_recycle(ann_znode_t* znode)
{
    cg_znode_base_deconstruct(znode);

    if (znode->payload) {
        cg_tensor_recycle(znode->payload);
        znode->payload = NULL;
    }
    if (znode->gradient) {
        cg_tensor_recycle(znode->gradient);
        znode->gradient = NULL;
    }
    free(znode);
    return 0;
}