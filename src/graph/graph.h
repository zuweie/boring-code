/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-14 10:13:53
 * @LastEditTime: 2021-10-24 09:42:04
 * @LastEditors: Please set LastEditors
 */
#ifndef _GRAPH_H_
#define _GRAPH_H_

#include "container/ty.h"
#include "container/cn.h"
#include "matrix/CooMatrix.h"

typedef struct _vertex 
{
    /* vertex id */
    Tv vertex_id;
    /* bfs 与 dfs 算法的临时信息 */
    void* exploring;
    /* 定点索引 */
    size_t index;
    /* 邻接表 */
    CN paths;
} vertex_t;

typedef struct _path
{
    vertex_t* to;
    float weight;
    
} path_t;

typedef struct _graph
{
    /* data */
    CN vertexes;
    int (*match_path)(T* node, T* find);
    int (*match_vertex)(T* node, T* find);
    int (*init_exploring)(void* exploring);
    size_t exploring_size;
} Graph;

Graph* Graph_create(int(*)(T*, T*), int(*)(T*, T*), size_t);
Graph* Graph_create_reverse(Graph* graph);
int Graph_destroy(Graph* graph);
int Graph_add_vertex(Graph* graph, T* vertex);
int Graph_add_path(vertex_t* from, vertex_t* to, float weight);
int Graph_del_vertex(vertex_t* vertex);
int Graph_del_path(vertex_t* from, vertex_t* edge);
int Graph_get_paths_matrix(Graph* origin, CooMatrix* matrix);
int Graph_connect_vertexes(Graph* graph, CooMatrix* matrix);
vertex_t* Graph_get_vertex(Graph* graph, T* vertex_id);
path_t* Graph_get_path(vertex_t* from, T* to_id);
int Graph_set_path_matrix(Graph* graph, T* from, T* to, CooMatrix* matrix);
int Graph_initialize_exploring(Graph* graph, int (*)(void*));
void Graph_indexing_vertex(Graph* graph);

#endif