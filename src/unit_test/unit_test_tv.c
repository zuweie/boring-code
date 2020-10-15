/*
 * @Author: your name
 * @Date: 2020-10-13 21:41:25
 * @LastEditTime: 2020-10-16 07:32:37
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/unit_test_tv.c
 */
#include <stdio.h>
#include <CUnit/Basic.h>
#include "container/Tv.h"
#include "unit_test.h"
#include "test_data.h"

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
    Tv v1 = getTSi(19);
    Tv v2 = getTSi(19);
    CU_ASSERT_TRUE(Tv_equl(v1, v2)==0);

    v1 = getTSi(13);
    v2 = getTSi(76);
    CU_ASSERT_FALSE(Tv_equl(v1, v2)==0);

    v1 = getTSf(22);
    v2 = getTSf(22);
    CU_ASSERT_TRUE(Tv_equl(v1, v2)==0);

    v1 = getTSf(24);
    v2 = getTSf(46);
    CU_ASSERT_FALSE(Tv_equl(v1, v2)==0);

}

static void test_tv_value_cmp (void) 
{
    Tv v1, v2;
    int i1 = geti(19);
    int i2 = geti(23);
    i1 < i2 ? ({v1 = getTSi(19); v2 = getTSi(23);}) : ({v1 = getTSi(23); v2 = getTSi(19);});
    CU_ASSERT(Tv_cmpi(v1, v2) < 0);
    CU_ASSERT(Tv_cmpi(v2, v1) > 0);
    

    v1 = getTSi(19);
    v2 = getTSi(19);
    CU_ASSERT(Tv_cmpi(v1, v2) == 0);

    float f1 = getf(39);
    float f2 = getf(78);

    f1 < f2 ? ({v1 = getTSf(39); v2 = getTSf(78);}) : ({v1 = getTSf(78); v2 = getTSf(39);});

    CU_ASSERT(Tv_cmpf(v1, v2) < 0);
    CU_ASSERT(Tv_cmpf(v2, v1) > 0);

    v1 = getTSf(51);
    v2 = getTSf(51);
    CU_ASSERT_TRUE(Tv_cmpf(v1,v2)==0);
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