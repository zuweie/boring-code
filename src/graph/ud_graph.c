/*
 * @Author: your name
 * @Date: 2020-11-27 23:10:30
 * @LastEditTime: 2020-12-03 23:28:14
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/graph/undirect_graph.c
 */

#include "container/Tv.h"
#include "container/List.h"
#include "container/Entity.h"
#include "container/HashMap.h"
#include "ud_graph.h"

static uvertex_t* _create_uvertex(UDGraph* graph, Tv vertex_id) 
{
    uvertex_t* vertex = malloc(sizeof(uvertex_t) + graph->exploring_size);
    vertex->id = vertex_id;
    if (graph->exploring_size) 
        vertex->exploring = &vertex[1];
    else
        vertex->exploring = NULL;
    return vertex;
}

static uedge_t* _create_uedge(uvertex_t* vertex1, uvertex_t* vertex2, float weight)
{
    uedge_t* edge = malloc(sizeof(uvertex_t));
    edge->epv = vertex1;
    edge->epw = vertex2;
    edge->weight = weight;
    return edge;
}

static void _free_uvertex(Tv v) 
{
    uvertex_t* vertex = t2p(v);
    free(vertex);
}

static void _free_uedge(Tv v) 
{
    uedge_t* edge = t2p(v);
    free(edge);
}

UDGraph* UDGraph_create(int (*match_vertex)(Tv, Tv), int(*match_edge)(Tv, Tv), size_t exploring_size) 
{
    UDGraph* graph = malloc(sizeof(UDGraph));
    graph->exploring_size = exploring_size;
    graph->uvertexs = _List(match_vertex);
    graph->uedges   = _List(match_edge);
    return graph;
}

int UDGraph_destroy(UDGraph* udgraph) 
{
    List_(udgraph->uedges, _free_uedge);
    List_(udgraph->uvertexs, _free_uvertex);
    free(udgraph);
}

int UDGraph_add_vertex(UDGraph* udgraph, Tv vertex_id) 
{
    uvertex_t* vertex = _create_uvertex(udgraph, vertex_id);
    CN_add(udgraph->uvertexs, p2t(vertex));
    return 0;
}

int UDGraph_add_edge(UDGraph* udgraph, Tv vertex_1, Tv vertex_2, float w) 
{
    It i_v1 = CN_find(udgraph->uvertexs, vertex_1);
    It i_v2 = CN_find(udgraph->uvertexs, vertex_2);

    if (!It_is_tail(i_v1) && !It_is_tail(i_v2)) {
        
        uvertex_t* v1 = It_getptr(i_v1);
        uvertex_t* v2 = It_getptr(i_v2);
        uedge_t* edge = _create_uedge(v1, v2, w);
        CN_add(udgraph->uedges, p2t(edge));

    }
    return 0;
}

int UDGraph_del_vertex(UDGraph* udgraph, Tv vertex_id, int (*match_edge)(Tv, Tv)) 
{
    Tv rdata;
    if (CN_rm_target(udgraph->uvertexs, vertex_id, &rdata) != -1) {
        // 1 把所有有关的边都干掉
        CN_eliminate(udgraph->uedges, vertex_id, match_edge, _free_uedge);
        uvertex_t* vertex = t2p(rdata);
        free(vertex);
    }
    return 0;
}

int UDGraph_del_edge(UDGraph* udgraph, Tv v1, Tv v2)
{
    Tv vertexs[2] = {v1, v2};
    Tv rdata;
    if (CN_rm_target(udgraph->uedges, p2t(vertexs), &rdata) != -1) {
        uedge_t* pedge = t2p(rdata);
        free(pedge);
    }
    return 0;
}

void UDGraph_indexing_vertex(UDGraph* graph) 
{
    size_t i = 0;
    for (It first = CN_first(graph->uvertexs); !It_equal(first, CN_tail(graph->uvertexs)); first = It_next(first)) {
        uvertex_t* vertext = It_getptr(first);
        vertext->index     = i++;
    }
}
