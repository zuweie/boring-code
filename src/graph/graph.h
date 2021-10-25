/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-14 10:13:53
 * @LastEditTime: 2021-10-25 12:50:38
 * @LastEditors: Please set LastEditors
 */
#ifndef __GRAPH_H__
#define __GRAPH_H__

#include "container/ty.h"
#include "container/cn.h"
#include "matrix/CooMatrix.h"

typedef struct _vertex 
{
    /* vertex id */
    unsigned long vertex_id;
    /* bfs 与 dfs 算法的临时信息 */
    void* exploring;
    /* 定点索引 */
    int index;
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
int Graph_add_vertex(Graph* graph, unsigned long vertex);
int Graph_add_path(vertex_t* from, vertex_t* to, float weight);
int Graph_del_vertex(vertex_t* vertex);
int Graph_del_path(vertex_t* from, vertex_t* to);
int Graph_get_paths_matrix(Graph* origin, CooMatrix* matrix);
int Graph_connect_vertexes(Graph* graph, CooMatrix* matrix);
vertex_t* Graph_get_vertex(Graph* graph, unsigned long vertex_id);
path_t* Graph_get_path(vertex_t* from, unsigned long to_id);
int Graph_set_path_matrix(Graph* graph, T* from, T* to, CooMatrix* matrix);
int Graph_initialize_exploring(Graph* graph, int (*)(void*));
void Graph_indexing_vertex(Graph* graph);

#endif