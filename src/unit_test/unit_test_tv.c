/*
 * @Author: your name
 * @Date: 2020-10-13 21:41:25
 * @LastEditTime: 2020-10-14 13:10:37
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/unit_test_tv.c
 */
#include <stdio.h>
#include <CUnit/Basic.h>
#include "container/Tv.h"
#include "unit_test.h"

static int  suite_success_init (void) 
{
    printf("\n tv suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("\n tv suite success clean\n");
}

static void test_tv_value_equal (void) 
{
    Tv v1 = i2t(19); 
    Tv v2 = i2t(19);
    CU_ASSERT(tv_equl(v1, v2));

    v1 = i2t(9888739);
    v2 = i2t(9888739);
    CU_ASSERT(tv_equl(v1, v2));

    v1 = f2t(4455332.987);
    v2 = f2t(4455332.987);
    CU_ASSERT(tv_equl(v1, v2));

}

static void test_tv_value_cmp (void) 
{
    Tv v1 = i2t(19);
    Tv v2 = i2t(23);
    CU_ASSERT(cmpi(v1, v2) < 0);

    v1 = i2t(19);
    v2 = i2t(13);
    CU_ASSERT(cmpi(v1, v2) > 0);

    v1 = i2t(19);
    v2 = i2t(19);
    CU_ASSERT(cmpi(v1, v2) == 0);
}

int do_tv_test(void) 
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("The Tv Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test tv equal", test_tv_value_equal) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test tv cmp", test_tv_value_cmp) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    
}