/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-05-24 09:56:43
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-06-12 13:45:48
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

static int __recycle_flow_stack_elem(cg_ref_t ref) 
{
    cg_flow_elem_t* e = (cg_flow_elem_t*) ref;
    if (e->elem_type == e_beg || e->elem_type == e_end) free(e);
    return 0;
}

static int __recycle_znode(cg_ref_t ref) 
{
    cg_znode_t* znode = (cg_znode_t*) ref;
    return cg_znode_recycle(znode)

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
       if ( __do_forward((cg_znode_t*)first) != 0 ) break;
    }
    if (J->opt->fp) 
        return J->opt->fp(J, NULL);
} 

static int __do_gradient(cg_znode_t* J, cg_znode_t* start, int curr_version) 
{
    if (J != start && start->curr_gradient_version == curr_version) {
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
               int ret =  __do_gradient(J, superior, curr_version);
               if (ret != 0) return ret;
            }
        }

        // 最后算梯度
        int ret = 0;

        if (start->opt->bp) {
            ret = start->opt->bp(start);
        }

        if (ret)
            start->curr_gradient_version = curr_version;

        return ret;
    }
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
    return e;
}


cg_flow_elem_t* cg_comb_zonde (cg_t* cg, znode_type_t znode_type)
{   
    int opt_number = 0, znode_number = 0, beg_number = 0;

    // 检查一下是否有足够的元素可以生产操作节点。
    // 有至少一个 znode 和 一个 opt 对象
    cg_node_t* first = CG_LIST_TOP(cg->flow_stack);
    cg_flow_elem_t* elem = first->ref;

    if (elem->elem_type != e_end) return NULL;

    first = first->prev;

    for (;first !=CG_LIST_HEAD(cg->flow_stack); first = first->prev) {

        elem = first->ref;

        if (elem->elem_type == e_znode) znode_number++;

        else if (elem->elem_type == e_opt) opt_number++;

        if (elem->elem_type == e_beg) {
            beg_number++;
            break;
        };
    }
    // 已经确保所有的数据完整，可以生成 znode 了。
    if (beg_number==1 && opt_number==1 && znode_number>0) {
        cg_znode_t* znode = cg_create_zonde(cg, znode_type);
        cg_flow_elem_t* e = cg_list_pop(flow_stack);
        while (e->elem_type != e_beg) {
            switch (e->elem_type)
            {
                case e_znode:
                    cg_znode_t* _znode = (cg_zonde_t*)e;
                    cg_graph_link(&_znode->vertex, &znode->vertex)
                    break;
                case e_opt:
                    znode->opt = (cg_opt_base_t*)e;
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
        return (cg_flow_elem_t) znode;
    } else {
        if (beg_number == 0)   CG_DEBUG("error finish stack data with out begin");
        if (opt_number == 0)   CG_DEBUG("no opt");
        if (opt_number > 1)    CG_DEBUG("more than 1 opt");
        if (znode_number == 0) CG_DEBUG("no znode");
    }
    return NULL;
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

int cg_update_data_version(cg_t* cg)
{
    return ++(cg->data_version);    
}

int cg_show_roadmap(cg_t*)
{
    return 0;
}