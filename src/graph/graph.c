/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-14 10:14:04
 * @LastEditTime: 2020-10-15 08:23:37
 * @LastEditors: Please set LastEditors
 */
#include "container/cn.h"
#include "graph.h"

static vertex_t* _create_vertex(Graph* graph, Tv vertex) 
{
    // 生成一个顶点
    vertex_t* v =(vertex_t*) malloc (sizeof (vertex_t));
    v->vertex_id = vertex;
    // 这个找
    List_init(v->edges, graph->compare_edge);
    return v;
}
static int _free_vertex(vertex_t* vertex) {
    List_uninit(vertex->edges, NULL);
    free(vertex);
    return 0;
}
static edge_t* _create_edge(vertex_t* to, float weight) 
{
    // 生成邻接表的节点
    edge_t* node = (edge_t*) malloc (sizeof(edge_t)); 
    node->to = to;
    node->weight = weight;
    return node;
}
static int _free_edge (edge_t* pnode) 
{
    free(pnode);
}

int Graph_init(Graph* graph, int(*find_vertex)(Tv, Tv), int(*find_link)(Tv, Tv)) 
{
    // 初始化图
    List_init(graph->vertexes, find_vertex);
    graph->compare_edge   = find_link;
    graph->compare_vertex = find_vertex;
    return 0;
} 


int Graph_uninit(Graph* graph) 
{
    // 把图给干掉了
    
    // 把顶点删了。
    for (It first = CN_first(graph->vertexes); 
        !It_equal(first, CN_tail(graph->vertexes)); 
        first = It_next(first)) {

            vertex_t* pv = t2p(It_dref(first));

            Graph_delVertex(pv);
    }
    List_uninit(graph->vertexes, NULL);
    graph->compare_edge   = NULL;
    graph->compare_vertex = NULL;
    return 0;
}

int Graph_addVertex(Graph* graph, Tv vertex) 
{
    vertex_t* v = _create_vertex(graph, vertex);
    return CN_add_tail(graph->vertexes, p2t(v));
}

int Graph_addEdge(vertex_t* from, vertex_t* to, float weight)
{
    // 首先得找一下 开始点 到 终结点 是不是在图中。
    if (Graph_getEdge(from, to->vertex_id) == NULL) {
        edge_t *p = _create_edge(to, weight);
        return CN_add_tail(from->edges, p2t(p));
    }
}

int Graph_delVertex(vertex_t* vertex)
{
    Tv rnode;
    while (CN_rm_last(vertex->edges, &rnode) != -1)
    {
        _free_edge(t2p(rnode));
    }

    _free_vertex(vertex);
    return 0;
}

int Graph_delEdge(vertex_t* from, vertex_t* to)
{
    Tv rnode;
    if (CN_rm_target(from->edges, to->vertex_id, &rnode) != -1)
    {
        _free_edge(t2p(rnode));
    }
    return 0;
}


int Graph_indexingVertexes(Graph* graph) 
{
    int i =0;
    for (It first = CN_first(graph->vertexes);
        !It_equal(first, CN_tail(graph->vertexes));
        first = It_next(first)) {
            vertex_t* pv = It_getptr(first);
            pv->indexing = i ++;
    }
    return i;

}

vertex_t* Graph_getVertex(Graph* graph, Tv vertex_id) 
{
    It i = CN_find(graph->vertexes, vertex_id);
    return It_valid(i) ? It_getptr(i) : NULL;
}

edge_t* Graph_getEdge(vertex_t* from, Tv to_id) {
    It i = CN_find(from->edges, to_id);
    return It_valid(i) ? It_getptr(i) : NULL;
}

int Graph_getEdgeMatrix(Graph* graph, TSMatrix* tsmatrix) 
{
    
    size_t size = CN_size(graph->vertexes);
    if (tsmatrix->col == size && tsmatrix->row == size ) {
        Graph_indexingVertexes(graph);

        //Matrix* matrix = Matrix_create(size, size);

        for (It first = CN_first(graph->vertexes);
             !It_equal(first, CN_tail(graph->vertexes));
             first = It_next(first)){

            vertex_t *pvertex = It_getptr(first);

            for (It first2 = CN_first(pvertex->edges);
                 !It_equal(first2, CN_tail(pvertex->edges));
                 first2 = It_next(first2)){

                edge_t *pedge = It_getptr(first2);

                int x = pvertex->indexing;
                int y = pedge->to->indexing;

                TSMatrix_set(tsmatrix, x, y, 1.0f);
            }
        }

        return 0;
    }
    return -1;
} 

int Graph_addEdgeByMatrix(Graph* graph, TSMatrix* tsmatrix, float weight)
{
    size_t size = CN_size(graph->vertexes);
    if (tsmatrix->col == size && tsmatrix->row == size ) {
        Tv arr[size];
        CN_to_arr(graph->vertexes, arr);
        for (It first = CN_first(tsmatrix->elems); !It_equal(first, CN_tail(tsmatrix->elems)); first = It_next(first)) {
            ts_elem* elem = It_getptr(first);
            vertex_t* from = t2p(arr[elem->position.x]);
            vertex_t* to   = t2p(arr[elem->position.y]);
            Graph_addEdge(from, to, weight);
        }
       return 0;
    }
    return -1;
}
