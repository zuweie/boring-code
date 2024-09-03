#include <stdlib.h>
#include <string.h>
#include "cg_list.h"
#include "cg_garph.h"

cg_vertex_t* __serch_vertex_by_id(cg_garph_t* p_garph, const char* id)
{
    cg_node_t* p_first = p_garph->p_vertexes->p_top;
    while (p_first != LIST_HEAD(p_garph->p_vertexes))
    {
        /* code */
        cg_vertex_t* p_vertex = (cg_vertex_t*)p_first->ref;
        if (strcmp(p_vertex->p_id, id) == 0) return p_vertex;
        p_first = p_first->prev;
    }
    return NULL;
}

int __unset_vertex_visit_mark(cg_garph_t* p_garph) 
{
    cg_node_t* p_frist = p_garph->p_vertexes->p_top;
    while (p_frist != LIST_HEAD(p_garph->p_vertexes))
    {
        cg_vertex_t* p_vertex = (cg_vertex_t*)p_frist->ref;
        p_vertex->visit_mark = 0;
    }
    return0;
}

cg_vertex_t* __vertex_create(const char* p_id) 
{
    cg_vertex_t* p_vertex = (cg_vertex_t*) malloc (sizeof(cg_vertex_t));
    p_vertex->p_id = id;
    p_vertex->p_income_vertexes = cg_list_create();
    p_vertex->p_outcome_vertexes = cg_list_create();
    p_vertex->visit_mark = 0;
    return p_vertex;
}
int __vertex_recycle(cg_vertex_t* p_vertex) 
{
    cg_list_recycle(p_vertex->p_income_vertexes);
    cg_list_recycle(p_vertex->p_outcome_vertexes);
    return 0;
}

int __dfs_search(cg_vertex_t* p_start, cg_vertex_t* p_end, cg_list_t* p_searching, cg_list_t* p_paths) 
{
    p_start->visit_mark = 1;
    cg_list_push(p_searching, p_start);
    if (p_start == p_end) {
        // 找到了，copy 路径，
        cg_list_t* path = cg_list_create_cpy(p_searching);
        cg_list_push(p_paths, path);
    } else {
        // 继续向前进
        cg_node_t* p_first = p_start->p_outcome_vertexes->p_top;
        while (p_first != LIST_HEAD(p_start->p_outcome_vertexes))
        {
            cg_vertex_t* p_vertex = (cg_vertex_t*) p_vertex->ref;
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

int cg_garph_init(cg_garph_t* p_garph)
{
    p_garph->p_vertexes = cg_list_create();
}

int cg_garph_link(const char* p_from_id, const char* p_to_id)
{
    cg_vertex_t* p_from = __search_vertex_by_id(p_from_id);
    cg_vertex_t* p_to   = __search_vertex_by_id(p_to_id);
    // 
    cg_list_push(p_to->p_income_vertexes, p_from);
    cg_list_push(p_from->p_outcome_vertexes, p_to);
    return 0;
}
cg_vertex_t* cg_garph_add_vertex(cg_garph_t* p_garph, const char* p_id)
{
    cg_vertex_t* p_vertex = __vertex_create(p_id);
    cg_list_push(p_garph->p_vertexes, p_vertex);
    return p_vertex;
}
int cg_garph_search_paths(cg_garph_t* p_garph, const char* p_start_id, const char* p_end_id, cg_list_t* p_paths)
{
    // 1 设置所有的顶点的 visit mark 为 0，即未曾访问。
    __unset_vertex_visit_mark();
    cg_vertex_t* p_start = __search_vertex_by_id(p_start_id);
    cg_vertex_t* p_end   = __search_vertex_by_id(p_end_id);
    cg_list_t*   p_searching_path = cg_list_create();
    __dfs_search(p_start, p_end, p_searching_path, p_paths);
    cg_list_recycle(p_searching_path);
    return 0;
}
int cg_garph_recycle(cg_garph_t* p_garph)
{
    return cg_list_recycle(p_garph->p_vertexes, __vertex_recycle);
}