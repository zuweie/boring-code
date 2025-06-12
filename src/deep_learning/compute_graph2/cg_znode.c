/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-06-12 13:33:03
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-06-12 13:50:50
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_znode.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "cg_tensor.h"
#include "cg_znode.h"

static int __recycle_path(cg_ref_t ref) 
{
    cg_list_t* list = (cg_list_t*) ref;
    cg_list_recycle(list, NULL);
}

cg_flow_elem_t* cg_znode_create(cg_t* cg, znode_type_t znode_type)
{
    
    cg_znode_t* znode = (cg_znode_t*) malloc (sizeof(cg_znode_t));
    cg_list_push(cg->znode_list, znode);

    // name it
    switch (znode_type)
    {
    case e_x:
        /* code */
        sprintf(znode->vertex.id, "x_%d", cg_list_size(cg->znode_list));
        break;
    case e_y:
        sprintf(znode->vertex.id, "y_%d", cg_list_size(cg->znode_list));
        break;
    case e_weight:
        sprintf(znode->vertex.id, "w_%d", cg_list_size(cg->znode_list));
        break;
    case e_baise:
        sprintf(znode->vertex.id, "b_%d", cg_list_size(cg->znode_list));
        break;
    case e_y_hat:
        sprintf(znode->vertex.id, "y-hat_%d", cg_list_size(cg->znode_list));
        break;
    case e_loss:
        sprintf(znode->vertex.id, "J_%d", cg_list_size(cg->znode_list));
        break;
    case e_middle:
    default:
        sprintf(znode->vertex.id, "z_%d", cg_list_size(cg->znode_list));
        break;
    }
    znode->compute_graph         = cg;
    znode->payload               = NULL;
    znode->gradient              = NULL;
    znode->flow_elem.elem_type   = e_znode;
    znode->opt                   = NULL;
    znode->gradient_paths        = NULL;
    znode->curr_gradient_version = 0;

    cg_graph_add_vertex(&cg->compute_graph, &znode->vertex);

    return znode;
}

int cg_znode_recycle(cg_znode_t* znode)
{
    if (znode->payload) 
        cg_tensor_recycle(znode->payload);

    if (znode->gradient)
        cg_tensor_recycle(znode->gradient);

    if (znode->opt){
        if (znode->opt->recycle)
            znode->opt->recycle(znode->opt);

        free(znode->opt);
    } 
    
    // free gradient paths
    cg_list_recycle(znode->gradient_paths, __recycle_path);

    free(znode);

    return 0;
}