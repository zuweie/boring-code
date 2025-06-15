/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-05-24 09:56:43
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-06-15 09:25:57
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE{}
 */
#include <stdlib.h>
#include <stdio.h>
#include "base/container_of.h"
#include "cg_debug.h"
#include "cg_opt_base.h"
#include "cg_graph.h"
#include "cg_list.h"
#include "cg_flow.h"
#include "cg_znode_base.h"
#include "cg_base.h"

static int __recycle_path(cg_ref_t ref) 
{
    cg_list_t* path = (cg_list_t*) ref;
    cg_list_recycle(path, NULL);
    return 0;
}


static int __recycle_flow_stack_elem(cg_ref_t ref) 
{
    cg_flow_elem_t* e = (cg_flow_elem_t*) ref;
    if (e->elem_type == e_beg || e->elem_type == e_end) free(e);
    return 0;
}

static int __recycle_znode(cg_ref_t ref) 
{
    cg_znode_base_t* znode = (cg_znode_base_t*) ref;
    cg_list_recycle(znode->gradient_paths, __recycle_path);
    znode->gradient_paths = NULL;
    znode->gradient_version = 0;
    
    while (znode->opt)
    {   
        cg_opt_base_t* opt = znode->opt;
        znode->opt = znode->opt->next;
        
        if (opt->recycle)
            opt->recycle(opt);
        free(opt);
    }
    return 0;
}

static cg_znode_base_t* __combine_znode(cg_base_t* cg, cg_znode_base_t* com_znode)
{
    // 第一个步先查看 com_znode 是否注册到 cg_graph 中，如果
    // __regist_znode(cg, com_znode);
    cg_flow_elem_t* e = cg_list_pop(cg->flow_stack);
    while (e->elem_type != e_beg)
    {
        if (com_znode) {
            switch (e->elem_type)
            {
            case e_znode:
                cg_graph_link(&((cg_znode_base_t*)e)->vertex, &com_znode->vertex);
                break;
            case e_opt:
                // 将 opt 挂成一串
                {
                    cg_opt_base_t* opt = (cg_opt_base_t*) e;
                    cg_opt_base_t* com_opt = com_znode->opt;
                    while (com_opt && com_opt->next) com_opt = com_opt->next;
                    com_opt->next = opt;
                    opt->next = NULL;
                }
            default:
                // free end
                free(e);
                break;
            }
        }
        e = cg_list_pop(cg->flow_stack);
    }
    // free beg
    free(e);
    return com_znode;
}

static int __do_forward(cg_znode_base_t* J) 
{
    cg_node_t* first = CG_LIST_TOP(J->vertex.in_vertexes);
    while (first != CG_LIST_HEAD(J->vertex.in_vertexes)) {
        cg_vertex_t* vertex = first->ref;
        cg_znode_base_t* znode = container_of(vertex, cg_znode_base_t, vertex);
       if ( __do_forward(znode) != 0 ) break;
    }
    cg_opt_base_t* opt = J->opt;
    while (opt)
    {
        if (opt->fp && !opt->fp(J, NULL) ) return -1;
        opt = opt->next;
    }
    return 0;
} 

static int __do_gradient(cg_znode_base_t* J, cg_znode_base_t* start, int curr_version) 
{
    if (J != start && start->gradient_version < curr_version) {
        // do gradient
        if (start->gradient_paths == NULL) {
            // 从这点到终点 err 的偏导路径为空，先建立一个偏导的路径。
            // 建立从这点到终点的偏导路径。
            start->gradient_paths = cg_list_create();
            cg_graph_search_paths(&start->vertex, &J->vertex, start->gradient_paths);
        }

        // 遍历该节点上所有可能到终点的路径，然后确保它们的前趋的梯度是最新的。

        cg_node_t* first = CG_LIST_TOP(start->gradient_paths);
        while (first != CG_LIST_HEAD(start->gradient_paths)) {

            cg_list_t* path = (cg_list_t*) first->ref;
            
            // 偏导路径第一个是自己，那么第二个就是它的前趡。
            // 注意： 这里 path 列表中存入的是 vertex 指针，而不是 znode 的指针。需要通过 container_of 来反推 znode 的指针。
            cg_vertex_t* superior_vertex = CG_LIST_TOP(path)->prev->ref;
            cg_znode_base_t* superior = container_of(superior_vertex, cg_znode_base_t, vertex);
    
            if (superior->gradient_version < curr_version) {
               int ret =  __do_gradient(J, superior, curr_version);
               if (ret != 0) return ret;
            }
        }

        cg_opt_base_t* opt = start->opt;
        while (opt)
        {
            // 有多个 opt->bp 就干多次活
            if (opt->bp && !opt->bp(start, NULL)) return -1;
            opt = opt->next;
        }
    }
    return 0;
}

int cg_base_init(cg_base_t* cg)
{
    cg_graph_init(&cg->compute_graph);
    cg->flow_stack     = cg_list_create();
    cg->gradient_version = 0;
    return 0;
}

int cg_base_reset(cg_base_t* cg)
{
    cg_graph_reset(&cg->compute_graph);
    cg_list_recycle(cg->flow_stack, __recycle_flow_stack_elem);
    cg->flow_stack = NULL;
    cg->gradient_version = 0;
    return 0;
}

cg_flow_elem_t* cg_flow_beg(cg_base_t* cg)
{
    return cg_list_push(cg->flow_stack, flow_beg())->ref;
}

cg_flow_elem_t* cg_flow_end(cg_base_t* cg, cg_znode_base_t* znode)
{
    cg_list_push(cg->flow_stack, flow_end());
    return __combine_znode(cg, znode);
}

cg_flow_elem_t* cg_flow_push(cg_base_t* cg, cg_flow_elem_t* e)
{
    return cg_list_push(cg->flow_stack, e)->ref;
}


int cg_do_forward(cg_base_t* cg, cg_znode_base_t* J) 
{
    if (__do_forward(J) == 0) {
        cg->gradient_version++;
        return 0;
    }
    
    return -1;
}

int cg_do_gradient(cg_base_t* cg, cg_znode_base_t* J, cg_znode_base_t* gradient_node) 
{
   return __do_gradient(J, gradient_node, cg->gradient_version);
}

int cg_show_roadmap(cg_base_t* cg)
{
    return 0;
}