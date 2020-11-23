/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-14 10:13:53
 * @LastEditTime: 2020-11-23 15:16:03
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
Graph* Graph_create_reverse(Graph*);
int Graph_destroy(Graph* graph);
int Graph_add_vertex(Graph* graph, Tv vertex);
int Graph_add_edge(vertex_t* from, vertex_t* to, float weight);
int Graph_del_vertex(vertex_t* vertex);
int Graph_del_edge(vertex_t* from, vertex_t* edge);
int Graph_get_edge_matrix(Graph* origin, CooMatrix* matrix);
int Graph_add_edge_by_matrix(Graph* graph, CooMatrix* matrix);
vertex_t* Graph_get_vertex(Graph* graph, Tv vertex_id);
edge_t* Graph_get_edge(vertex_t* from, Tv to_id);
int Graph_set_edge_matrix(Graph* graph, Tv from, Tv to, CooMatrix* matrix);
#endif