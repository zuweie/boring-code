/*
 * @Author: your name
 * @Date: 2020-11-27 23:10:16
 * @LastEditTime: 2020-11-30 13:15:56
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/graph/undirect_graph.h
 */

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
UDGraph* UDGraph_destroy(UDGraph*);

int UDGraph_add_vertex(UDGraph*, Tv);
int UDgraph_add_edge(UDGraph*, Tv, Tv);

int UDGraph_del_vertex(UDGraph*, Tv);
int UDGraph_del_edge(UDgraph*, Tv, Tv);