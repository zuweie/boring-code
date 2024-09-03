/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2024-09-02 13:58:03
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2024-09-03 15:28:20
 * @FilePath: /boring-code/src/deep_learning/compute_graph/cg_graph.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __CG_GRAPH_H__
#define __CG_GRAPH_H__

/// @brief 一个简单的有向图的顶点
typedef struct _cg_vertex{

    const char *p_id;
    char visit_mark;
    cg_list_t* p_income_vertexes;
    cg_list_t* p_outcome_vertexes;
    
} cg_vertex_t;

/// @brief 一个简单的有向图
typedef struct _cg_graph{
    cg_list_t* p_vertexes;
} cg_graph_t;

int cg_graph_init(cg_graph_t* p_graph);
int cg_graph_link(cg_vertex_t* p_from, cg_vertex_t* p_to);
cg_vertex_t* cg_graph_add_vertex(cg_graph_t* p_graph, const char* id);
int cg_graph_search_paths(cg_vertex_t* p_star, cg_vertex_t* p_end, cg_list_t* p_paths)
int cg_graph_recycle(cg_graph_t* p_graph);

#endif 