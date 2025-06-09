/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-05-24 09:56:43
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-06-09 17:40:09
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE{}
 */
#include <stdlib.h>
#include <stdio.h>
#include "base/container_of.h"
#include "cg_opt_base.h"
#include "cg_graph.h"
#include "cg_list.h"
#include "cg_znode.h"
#include "cg.h"


static int __build_compute_flow(cg_t* cg) 
{
    return 0;
}

static int __recycle_znode(cg_ref_t ref) 
{
    cg_znode_t* znode = (cg_znode_t*) ref;
    cg_znode_recycle(znode);
    return 0;
}

int cg_init(cg_t* cg)
{
    cg->compute_graph  = cg_graph_init(&cg->compute_graph);
    cg->flow_stack     = cg_list_create();
    cg->data_version  = 0;
    cg->znode_count   = 1;
    return 0;
}

int cg_reset(cg_t* cg)
{
    cg_graph_reset(&cg->compute_graph);
    cg_list_recycle(cg->flow_stack);
    cg->data_version = 0;
    cg->znode_count  = 1;
    return 0;
}



int cg_flow_push(cg_t* cg, cg_flow_elem_t* flow_elem)
{
    if (flow_elem->typ == end) {
        
        return __build_compute_flow(cg);
        
    } else {
        
        // 将
        return cg_list_push(cg->flow_stack, flow_elem);
    }
}

cg_znode_t* cg_add_znode(cg_t* cg, const char* znode_name, int shape_axes, int* shape_dimens)
{
    cg->znode_count++;
    cg_znode_t* znode = (cg_znode_t*) malloc (sizeof(cg_znode_t));

    if (znode_name != NULL) {
        sprintf(znode->vertex.id, znode_name);
    } else {
        sprintf(znode->vertex.id, "znode_%d", cg->znode_count);
    }

    znode->flow_elem.typ = znode;
    if (shape_axes > 0 && shape_dimens != NULL) {
        znode->payload = cg_tensor_create(cg->znode_alloc, shape_axes, shape_dimens);
        znode->gradient = cg_tensor_create(cg->znode_alloc, shape_axes, shape_dimens);
    } else {
        znode->payload = NULL;
        znode->gradient = NULL;
    }

    cg_graph_add_vertex(znode->vertex);
    
    return znode;
}

int cg_train(cg_t*);
int cg_predict(cg_t*);
int cg_save_model(cg_t*, const char* path);
int cg_load_model(cg_t*, const char* path);
int cg_show_roadmap(cg_t*);