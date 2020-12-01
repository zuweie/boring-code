/*
 * @Author: your name
 * @Date: 2020-12-01 14:40:41
 * @LastEditTime: 2020-12-01 14:45:54
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/graph/udgraph_component.h
 */

#ifndef _UD_GRAPH_COMPONENT_H_
#define _UD_GRAPH_COMPONENT_H_
#include "container/Tv.h"
#include "ud_graph.h"

static int 
DEFAULT_MATCH_UVERTEX(Tv v1, Tv v2) 
{
    uvertex_t* pv = t2p(v1);
    return Tv_equl(pv->id, v2);
}

static int
DEFAULT_MATCH_UEDGE_WITH_2_EP(Tv v1, Tv v2)
{
    uedge_t* pedge = t2p(v1);
    Tv* vertexs = t2p(v2);
    return (Tv_equl(pedge->epv->id, vertexs[0]) | Tv_equl(pedge->epw->id, vertexs[1])) ? (Tv_equl(pedge->epw->id, vertexs[0]) | Tv_equl(pedge->epv->id, vertexs[1])) : 0;
}

static int
DEFAULT_MATCH_UEDGE_WITH_1_EP(Tv v1, Tv v2) 
{
    uedge_t* pedge = t2p(v1);
    return Tv_equl(pedge->epv->id, v2)? Tv_equl(pedge->epw->id, v2) : 0;
}

#endif