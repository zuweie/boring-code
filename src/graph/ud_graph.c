/*
 * @Author: your name
 * @Date: 2020-11-27 23:10:30
 * @LastEditTime: 2021-11-02 15:26:57
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/graph/undirect_graph.c
 */

#include "container/cn.h"
#include "container/entity.h"
#include "ud_graph.h"
static int __match_uvertex(T* t1, T* t2) 
{
    uvertex_t* pv = T_ptr(t1);
    unsigned long id = T_ulong(t2);
    return (pv->id == id) ? 0 : 1;
}

static uvertex_t* __create_uvertex(UDGraph* graph, unsigned long vertex_id) 
{
    uvertex_t* vertex = malloc(sizeof(uvertex_t) + graph->exploring_size);
    vertex->id = vertex_id;
    if (graph->exploring_size) 
        vertex->exploring = &vertex[1];
    else
        vertex->exploring = NULL;
    return vertex;
}

static uedge_t* __create_uedge(uvertex_t* vertex1, uvertex_t* vertex2, float weight)
{
    uedge_t* edge = malloc(sizeof(uvertex_t));
    edge->epv = vertex1;
    edge->epw = vertex2;
    edge->weight = weight;
    return edge;
}

static void __free_uvertex(T* v) 
{
    uvertex_t* vertex = T_ptr(v);
    free(vertex);
}

static void __free_uedge(T* v) 
{
    uedge_t* edge = T_ptr(v);
    free(edge);
}

UDGraph* UDGraph_create(size_t exploring_size) 
{
    UDGraph* graph = malloc(sizeof(UDGraph));
    graph->exploring_size = exploring_size;
    graph->uvertexs = CN_create(LIST|customized_compare, ptr_t, &__match_uvertex); 
    graph->uedges   = CN_create(LIST,ptr_t);
    return graph;
}

int UDGraph_destroy(UDGraph* udgraph) 
{
    CN_finalize(udgraph->uedges, __free_uedge);
    CN_finalize(udgraph->uvertexs, __free_uvertex);
    free(udgraph);
}

uvertex_t* UDGraph_add_vertex(UDGraph* udgraph, unsigned long vertex_id) 
{
    uvertex_t* vertex = __create_uvertex(udgraph, vertex_id);
    CN_add(udgraph->uvertexs, vertex);
    return vertex;
}

uedge_t* UDGraph_add_edge(UDGraph* udgraph, unsigned long vertex_1, unsigned long vertex_2, float w) 
{
    It i_v1 = CN_find(udgraph->uvertexs, vertex_1);
    It i_v2 = CN_find(udgraph->uvertexs, vertex_2);

    if (!It_is_tail(i_v1) && !It_is_tail(i_v2)) {
        
        uvertex_t* v1 = It_ptr(i_v1);
        uvertex_t* v2 = It_ptr(i_v2);
        
        uedge_t* edge = __create_uedge(v1, v2, w);
        CN_add(udgraph->uedges, edge);

        return edge;
    }
    return NULL;
}

int UDGraph_del_vertex(UDGraph* udgraph, unsigned long vertex_id) 
{
    It it = CN_find(udgraph->uvertexs, &vertex_id, NULL);
    uvertex_t* del_vertex;
    if (err_ok == CN_remove_at(udgraph->uvertexs, it, &del_vertex)) {
        // 1 把所有有关的边都干掉
        for (It first=CN_first(udgraph->uedges); !It_equal(first, CN_tail(udgraph->uedges));) {
            uedge_t* edge = It_ptr(first);
            if (edge->epv == del_vertex->id || edge->epw == del_vertex->id) {
                It rm_it = first;
                It_next(first);
                CN_remove_at(udgraph->uedges, rm_it, NULL);
                free(edge);
            } else {
                It_next(first);
            }
        } 
    }
    return 0;
}

int UDGraph_del_edge(UDGraph* udgraph, unsigned long v1, unsigned long v2)
{
 
    for (It first=CN_first(udgraph->uedges); !It_equal(first, CN_tail(udgraph->uedges)); It_next(first)) {
        uedge_t* pedge = It_ptr(first);
        if ((pedge->epv->id == v1 && pedge->epw->id == v2) 
        || (pedge->epv->id == v2) && (pedge->epw->id == v1)) {
            CN_remove_at(udgraph->uedges, first, NULL);
            free(pedge);
            break;
        }
    }
    return 0;
}

void UDGraph_indexing_vertex(UDGraph* graph) 
{
    size_t i = 0;
    for (It first = CN_first(graph->uvertexs); !It_equal(first, CN_tail(graph->uvertexs)); It_next(first)) {
        uvertex_t* vertext = It_getptr(first);
        vertext->index     = i++;
    }
}


