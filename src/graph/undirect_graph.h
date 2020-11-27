/*
 * @Author: your name
 * @Date: 2020-11-27 23:10:16
 * @LastEditTime: 2020-11-28 00:43:18
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/graph/undirect_graph.h
 */

#include "container/Tv.h"
#include "container/List.h"

typedef struct _undirect_graph_vertex {
    Tv id;
} uvertex_t;

typedef struct _undirected_graph_edge {
    uvertex_t* endpoint_a;
    uvertex_t* endpoint_b;
    float weight;
} uedge_t;

typedef struct _undirected_graph {

    List uvertexs;
    List uedges;

} Ugraph;