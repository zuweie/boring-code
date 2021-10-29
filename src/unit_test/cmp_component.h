/*
 * @Author: your name
 * @Date: 2020-12-03 13:43:00
 * @LastEditTime: 2020-12-03 21:16:30
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/cn_component.h
 */
#ifndef _CN_COMPONENT_H_
#define _CN_COMPONENT_H_

#include "container/Tv.h"
#include "container/Entity.h"
#include "graph/graph.h"
#include "graph/ud_graph.h"

// 把所有的比对函数全部搞到一切，可以复用，不用搞来搞去。真是操蛋

/* hashmap 的健值以 int 形式生成 hashkey */ 
static int 
entity_int_keyhasher(Tv v, size_t slot_size)
{
    Entity* entity = t2p(v);
    size_t key = 0;
    for (int i=0; i<entity->value_index; ++i) {
        key += t2i(entity->tv[i]);
    }
    return key % slot_size;
}

/* tree map 的插入比较，以int的形式。 */
static int
entity_int_insert_cmp (Tv v1, Tv v2){
    Entity* e1 = t2p(v1);
    Entity* e2 = t2p(v2);
    if (e1->value_index == e2->value_index) {
        for (int i=0; i<e1->value_index; ++i) {
            
            // 直接用 int 的方式来比较大小。
            int ret = Tv_cmpi(e1->tv[i], e2->tv[i]);
            if (ret !=0 )return ret;
        }
        // 相等。
        return 0;
    } else {
        return e1->value_index > e2->value_index ? 1: -1;
    }
}


static int
graph_match_vertex(Tv v1, Tv v2) 
{
    vertex_t* pv = t2p(v1);
    return Tv_equl(pv->vertex_id, v2);
}

static int
graph_match_path(Tv v1, Tv v2) 
{
    path_t* path = t2p(v1);
    return Tv_equl(path->to->vertex_id, v2);
}


static int 
udgraph_match_vertex(Tv v1, Tv v2) 
{
    uvertex_t* pv = t2p(v1);
    return Tv_equl(pv->id, v2);
}

static int
udgraph_match_edge_2p(Tv v1, Tv v2)
{
    uedge_t* pedge = t2p(v1);
    Tv* vertexs = t2p(v2);
    return (Tv_equl(pedge->epv->id, vertexs[0]) | Tv_equl(pedge->epw->id, vertexs[1])) ? (Tv_equl(pedge->epw->id, vertexs[0]) | Tv_equl(pedge->epv->id, vertexs[1])) : 0;
}

static int
udgraph_match_edge_1p(Tv v1, Tv v2) 
{
    uedge_t* pedge = t2p(v1);
    return Tv_equl(pedge->epv->id, v2)? Tv_equl(pedge->epw->id, v2) : 0;
}

#endif