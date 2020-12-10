/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-20 09:34:56
 * @LastEditTime: 2020-12-10 21:46:12
 * @LastEditors: Please set LastEditors
 */
#include "graph_research.h"
#include "container/Queue.h"
#include "container/Tv.h"
#include "container/It.h"
#include "container/List.h"
#include "container/HashMap.h"
#include "container/MxQueue.h"

#define PRIM_MAX 9999999.99f;

static int _topological_sort_cmp(Tv t1, Tv t2) 
{
    dfs_explor_t* v1 = ((vertex_t*)t2p(t1))->exploring;
    dfs_explor_t* v2 = ((vertex_t*)t2p(t2))->exploring;

    return INCMP_INT(i2t(v1->f_time), i2t(v2->f_time));
}

static int _udgaph_edge_weight_sort_cmp(Tv v1, Tv v2)
{
    uedge_t* e1 = t2p(v1);
    uedge_t* e2 = t2p(v2);
    
    return CMP_FLT(f2t(e1->weight), f2t(e2->weight));
}

static int _free_list_in_group_list(Tv v) 
{
    List* l = t2p(v);
    List_(*l, NULL);
    free(l);
}

static void _init_bfs_exploring(void* exploring) 
{
    bfs_explor_t* pn = exploring;
    pn->color = _grp_whtie;
    pn->distance = -1;
    pn->pi = NULL;
}

static void _init_dfs_exploring(void* exploring) 
{
    dfs_explor_t* pn = exploring;
    pn->color = _grp_whtie;
    pn->pi = NULL;
    pn->d_time = -1;
    pn->f_time = -1;
}

// 广度优先算法
int grp_bfs_exploring(Graph* graph, vertex_t* start) 
{
    Graph_initialize_exploring(graph, _init_bfs_exploring);
    bfs_explor_t* pbfs = (bfs_explor_t*)start->exploring;
    pbfs->color = _grp_gray;
    pbfs->distance = 0;
    pbfs->pi = NULL;

    Queue queue = _Queue(NULL);
    Queue_offer(queue, p2t(start));
    Tv rdata;
    
    while(Queue_poll(queue, &rdata) != -1) {

        vertex_t* pu = t2p(rdata);
        bfs_explor_t* pubfs = (bfs_explor_t*) pu->exploring;

        // 遍历节点的邻居表。
        for(It first = CN_first(pu->paths); 
            !It_equal(first, CN_tail(pu->paths)); 
            first=It_next(first)) {

            path_t* pv = It_getptr(first);
            bfs_explor_t* pvbfs  = (bfs_explor_t*) (pv->to->exploring);

            if (pvbfs->color == _grp_whtie) {
                pvbfs->color = _grp_gray;
                pvbfs->distance = pvbfs->distance + 1;
                pvbfs->pi = pu;
                Queue_offer(queue, p2t(pv->to));
            }
        }
        pubfs->color = _grp_black;
    }
    Queue_(queue,NULL);
    return 0;
}

static int _dfs_visit(vertex_t* pu, int* time) 
{
    dfs_explor_t* pudfs = (dfs_explor_t*) pu->exploring;
    pudfs->color = _grp_gray;
    pudfs->d_time = ++(*time);
    // 访问邻接表
    for(It first=CN_first(pu->paths); !It_equal(first, CN_tail(pu->paths)); first=It_next(first)) {
        path_t* pv   = It_getptr(first);
        dfs_explor_t* pvdfs = (dfs_explor_t*)pv->to->exploring;

        if (pvdfs->color == _grp_whtie) {
            pvdfs->pi = pu;
            _dfs_visit(pv->to, time);
        }
    }
    pudfs->color = _grp_black;
    pudfs->f_time = ++(*time);
    return 0;
}

// 深度优先算法
int grp_dfs_exploring(Graph* graph) 
{
    int time = -1;
    Graph_initialize_exploring(graph, _init_dfs_exploring);
    for(It first=CN_first(graph->vertexes); 
        !It_equal(first, CN_tail(graph->vertexes)); 
        first=It_next(first)) {

        vertex_t*   pu = It_getptr(first);
        dfs_explor_t* pudfs = (dfs_explor_t*)pu->exploring;
        if (pudfs->color == _grp_whtie) {
            _dfs_visit(pu, &time);
        }
        
    }
    return 0;
}

int grp_bfs_path(Graph* graph, vertex_t* start, vertex_t* desc, List arr) 
{
    if (start == desc) {
        CN_add_tail(arr, p2t(start));
    }else if (((bfs_explor_t*)(desc->exploring))->pi == NULL){
        return -1;
    }else {
        grp_bfs_path(graph, start, ((bfs_explor_t*)(desc->exploring))->pi, arr);
        CN_add_tail(arr, p2t(desc));
    }
    return 0;
}

// 再完成了 dfs 后启动的拓扑排序。
int grp_topological_sort(Graph* graph)
{
    CN_sort(graph->vertexes, _topological_sort_cmp);
}

// 计算有向图的强连通分支
Graph* grp_calculate_strongly_connected_component_graph(Graph* graph)
{
    grp_dfs_exploring(graph);
    grp_topological_sort(graph);
    Graph* reverse = Graph_create_reverse(graph);
    grp_dfs_exploring(reverse);
    return reverse;

}

int grp_calculate_component(Graph* graph, List list) 
{
    // 把所有的定点排好
    size_t arr_size = CN_size(graph->vertexes) * 2;
    vertex_t* vertex_arr[arr_size];

    for (It first = CN_first(graph->vertexes); !It_equal(first, CN_tail(graph->vertexes)); first = It_next(first)) {
        vertex_t* vertex = It_getptr(first);
        dfs_explor_t* dfs = vertex->exploring;
        vertex_arr[dfs->d_time] = vertex;
        vertex_arr[dfs->f_time] = vertex;
    }

    vertex_t* close_flag = vertex_arr[0];

    // 1 把第一个塞进去容器。
    CN_add_tail(list, p2t(close_flag));
    It split_from = CN_last(list);
    for (int i=1; i<arr_size; ++i) {
        vertex_t* curr_vertex = vertex_arr[i];

        if (close_flag == curr_vertex && i < arr_size-1) {
            CN_add_tail(list, p2t(NULL));
            close_flag = vertex_arr[++i];
            CN_add_tail(list, p2t(close_flag));
            split_from = CN_last(list);
        } else {
            It is_find = CN_search(list, split_from, p2t(curr_vertex));
            if (It_is_tail(is_find)) {
                CN_add_tail(list, p2t(curr_vertex));
            }
        }
    }
    return 0;
}

int ugrp_calculate_mst_kruskal(UDGraph* graph, List list) 
{
    List group_list = _List(NULL);
    
    for(It first = CN_first(graph->uvertexs); !It_equal(first, CN_tail(graph->uvertexs)); first = It_next(first)) {

        List* l = malloc(sizeof(List));
        *l = _List(NULL);//_Treeset(entity_insert_cmp);

        uvertex_t* vertex = It_getptr(first);
        // 为每个定点分配一个 list
        CN_add(*l, p2t(vertex));
        
        // 存入一个 list 中
        CN_add_tail(group_list, p2t(l));

        // mark 下自己节点所在的 group 的 iterator
        ((udg_kruskal_explor_t*)(vertex->exploring))->group_iterator = CN_last(group_list);

    }
    
        // 3 以变得 weigth 递增的方式来排序。
    CN_sort(graph->uedges, _udgaph_edge_weight_sort_cmp);

    // 遍历每一条边。
    for (It first = CN_first(graph->uedges); !It_equal(first, CN_tail(graph->uedges)); first = It_next(first)){
        uedge_t* edge = It_getptr(first);

        List* epv_list = It_getptr(((udg_kruskal_explor_t*)(edge->epv->exploring))->group_iterator);
        List* epw_list = It_getptr(((udg_kruskal_explor_t*)(edge->epw->exploring))->group_iterator);

        if (epv_list != epw_list) {

            It epv_list_it = ((udg_kruskal_explor_t*)(edge->epv->exploring))->group_iterator;
            It epw_list_it = ((udg_kruskal_explor_t*)(edge->epw->exploring))->group_iterator;
            
            for (It first = CN_first(*epw_list); !It_equal(first, CN_tail(*epw_list)); first = It_next(first)) {
                //Tv v = Set_get_item(*epw_list, first);
                uvertex_t* vertex = It_getptr(first);
                // 把 epw list 都换成 epv 的 list;
                ((udg_kruskal_explor_t*)(vertex->exploring))->group_iterator = epv_list_it;
            }
            // 把两个合并起来
            CN_merge(*epv_list, *epw_list);
            Tv rdata;
            CN_remove(group_list, epw_list_it, &rdata);
            // 把这个列 set 干掉。
            //free(t2p(rdata));
            _free_list_in_group_list(rdata);
            CN_add(list, p2t(edge));
        }
    }
    
    List_(group_list, _free_list_in_group_list);
    return 0;
}

int ugrp_calculate_mst_prim(UDGraph* udgraph, uvertex_t* start)
{
    // 初始化
    for (It first = CN_first(udgraph->uvertexs); !It_equal(first, CN_tail(udgraph->uvertexs)); first = It_next(first)) {
        uvertex_t* v = It_getptr(first);
        udg_prim_explor_t* explor = v->exploring;
        explor->key = PRIM_MAX;
        explor->pi  = NULL;
    }


}