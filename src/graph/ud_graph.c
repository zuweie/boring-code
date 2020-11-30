/*
 * @Author: your name
 * @Date: 2020-11-27 23:10:30
 * @LastEditTime: 2020-11-30 14:47:29
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/graph/undirect_graph.c
 */

#include "container/Tv.h"
#include "container/List.h"
#include "ud_graph.h"

static uvertex_t* _create_uvertex(Tv vertex_id) 
{
       
}

static uedge_t* _create_uedge(uvertex_t* vertex1, uvertex_t* vertex2)
{
    
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
    List_(udgraph->uedges, NULL);
    List_(udgraph->uvertexs, NULL);
    free(udgraph);
}

int UDGraph_add_vertex(UDGraph* udgraph, Tv vertex_id) 
{
    
}

int UDgraph_add_edge(UDGraph* udgraph, Tv vertex_1, Tv vertex_2) 
{
    
}

int UDGraph_del_vertex(UDGraph* udgraph, Tv vertex_id) 
{
    
}

int UDGraph_del_edge(UDgraph* udgraph, Tv vertex_1, Tv vertex_2)
{
    
}