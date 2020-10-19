/*
 * @Author: your name
 * @Date: 2020-10-19 13:17:51
 * @LastEditTime: 2020-10-19 13:31:04
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/test_rb_tree.c
 */
#include <stdio.h>
#include <CUnit/Basic.h>
#include "container/RbTree.h"
#include "test_data.h"
static int  suite_success_init (void) 
{
    printf("\n Rbtree suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("\n Rbtree suite success clean\n");
}

static void test_rb_tree_set (void) 
{
    
    CU_ASSERT_TRUE(1);
}

static void test_rb_tree_get (void) 
{
    CU_ASSERT_TRUE(1);
}


int do_rb_tree_test(void) 
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("Test Rbtree Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }
        
    if (NULL == CU_add_test(pSuite, "test rbtree set", test_rb_tree_set) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test rbtree get", test_rb_tree_set) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
}