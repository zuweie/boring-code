/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-14 10:13:53
 * @LastEditTime: 2020-10-15 07:47:58
 * @LastEditors: Please set LastEditors
 */
#ifndef _GRAPH_H_
#define _GRAPH_H_

#include "container/Tv.h"
#include "container/List.h"
#include "matrix/tsmatrix.h"

typedef struct _vertex 
{
    /* vertex id */
    Tv vertex_id;
    /* bfs 与 dfs 算法的临时信息 */
    void* exploring;
    /* 本顶点在列表中的索引，用于生成连接矩阵 */
    int indexing;

    List edges;
} vertex_t;

typedef struct _edge
{
    vertex_t* to;
    float weight;
    
} edge_t;

typedef struct _graph
{
    /* data */
    List vertexes;
    int (*compare_edge)(Tv node, Tv find);
    int (*compare_vertex) (Tv node, Tv find);
} Graph;

int Graph_init(Graph* graph, int(*find_vertex)(Tv, Tv), int(*find_link)(Tv, Tv));
int Graph_uninit(Graph* graph);
int Graph_addVertex(Graph* graph, Tv vertex);
int Graph_addEdge(vertex_t* from, vertex_t* to, float weight);
int Graph_delVertex(vertex_t* vertex);
int Graph_delEdge(vertex_t* from, vertex_t* edge);
int Graph_indexingVertexes(Graph* graph);
int Graph_getEdgeMatrix(Graph* origin, TSMatrix* matrix);
int Graph_addEdgeByMatrix(Graph* graph, TSMatrix* matrix, float weight);

vertex_t* Graph_getVertex(Graph* graph, Tv vertex_id);
edge_t* Graph_getEdge(vertex_t* from, Tv to_id);
#endif