/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-05-24 09:56:43
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-06-14 08:33:09
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE{}
 */
#include <stdlib.h>
#include <stdio.h>
#include "cg_debug.h"
#include "cg_opt_base.h"
#include "cg_graph.h"
#include "cg_list.h"
#include "cg_znode_base.h"
#include "cg_base.h"

static int __recycle_path(cg_ref_t ref) 
{
    cg_list_t* path = (cg_list_t*) ref;
    cg_list_recycle(path);
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
    znode->curr_gradient_version = 0;
    
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

static int __combine_znode(cg_base_t* cg, cg_znode_base_t* com_znode)
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
                cg_znode_base_t* znode = (cg_znode_base_t*) e;
                // __regist_znode(cg, znode);
                cg_graph_link((cg_znode_base_t*)e, com_znode);
                break;
            case e_opt:
                // 将 opt 挂成一串
                cg_opt_base_t* opt;
                cg_opt_base_t* com_opt = com_znode->opt;
                while (com_opt && com_opt->next) com_opt = com_opt->next;
                com_opt->next = opt;
                opt->next = NULL;
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
       if ( __do_forward((cg_znode_base_t*)first) != 0 ) break;
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
            cg_znode_base_t* superior = CG_LIST_TOP(path)->prev->ref;
            
            if (superior->curr_gradient_version < curr_version) {
               int ret =  __do_gradient(J, superior, curr_version);
               if (ret != 0) return ret;
            }
        }

        cg_opt_base_t* opt = start->opt;
        while (opt)
        {
            // 真正干活的地方，外包给别人了
            if (opt->bp && !opt->bp(start, NULL)) return -1;
            opt = opt->next;
        }
    }
    return 0;
}

int cg_base_init(cg_base_t* cg)
{
    cg->compute_graph  = cg_graph_init(&cg->compute_graph);
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

int cg_add_znode(cg_base_t* cg, cg_znode_base_t* znode)
{
    cg_list_push(cg->znode_list, znode);
    return 0;
}  

cg_flow_elem_t* cg_flow_beg(cg_base_t* cg)
{
    return cg_list_push(cg->flow_stack, flow_beg())->ref;
}

cg_flow_elem_t* cg_flow_end(cg_base_t* cg, cg_znode_base_t* znode)
{
    cg_list_push(cg->flow_stack, flow_end());
    __combine_znode(cg, znode);
    return znode;
}

cg_flow_elem_t* cg_flow_push(cg_base_t* cg, cg_flow_elem_t* e)
{
    return cg_list_push(cg->flow_stack, e)->ref;
}


int cg_do_forward(cg_base_t* cg, cg_node_base_t* J) 
{
    int ret = __do_forward(J);
    cg->gradient_version;
    return 0;
}

int cg_do_gradient(cg_base_t* cg, cg_znode_base_t* J, cg_list_t* gradient_znodes) 
{
    cg_node_t* first   = CG_LIST_TOP(gradient_znodes);
    while(first != CG_LIST_HEAD(gradient_znodes)) {
        if (!__do_gradient(J, first->ref, cg->gradient_version)) return -1;
        first = first->prev;
    }
    return 0;
}

int cg_update_version(cg_base_t* cg)
{
    return cg->gradient_version++;
}

int cg_show_roadmap(cg_base_t* cg)
{
    return 0;
}