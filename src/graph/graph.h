/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-14 10:13:53
 * @LastEditTime: 2020-11-22 23:08:04
 * @LastEditors: Please set LastEditors
 */
#ifndef _GRAPH_H_
#define _GRAPH_H_

#include "container/Tv.h"
#include "container/List.h"
#include "matrix/CooMatrix.h"

typedef struct _vertex 
{
    /* vertex id */
    Tv vertex_id;
    /* bfs 与 dfs 算法的临时信息 */
    void* exploring;
    /* 本顶点在列表中的索引，用于生成连接矩阵 */
    int indexing;
    /* 邻接表 */
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
    int (*match_edge)(Tv node, Tv find);
    int (*match_vertex) (Tv node, Tv find);
} Graph;

Graph* Graph_create(int(*)(Tv, Tv), int(*)(Tv, Tv));
int Graph_destroy(Graph* graph);
int Graph_add_vertex(Graph* graph, Tv vertex);
int Graph_add_edge(vertex_t* from, vertex_t* to, float weight);
int Graph_del_vertex(vertex_t* vertex);
int Graph_del_edge(vertex_t* from, vertex_t* edge);
int Graph_indexing_vertexes(Graph* graph);
int Graph_get_edge_matrix(Graph* origin, CooMatrix* matrix);
int Graph_add_edge_by_matrix(Graph* graph, CooMatrix* matrix);
int Graph_get_vertexes_id(Graph* graph, List vertexes);
vertex_t* Graph_get_vertex(Graph* graph, Tv vertex_id);
edge_t* Graph_get_edge(vertex_t* from, Tv to_id);
#endif