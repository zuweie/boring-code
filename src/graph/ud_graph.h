/*
 * @Author: your name
 * @Date: 2020-11-27 23:10:16
 * @LastEditTime: 2020-11-30 18:06:16
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/graph/undirect_graph.h
 */
#ifndef _UD_GRAPH_H_
#define _UD_GRAPH_H_

#include "container/Tv.h"
#include "container/List.h"

typedef struct uvertex {
    Tv id;
    void* exploring;
} uvertex_t;

typedef struct _uedge {
    uvertex_t* epv;
    uvertex_t* epw;
    float weight;
} uedge_t;

typedef struct _udgraph {

    List uvertexs;
    List uedges;
    size_t exploring_size;
} UDGraph;

UDGraph* UDGraph_create(int (*)(Tv, Tv), int(*)(Tv, Tv), size_t);
int UDGraph_destroy(UDGraph*);

int UDGraph_add_vertex(UDGraph*, Tv);
int UDGraph_add_edge(UDGraph*, Tv, Tv, float);

int UDGraph_del_vertex(UDGraph*, Tv);
int UDGraph_del_edge(UDGraph*, Tv, Tv);

static int 
DEFAULT_MATCH_UVERTEX(Tv v1, Tv v2) 
{
    uvertex_t* pv = t2p(v1);
    return Tv_equl(pv->id, v2);
}

static int
DEFAULT_MATCH_UEDGE(Tv v1, Tv v2)
{
    uedge_t* pedge = t2p(v1);
    Tv* vertexs = t2p(v2);
    return Tv_equl(pedge->epv->id, vertexs[0]) | Tv_equl(pedge->epw->id, vertexs[1]) ? Tv_equl(pedge->epw->id, vertexs[0]) | Tv_equl(pedge->epv->id, vertexs[1]) : 0;
}

#endif