/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-05-24 17:57:53
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-06-15 11:06:11
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_graph.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%A
 */
#include <string.h>
#include "cg_graph.h"

static int __vertex_hash(void* key) 
{
    unsigned int hash = 0;
    const char* str = key;
    while(*str) {
        hash = (hash * 31 + *str) % SLOT_NUM;
        str++;
    }
    return hash;
}

static int __vertex_cmp(void* k1, void* k2) 
{
    const cg_vertex_t* v1 = k1;
    return strcmp(v1->id, k2);
}

static int __marker_hash(void* key) 
{
    unsigned int hash = 0;
    const char* str = key;
    while(*str) {
        hash = (hash * 31 + *str) % SLOT_NUM;
        str++;
    }
    return hash;
}

static int __marker_cmp(void* k1, void* k2) 
{
    return strcmp(k1, k2);
}

static int __vertexes_recycle(cg_ref_t ref) 
{
    cg_vertex_t* vertex = (cg_vertex_t*)ref;
    cg_list_recycle(vertex->out_vertexes, NULL);
    cg_list_recycle(vertex->in_vertexes, NULL);
    vertex->out_vertexes = NULL;
    vertex->in_vertexes  = NULL;
    return 0;
}

static int __deep_first_search(cg_vertex_t* p_start, cg_vertex_t* p_end, cg_hash_t* p_marker, cg_list_t* p_searching, cg_list_t* p_paths)
{
    cg_hash_set(p_marker, p_start->id, NULL);
    cg_list_push(p_searching, p_start);
    if (p_start == p_end) {

        cg_list_t* path = cg_list_create_cpy(p_searching);
        cg_list_revert(path);
        cg_list_push(p_paths, path);
        
    } else {

        cg_node_t* p_first = CG_LIST_TOP(p_start->out_vertexes);
        while (p_first != CG_LIST_HEAD(p_start->out_vertexes))
        {
            cg_vertex_t* p_vertex = (cg_vertex_t*) p_first->ref;
            if (!cg_hash_has(p_marker, p_vertex->id)) {
                __deep_first_search(p_vertex, p_end, p_marker, p_searching, p_paths);
            }
            p_first = p_first->prev;
        }
    }

    cg_ref_t pop = cg_list_pop(p_searching);
    cg_hash_del(p_marker, p_start->id);
    return 0;
}

int cg_graph_init(cg_graph_t* p_graph)
{
    p_graph->vertexes = cg_hash_create(&__vertex_hash, &__vertex_cmp);
    return 0;
}

int cg_graph_reset(cg_graph_t* p_graph)
{
    // 此 graph 的 vertex 对象并不需要 graph 来维护。
    return cg_hash_recycle(p_graph->vertexes, __vertexes_recycle);
}

int cg_graph_link(cg_vertex_t* p_from, cg_vertex_t* p_to)
{
    cg_list_push(p_from->out_vertexes, p_to);
    cg_list_push(p_to->in_vertexes, p_from);
    return 0;
}

int cg_graph_add_vertex(cg_graph_t* p_graph, cg_vertex_t* vertex)
{
    return cg_hash_set(p_graph->vertexes, vertex->id, vertex);
}

int cg_graph_search_paths(cg_vertex_t* p_start, cg_vertex_t* p_end, cg_list_t* p_paths)
{   
    cg_list_t* p_searching = cg_list_create();
    cg_hash_t* p_marker    = cg_hash_create(__marker_hash, __marker_cmp);
    __deep_first_search(p_start, p_end, p_marker, p_searching, p_paths);
    cg_list_recycle(p_searching, NULL);
    cg_hash_recycle(p_marker, NULL);
    p_searching = NULL;
    return 0;
}

int cg_graph_inspect(cg_vertex_t* start)
{
    return 0;
}
