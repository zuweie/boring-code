/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-14 10:13:53
 * @LastEditTime: 2020-06-14 09:11:24
 * @LastEditors: Please set LastEditors
 */
#ifndef _GRAPH_H_
#define _GRAPH_H_

#include "container/tv.h"
#include "container/link_array.h"
#include "matrix/tsmatrix.h"

typedef struct _vertex 
{
    /* vertex id */
    tv vertex_id;
    /* bfs 与 dfs 算法的临时信息 */
    void* exploring;
    /* 本顶点在列表中的索引，用于生成连接矩阵 */
    int indexing;

    LinkArray edges;
} vertex_t;

typedef struct _edge
{
    vertex_t* to;
    float weight;
    
} edge_t;

typedef struct _graph
{
    /* data */
    LinkArray vertexes;
    int (*compare_edge)(tv node, tv find);
    int (*compare_vertex) (tv node, tv find);
} Graph;

int Graph_init(Graph* graph, int(*find_vertex)(tv, tv), int(*find_link)(tv, tv));
int Graph_free(Graph* graph);
int Graph_addVertex(Graph* graph, tv vertex);
int Graph_addEdge(vertex_t* from, vertex_t* to, float weight);
int Graph_delVertex(vertex_t* vertex);
int Graph_delEdge(vertex_t* from, vertex_t* edge);
int Graph_indexingVertexes(Graph* graph);
int Graph_getEdgeMatrix(Graph* origin, TSMatrix* matrix);
int Graph_addEdgeByMatrix(Graph* graph, TSMatrix* matrix, float weight);

vertex_t* Graph_getVertex(Graph* graph, tv vertex_id);
edge_t* Graph_getEdge(vertex_t* from, tv to_id);
#endif