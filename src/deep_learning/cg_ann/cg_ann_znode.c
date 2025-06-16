/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-06-13 13:31:06
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-06-16 16:40:21
 * @FilePath: /boring-code/src/deep_learning/cg_ann/cg_ann_znode.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>
#include "deep_learning/compute_graph2/cg_list.h"
#include "deep_learning/compute_graph2/cg_base.h"
#include "deep_learning/compute_graph2/cg_tensor.h"
#include "cg_ann.h"
#include "cg_ann_znode.h"

static ann_znode_t* __create_ann_znode(cg_ann_t* ann, ann_znode_type_t znode_type)
{
    ann_znode_t* znode = (ann_znode_t*) malloc (sizeof(ann_znode_t));
    znode->ann = ann;
    // 需要将调用基类的构造函数进行初始化
    char vertex_id[64];
    switch (znode_type)
    {
    case e_x:
        /* code */
        sprintf(vertex_id, "x_%d", cg_list_size(ann->znode_list));
        break;
    case e_y:
        sprintf(vertex_id, "y_%d", cg_list_size(ann->znode_list));
        break;
    case e_weight:
        sprintf(vertex_id, "w_%d", cg_list_size(ann->znode_list));
        break;
    case e_bais:
        sprintf(vertex_id, "b_%d", cg_list_size(ann->znode_list));
        break;
    case e_y_hat:
        sprintf(vertex_id, "y-hat_%d", cg_list_size(ann->znode_list));
        break;
    case e_loss:
        sprintf(vertex_id, "J_%d", cg_list_size(ann->znode_list));
        break;
    case e_auto:
    default:
        sprintf(vertex_id, "z_%d", cg_list_size(ann->znode_list));
        break;
    }
    znode->znode_type = znode_type;
    znode->znode_base.gradient_version = 0;
    cg_znode_base_construct(ann, znode, vertex_id);
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