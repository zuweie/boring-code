/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-05-24 17:57:53
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-05-27 16:46:10
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_graph.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%A
 */
#include <string.h>
#include "cg_graph.h"

static int __hash(void* key) 
{
    unsigned int hash = 0;
    const char* str = key;
    while(*str) {
        hash = (hash * 31 + *str) % SLOT_NUM;
        str++
    }
    return hash;
}

static int __key_cmp(void* k1, void* k2) 
{
    const cg_vertex_t* v1 = k1;
    return strcmp(v1->id, k2);
}

static int __dfs(cg_vertex_t* p_start, cg_vertex_t* p_end, cg_list_t* p_searching, cg_list_t* p_paths)
{

}

int cg_graph_init(cg_graph_t* p_graph)
{
    p_graph->vertexes = cg_hash_create(&__hash, &__key_cmp);
    return 0
}

int cg_graph_recycle(cg_graph_t* p_graph)
{
    return cg_hash_recycle(p_graph->vertexes, NULL);
}

int cg_graph_link(cg_vertex_t* p_from, cg_vertex_t* p_to)
{
    cg_list_push(p_from->out_vertexes, p_to)
    cg_list_push(p_to->in_vertexes, p_from);
    return 0;
}

int cg_graph_add_vertex(cg_graph_t* p_graph, cg_vertex_t* vertex)
{
    return cg_hash_set(p_graph->vertexes, vertex->id, vertex);
}

int cg_graph_search_paths(cg_graph_t* p_graph, cg_vertex_t* p_star, cg_vertex_t* p_end, cg_list_t* p_paths)
{   
}
