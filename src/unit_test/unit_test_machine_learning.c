/*
 * @Author: your name
 * @Date: 2021-03-31 22:05:01
 * @LastEditTime: 2021-03-31 22:10:42
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/unit_test_machine_learnling.c
 */
#include <stdio.h>
#include <CUnit/Basic.h>

#define PRINTF_DOUBLES(x) printf("%lf ", (x));

static int  suite_success_init (void) 
{
    printf("\nmachine learning suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("\nmachine learning suite success clean\n");
}

static void test_linear_regression(void) 
{
    
}

int do_macine_learning_test (void) 
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("Test Machine Learning Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test linear regressing", test_linear_regression) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
}