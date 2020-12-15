/*
 * @Author: your name
 * @Date: 2020-12-01 08:42:35
 * @LastEditTime: 2020-12-14 07:40:14
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/unit_test_udgraph.c
 */
#include <stdio.h>
#include <CUnit/Basic.h>
#include "cmp_component.h"
#include "graph/graph_research.h"
#include "graph/ud_graph.h"
#include "graph/graph_research.h"
#include "container/List.h"
#include "unit_test.h"
#include "test_data.h"

static int  suite_success_init (void) 
{
    printf("\nUDGraph research suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("\nUDGraph research suite success clean\n");
}

static void test_udgraph_mst_kruskal(void) 
{
    UDGraph* graph = UDGraph_create(udgraph_match_vertex, udgraph_match_edge_2p, sizeof(It));
    List list = _List(NULL);
    
    UDGraph_add_vertex(graph, i2t('a'));
    UDGraph_add_vertex(graph, i2t('b'));
    UDGraph_add_vertex(graph, i2t('c'));
    UDGraph_add_vertex(graph, i2t('d'));
    UDGraph_add_vertex(graph, i2t('e'));
    UDGraph_add_vertex(graph, i2t('f'));
    UDGraph_add_vertex(graph, i2t('g'));
    UDGraph_add_vertex(graph, i2t('h'));
    UDGraph_add_vertex(graph, i2t('i'));

    CU_ASSERT_TRUE(UDGraph_add_edge(graph, i2t('a'), i2t('b'), 4) == 0);
    CU_ASSERT_TRUE(UDGraph_add_edge(graph, i2t('b'), i2t('h'), 11) == 0);
    CU_ASSERT_TRUE(UDGraph_add_edge(graph, i2t('a'), i2t('h'), 8) == 0);
    CU_ASSERT_TRUE(UDGraph_add_edge(graph, i2t('b'), i2t('c'), 8) == 0);
    CU_ASSERT_TRUE(UDGraph_add_edge(graph, i2t('c'), i2t('d'), 7) == 0);
    CU_ASSERT_TRUE(UDGraph_add_edge(graph, i2t('d'), i2t('e'), 9) == 0);
    CU_ASSERT_TRUE(UDGraph_add_edge(graph, i2t('e'), i2t('f'), 10) == 0);
    CU_ASSERT_TRUE(UDGraph_add_edge(graph, i2t('d'), i2t('f'), 14) == 0);
    CU_ASSERT_TRUE(UDGraph_add_edge(graph, i2t('c'), i2t('f'), 4) == 0);
    CU_ASSERT_TRUE(UDGraph_add_edge(graph, i2t('c'), i2t('i'), 2) == 0);
    CU_ASSERT_TRUE(UDGraph_add_edge(graph, i2t('i'), i2t('g'), 6) == 0);
    CU_ASSERT_TRUE(UDGraph_add_edge(graph, i2t('f'), i2t('g'), 2) == 0);
    CU_ASSERT_TRUE(UDGraph_add_edge(graph, i2t('h'), i2t('g'), 1) == 0);
    CU_ASSERT_TRUE(UDGraph_add_edge(graph, i2t('i'), i2t('h'), 7) == 0);
    
    ugrp_calculate_mst_kruskal(graph, list);
    
    printf("\n");
    int i = 0;
    for (It first = CN_first(list); !It_equal(first, CN_tail(list)); first = It_next(first), ++i) {

        uedge_t* edge = It_getptr(first);
        printf("(%d), edge: %c --- %c  weight: %f", i, t2i(edge->epv->id), t2i(edge->epw->id), edge->weight);
        printf("\n");

    }
    printf("\n");
    
    UDGraph_destroy(graph);
    List_(list, NULL);
}



int do_udgraph_test (void) 
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("Test UDGraph Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }   

    if (NULL == CU_add_test(pSuite, "test mst kruskal", test_udgraph_mst_kruskal) ) {
        CU_cleanup_registry();
        return CU_get_error();
    } 

}