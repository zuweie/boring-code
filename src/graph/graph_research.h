/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-20 09:44:49
 * @LastEditTime: 2020-11-24 14:17:23
 * @LastEditors: Please set LastEditors
 */
#ifndef _GRAPH_SEARCH_H_
#define _GRAPH_SEARCH_H_
#include "graph.h"
#include "container/List.h"
typedef enum _grp_color {
    _grp_whtie = 0,
    _grp_gray  = 1,
    _grp_black = 2
} grp_color_t;

typedef struct _bfs_explor {
    int distance;
    grp_color_t color;
    vertex_t* pi;
}bfs_explor_t; 

typedef struct _dfs_explor {
    int d_time;
    int f_time;
    int component_id;
    grp_color_t color;
    vertex_t* pi;
} dfs_explor_t;

int grp_bfs_exploring(Graph* graph, vertex_t* start);
int grp_dfs_exploring(Graph* graph);

void grp_cleanup_exploring(Graph* graph);
int grp_bfs_path(Graph* graph, vertex_t* start, vertex_t* desc, List arr);
int grp_topological_sort(Graph* graph);
int grp_calculate_component(Graph* graph, List list);
Graph* grp_calculate_strongly_connected_component_graph (Graph* graph);
#endif