/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-05-24 17:58:01
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-06-17 11:16:00
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_graph.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __CG_GRAPH_H__
#define __CG_GRAPH_H__

typedef struct cg_list cg_list_t;
typedef struct cg_hash cg_hash_t;
typedef struct cg_vertex cg_vertex_t;

typedef struct cg_graph {
    cg_hash_t* vertexes;
} cg_graph_t;

int cg_graph_init(cg_graph_t* p_graph);
int cg_graph_reset(cg_graph_t* p_graph);

int cg_graph_add_vertex(cg_graph_t* p_graph, cg_vertex_t* vertex);
int cg_graph_link(cg_vertex_t* p_from, cg_vertex_t* p_to);
int cg_graph_search_paths(cg_vertex_t* p_start, cg_vertex_t* p_end, cg_list_t* p_paths);
int cg_graph_inspect(cg_vertex_t* p_start);

#endif