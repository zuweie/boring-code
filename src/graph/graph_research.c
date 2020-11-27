/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-20 09:34:56
 * @LastEditTime: 2020-11-27 22:56:15
 * @LastEditors: Please set LastEditors
 */
#include "graph_research.h"
#include "container/Queue.h"
#include "container/Tv.h"
#include "container/It.h"

static int _topological_sort_cmp(Tv t1, Tv t2) 
{
    dfs_explor_t* v1 = ((vertex_t*)t2p(t1))->exploring;
    dfs_explor_t* v2 = ((vertex_t*)t2p(t2))->exploring;

    return INCMP_INT(i2t(v1->f_time), i2t(v2->f_time));
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
