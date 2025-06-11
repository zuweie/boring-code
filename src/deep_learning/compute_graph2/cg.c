/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-05-24 09:56:43
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-06-11 14:28:15
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE{}
 */
#include <stdlib.h>
#include <stdio.h>
#include "cg_debug.h"
#include "cg_opt_base.h"
#include "cg_graph.h"
#include "cg_list.h"
#include "cg_znode.h"
#include "compute_graph.h"

static int __recycle_path(cg_ref_t ref) 
{
    cg_list_t* list = (cg_list_t*) ref;
    cg_list_recycle(list, NULL);
}

static int __recycle_flow_stack_elem(cg_ref_t ref) 
{
    cg_flow_elem_t* e = (cg_flow_elem_t*) ref;
    if (e->elem_type == e_beg || e->elem_type == e_end) free(e);
    return 0;
}

static int __recycle_znode(cg_ref_t ref) 
{
    cg_znode_t* znode = (cg_znode_t*) ref;

    if (znode->payload) 
        cg_tensor_recycle(znode->payload);

    if (znode->gradient)
        cg_tensor_recycle(znode->gradient);

    if (znode->opt){
        znode->opt->recycle(znode->opt);
        free(znode->opt);
    } 
    
    // free gradient paths
    cg_list_recycle(znode->gradient_paths, __recycle_path);

    free(znode);

    return 0;

}

static cg_znode_t* __find_znode(cg_t* cg, znode_type_t znode_type) 
{
    cg_node_t* first = CG_LIST_TOP( cg->znode_list );
    cg_znode_t* loss = NULL;
    while( first != CG_LIST_HEAD(cg->znode_list)) {
        loss = first->ref;
        if (loss->znode_type == znode_type) return loss;
        first = first->prev;
    }
    return NULL;
}

static int __do_forward(cg_znode_t* J) 
{
    cg_node_t* first = CG_LIST_TOP(J->vertex.in_vertexes);
    while (first != CG_LIST_HEAD(J->vertex.in_vertexes)) {
        __do_forward((cg_znode_t*)first);
    }
    if (J->opt->fp) J->opt->fp(J, NULL);
    return 0;
} 

static int __do_gradient(cg_znode_t* J, cg_znode_t* start, int curr_version) 
{
    if (J != start && start->curr_gradient_version < curr_version) {
        // do gradient
        if (start->gradient_paths == NULL) {
            // 从这点到终点 err 的偏导路径为空，先建立一个偏导的路径。
            // 建立从这点到终点的偏导路径。
            start->gradient_paths = cg_list_create();
            cg_graph_search_paths(&start->vertex, &J->vertex, start->gradient_paths);
        }

        // 根据路径上节点找到
        cg_node_t* first = CG_LIST_TOP(start->gradient_paths);
        while (first != CG_LIST_HEAD(start->gradient_paths)) {

            cg_list_t* path = (cg_list_t*) first->ref;
            
            // 偏导路径第一个是自己，那么第二个就是它的前趡。
            cg_znode_t* superior = CG_LIST_TOP(path)->prev->ref;
            
            if (superior->curr_gradient_version < curr_version) {
                __do_gradient(J, superior, curr_version);
            }
        }

        // 最后算梯度
        if (start->opt->bp) start->opt->bp(start);

        start->curr_gradient_version = curr_version
    }
    return 0;
}

int cg_forward_propagation(cg_t* cg) 
{
    cg_node_t* first = CG_LIST_TOP(cg->znode_list);
    cg_znode_t* J    = __find_znode(cg, e_loss);
    __do_forward(J);
    cg->data_version++;
    return 0;
}

int cg_backward_propagation(cg_t* cg) 
{
    cg_node_t* first   = CG_LIST_TOP(cg->znode_list);
    cg_znode_t* J      = __find_znode(cg, e_loss);
    cg_znode_t* znode  = NULL;

    while (first != CG_LIST_HEAD(cg->znode_list)) {

        znode = first->ref;
        if (znode->znode_type != e_X || znode->znode_type != e_y) {
            __do_gradient(J, znode, cg->data_version);
        }
    }
    return 0;
}

static int __mount_zonde(cg_znode_t* znode, cg_list_t* flow_stack)
{   
    cg_flow_elem_t* e = cg_list_pop(flow_stack);
    while (e->elem_type != e_beg) {
        switch (e->elem_type)
        {
            case e_znode:
                cg_znode_t* _znode = (cg_zonde_t*)e;
                cg_graph_link(&_znode->vertex, &znode->vertex)
                break;
            case e_opt:
                if (znode->opt == NULL) {
                    znode->opt = (cg_opt_base_t*)e;
                } else {
                    CG_DEBUG("ERROR: znode->opt has been assign twice !!");
                    return -1;
                }
                break; 
            default:
                // after use e_end, free thiz guy
                free(e);
                break;
        }
        e = cg_list_pop(flow_stack);
    }
    // 这里是 e_beg, free this guy.
    free(e);
    return 0;
}

int cg_init(cg_t* cg, cg_allocator_t* alloc)
{
    cg->compute_graph  = cg_graph_init(&cg->compute_graph);
    cg->znode_list     = cg_list_create();
    cg->flow_stack     = cg_list_create();
    cg->data_version  = 0;
    cg->znode_count   = 0;
    return 0;
}

int cg_reset(cg_t* cg)
{
    cg_graph_reset(&cg->compute_graph);

    // 清除 flow stack 中的残留 elem
    cg_list_recycle(cg->flow_stack, __recycle_flow_stack_elem);
    // 释放所有 znode 的空间
    cg_list_recycle(cg->znode_list, __recycle_znode);
    cg->data_version = 0;
    cg->znode_count  = 1;
    return 0;
}

cg_flow_elem_t* cg_flow_push(cg_t* cg, cg_flow_elem_t* e)
{
    cg_list_push(cg->flow_stack, e);

    if (e->typ == e_end) {
        
        // 生产一个 new 中间节点
        cg_znode_t*  znode = (cg_znode_t*) cg_create_zonde(cg, NULL, 0, NULL);
        __mount_zonde(znode, cg->flow_stack);

        return (cg_flow_elem_t*) znode;
    }
    return e;
}

cg_flow_elem_t* cg_create_zonde(cg_t* cg, const char* znode_name, zonde_type_t znode_type)
{
    cg->znode_count++;
    cg_znode_t* znode = (cg_znode_t*) malloc (sizeof(cg_znode_t));

    if (znode_name != NULL) {
        sprintf(znode->vertex.id, znode_name);
    } else {
        sprintf(znode->vertex.id, "znode_%d", cg->znode_count);
    }
    znode->compute_graph = cg;
    znode->flow_elem.typ = e_znode;
    znode->payload  = NULL;
    znode->gradient = NULL;
    znode->opt            = NULL;
    znode->gradient_paths = NULL;
    znode->znode_type     = znode_type;
    znode->curr_gradient_version = 0;
    cg_graph_add_vertex(&cg->compute_graph, &znode->vertex);
    cg_list_push(cg->znode_list, znode);

    return znode;
}

cg_flow_elem_t* cg_create_opt(cg_t* cg, void* param, cg_opt_base_t* (*opt_create)(void*))
{
    return opt_create(param);
}

cg_flow_elem_t* cg_create_beg_elem () 
{
    // elem will free in __mount_znode function
    cg_flow_elem_t* elem = (cg_flow_elem_t*) malloc (sizeof(cg_flow_elem_t));
    return *elem = (cg_flow_elem_t){.elem_type = e_beg};
}

cg_flow_elem_t* cg_create_end_elem()
{
    // elem will free in __mout_znode function
    cg_flow_elem_t* elem = (cg_flow_elem_t*) malloc (sizeof (cg_flow_elem_t));
    return * elem = (cg_flow_elem_t) {.elem_type = e_end}
}

int cg_update_data_version(cg_t* cg)
{
    return ++(cg->data_version);    
}

int cg_show_roadmap(cg_t*)
{
    return 0;
}