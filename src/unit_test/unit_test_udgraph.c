/*
 * @Author: your name
 * @Date: 2020-12-01 08:42:35
 * @LastEditTime: 2020-12-01 10:48:45
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/unit_test_udgraph.c
 */
#include <stdio.h>
#include <CUnit/Basic.h>
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

static void test_udgraph_vertex(void) 
{
    
}

int do_udgraph_test (void) 
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("Test UDGraph Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }    
}