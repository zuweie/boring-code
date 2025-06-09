/*
 * @Author: your name
 * @Date: 2020-10-13 21:41:25
 * @LastEditTime: 2025-06-09 12:13:53
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/unit_test_tv.c
 */
#include <stdio.h>
#include <CUnit/Basic.h>
#include "container/Typ.h"
#include "base/type_value/__type_value.h"
#include "unit_test.h"
#include "test_data.h"

static int  suite_success_init (void) 
{
    printf("\n T suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("\n T suite success clean\n");
}

static void test_t_cpy (void) 
{
    float f1 = 0.245f;
    float f2;
    type_value_cpy(&f2, &f1, sizeof(float));
    CU_ASSERT_DOUBLE_EQUAL(f1, f2, 0.0001);
}

static void test_t_swap (void) 
{
    double f1 = 3.45678f;
    double f2 = 9.87654f;
    type_value_swap(&f1, &f2, sizeof(double));
    CU_ASSERT_DOUBLE_EQUAL(f1, 9.87654f, 0.00001);
    CU_ASSERT_DOUBLE_EQUAL(f2, 3.45678f, 0.00001);
}


int do_t_test(void) 
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("The Tv Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test tv equal", test_t_cpy) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test tv cmp", test_t_swap) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }


}