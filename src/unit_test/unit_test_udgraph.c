/*
 * @Author: your name
 * @Date: 2020-12-01 08:42:35
 * @LastEditTime: 2025-06-09 12:11:53
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/unit_test_udgraph.c
 */
#include <stdio.h>
#include <CUnit/Basic.h>
//#include "cmp_component.h"
#include "graph/graph_research.h"
#include "graph/ud_graph.h"
#include "graph/graph_research.h"
//#include "container/List.h"
#include "container/Cont.h"
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
    UDGraph* graph = UDGraph_create(sizeof(Iter));
    CN list = CN_create(LIST, ptr_t);
    
    UDGraph_add_vertex(graph, 'a');
    UDGraph_add_vertex(graph, 'b');
    UDGraph_add_vertex(graph, 'c');
    UDGraph_add_vertex(graph, 'd');
    UDGraph_add_vertex(graph, 'e');
    UDGraph_add_vertex(graph, 'f');
    UDGraph_add_vertex(graph, 'g');
    UDGraph_add_vertex(graph, 'h');
    UDGraph_add_vertex(graph, 'i');

    CU_ASSERT_FALSE(UDGraph_add_edge(graph, 'a', 'b', 4) == 0);
    CU_ASSERT_FALSE(UDGraph_add_edge(graph, 'b', 'h', 11) == 0);
    CU_ASSERT_FALSE(UDGraph_add_edge(graph, 'a', 'h', 8) == 0);
    CU_ASSERT_FALSE(UDGraph_add_edge(graph, 'b', 'c', 8) == 0);
    CU_ASSERT_FALSE(UDGraph_add_edge(graph, 'c', 'd', 7) == 0);
    CU_ASSERT_FALSE(UDGraph_add_edge(graph, 'd', 'e', 9) == 0);
    CU_ASSERT_FALSE(UDGraph_add_edge(graph, 'e', 'f', 10) == 0);
    CU_ASSERT_FALSE(UDGraph_add_edge(graph, 'd', 'f', 14) == 0);
    CU_ASSERT_FALSE(UDGraph_add_edge(graph, 'c', 'f', 4) == 0);
    CU_ASSERT_FALSE(UDGraph_add_edge(graph, 'c', 'i', 2) == 0);
    CU_ASSERT_FALSE(UDGraph_add_edge(graph, 'i', 'g', 6) == 0);
    CU_ASSERT_FALSE(UDGraph_add_edge(graph, 'f', 'g', 2) == 0);
    CU_ASSERT_FALSE(UDGraph_add_edge(graph, 'h', 'g', 1) == 0);
    CU_ASSERT_FALSE(UDGraph_add_edge(graph, 'i', 'h', 7) == 0);
    
    ugrp_calculate_mst_kruskal(graph, list);
    
    printf("\n");
    int i = 0;
    for (Iter first = CN_first(list); !It_equal(first, CN_tail(list)); first = It_next(first), ++i) {

        uedge_t* edge = It_ptr(first);
        printf("(%d), edge: %c --- %c  weight: %f", i, edge->epv->id, edge->epw->id, edge->weight);
        printf("\n");

    }
    printf("\n");
    
    UDGraph_destroy(graph);
    CN_finalize(list, NULL);
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