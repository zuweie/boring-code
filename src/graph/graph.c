/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-14 10:14:04
 * @LastEditTime: 2025-06-09 12:18:05
 * @LastEditors: zuweie jojoe.wei@gmail.com
 */
//#include "container/cn.h"
#include "container/Entity.h"
#include "graph.h"

static int __match_vertex(T* t1, T* t2) 
{
    vertex_t* pver1  = T_ptr(t1);
    unsigned long id = T_ulong(t2);
    int cmp = (pver1->vertex_id == id) ? 0 : 1;
    return cmp;
}

static int __match_path(T* t1, T* t2)
{
    path_t* path = T_ptr(t1);
    unsigned long id = T_ulong(t2);
    return (path->to->vertex_id == id) ? 0 : 1;
}
static vertex_t* __create_vertex(Graph* graph, unsigned long vertex_id) 
{
    // 生成一个顶点
    vertex_t* v =(vertex_t*) malloc (sizeof (vertex_t) + graph->exploring_size);
    v->vertex_id = vertex_id;
    // 这个找
    v->paths = CN_create(LIST|customized_compare, ptr_t, graph->match_path);
    if (graph->exploring_size){
        v->exploring = &v[1];
    } else {
        v->exploring = NULL;
    }
    return v;
}

static path_t* __create_path(vertex_t* to, float weight) 
{
    // 生成邻接表的节点
    path_t* node = (path_t*) malloc (sizeof(path_t)); 
    node->to = to;
    node->weight = weight;
    return node;
}



Graph* Graph_create(int exploring_size) 
{
    // 初始化图
    Graph* graph = (Graph*) malloc (sizeof(Graph));
    graph->vertexes = CN_create(LIST|customized_compare, ptr_t, &__match_vertex);
    graph->match_path   = &__match_path;
    graph->match_vertex = &__match_vertex;
    graph->exploring_size = exploring_size;
    return graph;
} 

Graph* Graph_create_reverse(Graph* graph) 
{
    Graph* new_graph        = (Graph*) malloc (sizeof(Graph));
    new_graph->vertexes     = CN_create(LIST|customized_compare, ptr_t, graph->match_vertex);
    new_graph->match_path   = graph->match_path;
    new_graph->match_vertex = graph->match_vertex;
    new_graph->init_exploring = graph->init_exploring;
    new_graph->exploring_size = graph->exploring_size;
    
    // 注入复制定点
    for (Iter first = CN_first(graph->vertexes); !It_equal(first, CN_tail(graph->vertexes)); first=It_next(first)){
        vertex_t* v = It_ptr(first);
        Graph_add_vertex(new_graph, v->vertex_id);
    }
    
    CooMatrix* cooMatrix = CooMatrix_create(CN_size(graph->vertexes), CN_size(graph->vertexes));
    Graph_get_paths_matrix(graph, cooMatrix);
    Matrix_trans(cooMatrix);
    Graph_connect_vertexes(new_graph, cooMatrix);
    CooMatrix_destroy(cooMatrix);
    
    return new_graph;
}

int Graph_destroy(Graph* graph) 
{
    // 把顶点删了。
    for (Iter first = CN_first(graph->vertexes); 
        !It_equal(first, CN_tail(graph->vertexes)); 
        first = It_next(first)) {
            vertex_t* pv = It_ptr(first);
            Graph_del_vertex(pv);
    }
    //List_(&graph->vertexes, NULL);
    CN_finalize(graph->vertexes, NULL);
    graph->match_path   = NULL;
    graph->match_vertex = NULL;
    
    free(graph);
    return 0;
}

vertex_t* Graph_add_vertex(Graph* graph, unsigned long vertex_id) 
{
    vertex_t* v = __create_vertex(graph, vertex_id);
    CN_add(graph->vertexes, v);
    return v;
}

vertex_t* Graph_get_vertex(Graph* graph,  unsigned long vertex_id) 
{
    Iter i = CN_find(graph->vertexes, vertex_id);
    return !It_is_tail(i) ? It_ptr(i) : NULL;
}

path_t* Graph_add_path(Graph* graph, unsigned long from_id, unsigned long to_id, float weight)
{
    // 首先得找一下 开始点 到 终结点 是不是在图中。
    vertex_t* from = Graph_get_vertex(graph, from_id);
    vertex_t* to   = Graph_get_vertex(graph, to_id);
    if (from && to) {
        path_t *p = __create_path(to, weight);
        CN_add(from->paths, p);  
        return p;
    }
    return NULL;
}

int Graph_del_vertex(vertex_t* vertex)
{
    path_t* path;
    // free the edge of the vertex
    while (CN_remove(vertex->paths, &path) == err_ok){
        free(path);
    }
    //List_(&vertex->paths, NULL);
    CN_finalize(vertex->paths, NULL);
    free(vertex);
    return 0;
}

int Graph_del_path(vertex_t* from, unsigned long to)
{
    Iter del = CN_find(from->paths, to);
    path_t* del_path;
    CN_remove_at(from->paths, del, &del_path);
    free(del_path);
    return 0;
}

// vertex_t* Graph_get_vertex(Graph* graph,  unsigned long vertex_id) 
// {
//     It i = CN_find(graph->vertexes, vertex_id);
//     return It_valid(i) ? It_getptr(i) : NULL;
// }

// path_t* Graph_get_path(vertex_t* from, unsigned long to_id) 
// {
//     It i = CN_find(&from->paths, to_id);
//     return It_valid(i) ? It_getptr(i) : NULL;
// }

int Graph_get_paths_matrix(Graph* graph, CooMatrix* matrix) 
{
    
    size_t size = CN_size(graph->vertexes);

    if (Matrix_rows(matrix) == size && Matrix_cols(matrix) == size ) {

        Graph_indexing_vertex(graph);

        for (Iter first = CN_first(graph->vertexes);
            !It_equal(first, CN_tail(graph->vertexes));
            first = It_next(first)){

            vertex_t *pvertex = It_ptr(first);

            for (Iter first2 = CN_first(pvertex->paths);
                !It_equal(first2, CN_tail(pvertex->paths));
                first2 = It_next(first2)){

                path_t *path = It_ptr(first2);
                Matrix_set(matrix, pvertex->index, path->to->index, path->weight);
            }
        }
        return 0;
    }
    return -1;
} 

// 根据矩阵连线。
int Graph_connect_vertexes(Graph* graph, CooMatrix* coomatrix)
{
    size_t size = CN_size(graph->vertexes);
    if (Matrix_rows(coomatrix) == size && Matrix_cols(coomatrix) == size ) {

        unsigned long vertex_id[size];
        int i=0;
        for (Iter first=CN_first(graph->vertexes); !It_equal(first, CN_tail(graph->vertexes)); first=It_next(first), ++i) {
            vertex_id[i] = ((vertex_t*)It_ptr(first))->vertex_id;
        }

        for (Iter first = CN_first(coomatrix->coo); !It_equal(first, CN_tail(coomatrix->coo)); first=It_next(first)) {
            entity_t* ent = It_ptr(first);
            unsigned int x = ef_uint(ent, 0);
            unsigned int y = ef_uint(ent, 1);
            float w = ef_float(ent, 2);
            Graph_add_path(graph, vertex_id[x], vertex_id[y], w);
       }
       return 0;
    }
    return -1;
}

int Graph_initialize_exploring(Graph* graph, int (*initialize)(void* exploring)) 
{
    for(Iter first = CN_first(graph->vertexes); !It_equal(first, CN_tail(graph->vertexes)); first=It_next(first)) {
        vertex_t* vertex = It_ptr(first);
        initialize(vertex->exploring);
    }
    return 0;
}

void Graph_indexing_vertex(Graph* graph) 
{
    size_t i =0;
    for (Iter first = CN_first(graph->vertexes); !It_equal(first, CN_tail(graph->vertexes)); first=It_next(first)) {
        vertex_t* vertex = It_ptr(first);
        vertex->index = i++;
    }
}