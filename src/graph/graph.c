/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-14 10:14:04
 * @LastEditTime: 2020-11-23 14:46:56
 * @LastEditors: Please set LastEditors
 */
#include "container/cn.h"
#include "container/HashMap.h"
#include "graph.h"
static int _vertex_id_hasher (Tv v1, size_t slot_size) 
{
    Entity* entity = t2p(v1);
    size_t id  = t2i(entity->tv[0]);
    size_t key = id % slot_size;
    return key;
}

static int _bulid_vertexes_id_indexing(Graph* graph, Map map) 
{
    int i =0;
    for (It first = CN_first(graph->vertexes);
        !It_equal(first, CN_tail(graph->vertexes));
        first = It_next(first)) {
            vertex_t* v = It_getptr(first);
            Map_set(map, v->vertex_id, i2t(i++));
    }
    return i;

}

static vertex_t* _create_vertex(Graph* graph, Tv vertex) 
{
    // 生成一个顶点
    vertex_t* v =(vertex_t*) malloc (sizeof (vertex_t));
    v->vertex_id = vertex;
    // 这个找
    v->edges = _List(graph->match_edge);    
    return v;
}

static edge_t* _create_edge(vertex_t* to, float weight) 
{
    // 生成邻接表的节点
    edge_t* node = (edge_t*) malloc (sizeof(edge_t)); 
    node->to = to;
    node->weight = weight;
    return node;
}

Graph* Graph_create(int(*match_vertex)(Tv, Tv), int(*match_edge)(Tv, Tv)) 
{
    // 初始化图
    Graph* graph = (Graph*) malloc (sizeof(Graph));
    graph->vertexes = _List(match_vertex);
    graph->match_edge   = match_edge;
    graph->match_vertex = match_vertex;
    return graph;
} 

Graph* Graph_create_reverse(Graph* graph) 
{
    Graph* new_graph = (Graph*) malloc (sizeof(Graph));
    new_graph->vertexes = _List(graph->match_vertex);
    new_graph->match_edge = graph->match_edge;
    new_graph->match_vertex = graph->match_vertex;
    
    // 注入复制定点
    for (It first = CN_first(graph->vertexes); !It_equal(first, CN_tail(graph->vertexes)); first = It_next(first)){
        vertex_t* v = It_getptr(first);
        Graph_add_vertex(new_graph, v->vertex_id);
    }
    
    CooMatrix* cooMatrix = CooMatrix_create(CN_size(graph->vertexes), CN_size(graph->vertexes));
    Graph_get_edge_matrix(graph, cooMatrix);
    Matrix_trans(cooMatrix);
    Graph_add_edge_by_matrix(new_graph, cooMatrix);
    CooMatrix_destroy(cooMatrix);
    
    return new_graph;
}

int Graph_destroy(Graph* graph) 
{
    // 把顶点删了。
    for (It first = CN_first(graph->vertexes); 
        !It_equal(first, CN_tail(graph->vertexes)); 
        first = It_next(first)) {

            vertex_t* pv = It_getptr(first);
            Graph_del_vertex(pv);
    }
    List_(graph->vertexes, NULL);
    graph->match_edge   = NULL;
    graph->match_vertex = NULL;
    free(graph);
    return 0;
}

int Graph_add_vertex(Graph* graph, Tv vertex) 
{
    vertex_t* v = _create_vertex(graph, vertex);
    return CN_add_tail(graph->vertexes, p2t(v));
}

int Graph_add_edge(vertex_t* from, vertex_t* to, float weight)
{
    // 首先得找一下 开始点 到 终结点 是不是在图中。
    //if (Graph_getEdge(from, to->vertex_id) == NULL) {
        edge_t *p = _create_edge(to, weight);
        return CN_add_tail(from->edges, p2t(p));
    //}
}

int Graph_del_vertex(vertex_t* vertex)
{
    Tv rnode;
    // free the edge of the vertex
    It it = CN_last(vertex->edges);
    while (CN_rm_last(vertex->edges, &rnode) != -1){
        free(t2p(rnode));
    }
    List_(vertex->edges, NULL);
    free(vertex);
    return 0;
}

int Graph_del_edge(vertex_t* from, vertex_t* to)
{
    Tv rnode;
    if (CN_rm_target(from->edges, to->vertex_id, &rnode) != -1)
    {
        free(t2p(rnode));
    }
    return 0;
}

vertex_t* Graph_get_vertex(Graph* graph, Tv vertex_id) 
{
    It i = CN_find(graph->vertexes, vertex_id);
    return It_valid(i) ? It_getptr(i) : NULL;
}

edge_t* Graph_get_edge(vertex_t* from, Tv to_id) {
    It i = CN_find(from->edges, to_id);
    return It_valid(i) ? It_getptr(i) : NULL;
}

int Graph_get_edge_matrix(Graph* graph, CooMatrix* matrix) 
{
    
    size_t size = CN_size(graph->vertexes);
    if (Matrix_rows(matrix) == size && Matrix_cols(matrix) == size ) {
        
        Map vertex_index_map = _Hashmap(_vertex_id_hasher);
        _bulid_vertexes_id_indexing(graph, vertex_index_map);
        

        for (It first = CN_first(graph->vertexes);
             !It_equal(first, CN_tail(graph->vertexes));
             first = It_next(first)){

            vertex_t *pvertex = It_getptr(first);

            for (It first2 = CN_first(pvertex->edges);
                 !It_equal(first2, CN_tail(pvertex->edges));
                 first2 = It_next(first2)){

                edge_t *pedge = It_getptr(first2);

                Tv vx, vy;
                Map_get(vertex_index_map, pvertex->vertex_id, vx);
                Map_get(vertex_index_map, pedge->to->vertex_id, vy);
                Matrix_set(matrix, t2i(vx), t2i(vy), pedge->weight);
            }
        }
        Hashmap_(vertex_index_map);

        return 0;
    }
    return -1;
} 

int  Graph_add_edge_by_matrix(Graph* graph, CooMatrix* coomatrix)
{
    size_t size = CN_size(graph->vertexes);
    if (Matrix_rows(coomatrix) == size && Matrix_cols(coomatrix) == size ) {
        Tv arr[size];
        CN_to_arr(graph->vertexes, arr);
        for (It first = CN_first(coomatrix->coo); !It_equal(first, CN_tail(coomatrix->coo)); first = It_next(first)) {
           Entity* entity = Hahsmap_node_2_entity(It_getptr(first));
           size_t x = t2i(entity->tv[0]);
           size_t y = t2i(entity->tv[1]);
           float  w = t2f(entity->tv[2]);
           vertex_t* from = t2p(arr[x]);
           vertex_t* to   = t2p(arr[y]);
           Graph_add_edge(from, to, w);
       }
       return 0;
    }
    return -1;
}
