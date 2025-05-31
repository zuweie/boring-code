/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-05-31 22:44:25
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-05-31 23:21:58
 * @FilePath: /boring-code/src/unit_test/unit_test_dl_cg2.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
// 计算图2 测试用例

#include <stdio.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "unit_test.h"
#include "deep_learning/compute_graph2/cg_list.h"
#include "deep_learning/compute_graph2/cg_hash.h"
#include "deep_learning/compute_graph2/cg_graph.h"
#include "deep_learning/compute_graph2/cg_tensor.h"
#include "deep_learning/compute_graph2/cg.h"

static int  suite_success_init (void) 
{
    printf("\ncg2 suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("\ncg2 suite success clean\n");
}

static void cg2_testcase(void) 
{
    printf("cg2 test case\n");
}

static void cg_list_testcase(void) 
{

}

static void cg_hash_testcase(void)
{

}

static void cg_allocator_testcase(void) 
{

}

static void cg_tensor_testcase(void)
{
    
}

static void cg_compute_graph_testcase(void)
{
    
}

int do_cg2_test (void) 
{

    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("The cg2 Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test lcs ..\n", cg2_testcase) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    return 0;
}