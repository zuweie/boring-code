/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-20 09:34:56
 * @LastEditTime: 2020-06-14 01:22:23
 * @LastEditors: Please set LastEditors
 */
#include "graph_search.h"
#include "container/queue.h"
#include "container/tv.h"
#include "container/it.h"
static void _init_bfs_exploring(it pos) 
{
    //bfs_node_t* pn = allocate(g_pool(0), sizeof(bfs_node_t));
    bfs_explor_t* pn = (bfs_explor_t*)malloc(sizeof(bfs_explor_t));
    pn->color = _grp_whtie;
    pn->distance = -1;
    pn->pi = NULL;

    vertex_t* vertex = It_getptr(pos);
    vertex->exploring = pn;
}

static void _init_dfs_exploring(it pos) 
{
    //dfs_node_t* pn = allocate(g_pool(0), sizeof(dfs_node_t));
    dfs_explor_t* pn = (dfs_explor_t*) malloc (sizeof(dfs_explor_t));
    pn->color = _grp_whtie;
    pn->pi = NULL;
    pn->d_time = -1;
    pn->f_time = -1;

    vertex_t* vertex = It_getptr(pos);
    vertex->exploring = pn;
}

static void _free_exploring (it pos) 
{
    vertex_t* vertex = It_getptr(pos);
    free (vertex->exploring);
    vertex->exploring = NULL;
}

// 广度优先算法
int grp_bfs(Graph* graph, vertex_t* start) {

    CN_travel(&graph->vertexes, _init_bfs_exploring);
    // 2 做广度优先遍历
    // 

    bfs_explor_t* pbfs = (bfs_explor_t*)start->exploring;
    pbfs->color = _grp_gray;
    pbfs->distance = 0;
    pbfs->pi = NULL;

    Queue queue;
    Queue_init(&queue, NULL);
    Queue_offer(&queue, p2t(start));
    tv rdata;
    
    while(Queue_poll(&queue, &rdata) != -1) {

        vertex_t* pu = t2p(rdata);
        bfs_explor_t* pubfs = (bfs_explor_t*) pu->exploring;

        // 遍历节点的邻居表。
        for(it first = CN_first(&pu->edges); 
            !It_equal(first, CN_tail(&pu->edges)); 
            first=It_next(first)) {

            edge_t* pv = It_getptr(first);
            bfs_explor_t* pvbfs  = (bfs_explor_t*) (pv->to->exploring);

            if (pvbfs->color == _grp_whtie) {
                pvbfs->color = _grp_gray;
                pvbfs->distance = pvbfs->distance + 1;
                pvbfs->pi = pu;
                Queue_offer(&queue, p2t(pv->to));
            }
        }
        pubfs->color = _grp_black;
    }
    Queue_free(&queue);
    return 0;
}

static int _dfs_visit(vertex_t* pu, int* time) 
{
    dfs_explor_t* pudfs = (dfs_explor_t*) pu->exploring;
    pudfs->color = _grp_gray;
    pudfs->d_time = *time + 1;
    // 访问邻接表
    for(it first=CN_first(&pu->edges); !It_equal(first, CN_tail(&pu->edges)); first=It_next(first)) {
        edge_t* pv   = It_getptr(first);
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
int grp_dfs(Graph* graph) 
{
    int time = 0;
    CN_travel(&graph->vertexes, _init_dfs_exploring);

    for(it first=CN_first(&graph->vertexes); 
        !It_equal(first, CN_tail(&graph->vertexes)); 
        first=It_next(first)) {

        vertex_t*   pu = It_getptr(first);
        dfs_explor_t* pudfs = (dfs_explor_t*)pu->exploring;
        if (pudfs->color == _grp_whtie) {
            _dfs_visit(pu, &time);
        }
        
    }
    return 0;
}

int grp_bfs_path(Graph* graph, vertex_t* start, vertex_t* desc, LinkArray* arr) 
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

void grp_cleanup_exploring_info(Graph* graph) 
{
    CN_travel(&graph->vertexes, _free_exploring);
}