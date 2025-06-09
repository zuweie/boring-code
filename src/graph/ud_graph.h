/*
 * @Author: your name
 * @Date: 2020-11-27 23:10:16
 * @LastEditTime: 2025-06-09 12:08:20
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/graph/undirect_graph.h
 */
#ifndef __UD_GRAPH_H__
#define __UD_GRAPH_H__

#include "container/Typ.h"
#include "container/Cont.h"

typedef struct uvertex {

    unsigned long id;
    void* exploring;
    size_t index;
} uvertex_t;

typedef struct _uedge {
    uvertex_t* epv;
    uvertex_t* epw;
    float weight;
} uedge_t;

typedef struct _udgraph {

    CN uvertexs;
    CN uedges;
    int exploring_size;
} UDGraph;

UDGraph* UDGraph_create(size_t);
int UDGraph_destroy(UDGraph*);

uvertex_t* UDGraph_add_vertex(UDGraph*, unsigned long);
uedge_t* UDGraph_add_edge(UDGraph*, unsigned long, unsigned long,  float);

// int UDGraph_del_vertex(UDGraph*, unsigned long);
// int UDGraph_del_edge(UDGraph*, unsigned long, unsigned long);

void UDGraph_indexing_vertex(UDGraph*);
#endif