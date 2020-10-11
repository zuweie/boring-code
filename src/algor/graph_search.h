/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-20 09:44:49
 * @LastEditTime: 2020-06-11 11:06:39
 * @LastEditors: Please set LastEditors
 */
#ifndef _GRAPH_SEARCH_H_
#define _GRAPH_SEARCH_H_
#include "graph.h"
#include "container/link_array.h"
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
    grp_color_t color;
    vertex_t* pi;
} dfs_explor_t;

int grp_bfs(Graph* graph, vertex_t* start);
int grp_dfs(Graph* graph);

void grp_cleanup_exploring_info(Graph* graph);
int grp_bfs_path(Graph* graph, vertex_t* start, vertex_t* desc, LinkArray* arr);
#endif