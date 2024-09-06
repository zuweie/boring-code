/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2024-09-03 10:21:20
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2024-09-06 19:48:45
 * @FilePath: /boring-code/src/deep_learning/compute_graph/cg_graph.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdlib.h>
#include <string.h>
#include "cg_list.h"
#include "cg_graph.h"

static int __unset_vertex_visit_mark(cg_graph_t* p_graph) 
{
    cg_node_t* p_frist = p_graph->p_vertexes->p_top;
    while (p_frist != LIST_HEAD(p_graph->p_vertexes))
    {
        cg_vertex_t* p_vertex = (cg_vertex_t*)p_frist->ref;
        p_vertex->visit_mark = 0;
        p_frist = p_frist->prev;
    }
    return 0;
}

static cg_vertex_t* __vertex_create(const char* str_id) 
{
    cg_vertex_t* p_vertex = (cg_vertex_t*) malloc (sizeof(cg_vertex_t));
    p_vertex->str_id = str_id;
    p_vertex->p_income_vertexes = cg_list_create();
    p_vertex->p_outcome_vertexes = cg_list_create();
    p_vertex->visit_mark = 0;
    return p_vertex;
}

static int __vertex_recycle(cg_vertex_t* p_vertex) 
{
    // 解除路径
    cg_list_recycle(p_vertex->p_income_vertexes, NULL);
    cg_list_recycle(p_vertex->p_outcome_vertexes, NULL);
    // 启动自毁
    free(p_vertex);
    return 0;
}

int __dfs_search(cg_vertex_t* p_start, cg_vertex_t* p_end, cg_list_t* p_searching, cg_list_t* p_paths) 
{
    p_start->visit_mark = 1;
    cg_list_push(p_searching, p_start);
    if (p_start == p_end) {
        // 找到了，copy 路径，
        cg_list_t* path = cg_list_create_cpy(p_searching);
        cg_list_revert(path);
        cg_list_push(p_paths, path);
    } else {
        // 继续向前进
        cg_node_t* p_first = p_start->p_outcome_vertexes->p_top;
        while (p_first != LIST_HEAD(p_start->p_outcome_vertexes))
        {
            cg_vertex_t* p_vertex = (cg_vertex_t*) p_first->ref;
            if (p_vertex->visit_mark == 0) {
                __dfs_search(p_vertex, p_end, p_searching, p_paths);
            }
            p_first = p_first->prev;
        }
    }
    cg_list_pop(p_searching);
    p_start->visit_mark = 0;
    return 0;
}

int cg_graph_init(cg_graph_t* p_graph)
{
    p_graph->p_vertexes = cg_list_create();
}

int cg_graph_link(cg_vertex_t* p_from, cg_vertex_t* p_to)
{
    // 
    cg_list_push(p_to->p_income_vertexes, p_from);
    cg_list_push(p_from->p_outcome_vertexes, p_to);
    return 0;
}
cg_vertex_t* cg_graph_add_vertex(cg_graph_t* p_graph, const char* p_id)
{
    cg_vertex_t* p_vertex = __vertex_create(p_id);
    cg_list_push(p_graph->p_vertexes, p_vertex);
    return p_vertex;
}

int cg_graph_search_paths(cg_graph_t* p_graph, cg_vertex_t* p_start, cg_vertex_t* p_end, cg_list_t* p_paths)
{
    // 1 设置所有的顶点的 visit mark 为 0，即未曾访问。
    __unset_vertex_visit_mark(p_graph);
    cg_list_t* p_searching = cg_list_create();
    __dfs_search(p_start, p_end, p_searching, p_paths);
    cg_list_recycle(p_searching, NULL);
    p_searching = NULL;
    return 0;
}
int cg_graph_recycle(cg_graph_t* p_graph)
{
    return cg_list_recycle(p_graph->p_vertexes, __vertex_recycle);
}