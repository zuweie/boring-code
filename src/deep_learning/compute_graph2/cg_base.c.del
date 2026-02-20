/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-05-24 09:56:43
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-06-17 13:23:35
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE{}
 */
#include <stdlib.h>
#include <string.h>
#include "base/container_of.h"
#include "cg_list.h"
#include "cg_hash.h"
#include "cg_graph.h"
#include "cg_debug.h"
#include "cg_opt_base.h"
#include "cg_flow.h"
#include "cg_znode_base.h"
#include "cg_base.h"

static int __marker_hash(void* key) 
{
    unsigned int hash = 0;
    const char* str = key;
    while(*str) {
        hash = (hash * 31 + *str) % SLOT_NUM;
        str++;
    }
    return hash;
}

static int __marker_cmp(void* k1, void* k2) 
{
    return strcmp(k1, k2);
}

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
                    cg_opt_base_t*  opt = (cg_opt_base_t*) e;
                    if (!com_znode->opt) 
                        com_znode->opt = opt;
                    else {
                        cg_opt_base_t* next = com_znode->opt;
                        while (next->next) next = next->next;
                        next->next = opt;
                    }
                }
                break;
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

static int __do_forward(cg_znode_base_t* J, cg_hash_t* marker) 
{
    if (!cg_hash_has(marker, J->vertex.id))
    {
        if (J->vertex.in_vertexes)
        {
            // 如果有 sub_node 把 sub_node 的 payload 搞定，再搞自己。
            cg_node_t *first = CG_LIST_TOP(J->vertex.in_vertexes);
            while (first != CG_LIST_HEAD(J->vertex.in_vertexes))
            {
                cg_vertex_t *vertex = first->ref;
                cg_znode_base_t *znode = container_of(vertex, cg_znode_base_t, vertex);
                if (__do_forward(znode, marker) != 0)
                    break;
                first = first->prev;
            }
        }
        
        int ret;
        cg_opt_base_t *opt = J->opt;
        while (opt)
        {
            // opt->fp 应该不为 NULL，为 NULL 就让其爆炸。
            if ((ret = opt->fp(J, NULL)) != 0) return ret;
            opt = opt->next;
        }
        
        cg_hash_set(marker, J->vertex.id, NULL);
    }

    return 0;
} 

static int __do_gradient(cg_znode_base_t* J, cg_znode_base_t* start, cg_hash_t* marker) 
{
    if (J != start && !cg_hash_has(marker, start->vertex.id)) {
        // do gradient
        if (start->gradient_paths == NULL) {
            // 从这点到终点 err 的偏导路径为空，先建立一个偏导的路径。
            // 建立从这点到终点的偏导路径。
            start->gradient_paths = cg_list_create();
            cg_graph_search_paths(&start->vertex, &J->vertex, start->gradient_paths);
        }

        // 遍历该节点上所有可能到终点的路径，然后确保它们的前趋的梯度是最新的。
        int ret;
        cg_node_t* first = CG_LIST_TOP(start->gradient_paths);
        while (first != CG_LIST_HEAD(start->gradient_paths)) {

            cg_list_t* path = (cg_list_t*) first->ref;
            
            // 偏导路径第一个是自己，那么第二个就是它的前趡。
            // 注意： 这里 path 列表中存入的是 vertex 指针，而不是 znode 的指针。需要通过 container_of 来反推 znode 的指针。
            cg_vertex_t* superior_vertex = CG_LIST_TOP(path)->prev->ref;
            cg_znode_base_t* superior = container_of(superior_vertex, cg_znode_base_t, vertex);
    
            // 若发现其上级的梯度不是最新的，那么先求上级的梯度。
            if ( !cg_hash_has(marker, superior->vertex.id)) {
                if ((ret = __do_gradient(J, superior, marker)) != 0) return ret;
            } 
            
            // 上级梯度求完，现在求上级对本级节点的梯度。
            cg_opt_base_t* opt = superior->opt;
            while(opt) {
                // opt 的 bp 应该是存在的，不存就让其爆炸，找出逻辑错误。
                if ((ret = opt->bp(superior, start, NULL)) != 0) return ret;
                opt = opt->next;
            }
            // 继续下一个求导路径
            first = first->prev;
        }
        // 做完所有的梯度求值后，更新当前节点的 梯度版本
        cg_hash_set(marker, start->vertex.id, NULL);
    }
    return 0;
}

int cg_base_init(cg_base_t* cg)
{
    cg_graph_init(&cg->compute_graph);
    cg->flow_stack       = cg_list_create();
    cg->forward_marker   = cg_hash_create(__marker_hash,__marker_cmp);
    cg->gradient_marker  = cg_hash_create(__marker_hash,__marker_cmp);
    return 0;
}

int cg_base_reset(cg_base_t* cg)
{
    cg_graph_reset(&cg->compute_graph);
    cg_list_recycle(cg->flow_stack, __recycle_flow_stack_elem);
    cg->flow_stack = NULL;
    cg_hash_recycle(cg->forward_marker, NULL);
    cg_hash_recycle(cg->gradient_marker, NULL);
    return 0;
}

int cg_base_reset_marker(cg_base_t* cg)
{
    cg_hash_reset(cg->forward_marker, NULL);
    cg_hash_reset(cg->gradient_marker, NULL);
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
    return __do_forward(J, cg->forward_marker);
}

int cg_do_gradient(cg_base_t* cg, cg_znode_base_t* J, cg_znode_base_t* gradient_node) 
{
    return __do_gradient(J, gradient_node, cg->gradient_marker);
}
