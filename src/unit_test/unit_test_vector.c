/*
 * @Author: your name
 * @Date: 2020-10-12 23:35:44
 * @LastEditTime: 2020-10-16 10:09:06
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/vetcor_test.c
 */

#include <stdio.h>
#include <CUnit/Basic.h>
#include "container/Vector.h"
#include "unit_test.h"
#include "test_data.h"


static int  suite_success_init (void) 
{
    printf("\nvector suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("\nvector suite success clean\n");
}

static void test_vector_size(void)
{
    const int VECTOR_SIZE = 10;
    Vector vector;
    Vector_init(vector, NULL);
    for (int i=0; i<VECTOR_SIZE; ++i) {
        Tv v = getTSi(i);
        CN_add_tail(vector, v);
    }
    CU_ASSERT_TRUE(VECTOR_SIZE==CN_size(vector));
    Vector_uninit(vector, NULL);
}

static void test_vector_insert(void) 
{
    const int VECTOR_SIZE = 10;
    Vector vector;
    Vector_init(vector, NULL);
    for (int i=0; i<VECTOR_SIZE; ++i) {
       Tv v = getTSf(i);
       CN_add_tail(vector, v);
    }

    Tv find = getTSf(5);
    CU_ASSERT_TRUE(CN_has(vector, find));

    find = getTSf(11);
    CU_ASSERT_FALSE(CN_has(vector, find));
   
    Vector_uninit(vector, NULL);
}

static void test_vector_remove() 
{
    const int VECTOR_SIZE = 10;
    Vector vector;
    Vector_init(vector, NULL);
    for (int i=0; i<VECTOR_SIZE; ++i) {
       Tv v = getTSf(i);
       CN_add_tail(vector, v);
    }

    Tv target = getTSf(5);
    Tv ret;
    CU_ASSERT_TRUE(CN_rm_target(vector, target, &ret) == 0);
    CU_ASSERT_TRUE(Tv_equl(target, ret)==0);
    CU_ASSERT_FALSE(CN_has(vector, target));
    Vector_uninit(vector, NULL);

    Vector vector2;
    Vector_init(vector2, CMP_STR);
    for (int i=0; i<VECTOR_SIZE; ++i) {
        Tv v = getTSs(i);
        CN_add_tail(vector2, v);
    }
    target = getTSs(5);
    ret;
    CU_ASSERT_TRUE(CN_rm_target(vector2, target, &ret) == 0);
    CU_ASSERT_TRUE(Tv_equl(target, ret)==0);
    CU_ASSERT_FALSE(CN_has(vector2, target));
    Vector_uninit(vector2, NULL);
}

void test_vector_sort()
{
    Vector vector;
    Vector_init(vector, NULL);
    const VECTOR_SIZE = 10;
    extern float test_data_float[TEST_DATA_SIZE];
    // printf("\n\nunsort \n\n");
    // for (int i=0; i<VECTOR_SIZE; ++i){
    //     printf("%f ", test_data_float[i]);
    // }
    // printf("\n\n");
    Arr_to_cn(test_data_float, VECTOR_SIZE, f2t, vector);

    // 从小到大的排序
    CN_sort(vector, CMP_FLT);
    // printf(" asc sort \n\n");
    // CN_travel(vector, PRINTF_IT_ON_FLOAT);
    // printf("\n\n");

    for(It first=CN_first(vector); !It_equal(first, CN_last(vector)); first=It_next(first)){
        Tv v1 = It_dref(first);
        Tv v2 = It_dref(It_next(first));
        CU_ASSERT_TRUE(Tv_cmpf(v1, v2) < 0);
    }
    // 从大到小的排序
    CN_sort(vector, INCMP_FLT);
    //printf(" desc sort \n\n");
    //CN_travel(vector, PRINTF_IT_ON_FLOAT);
    // printf("\n\n");
    // for(It first=CN_first(vector); !It_equal(first, CN_last(vector)); first=It_next(first)){
    //     Tv v1 = It_dref(first);
    //     Tv v2 = It_dref(It_next(first));
    //     CU_ASSERT_TRUE(Tv_cmpf(v1, v2) > 0);
    // }
    Vector_uninit(vector, NULL);
    
    // 测试 string 的排序。
    Vector vector2;
    Vector_init(vector2, NULL);
    // 灌入数据

    extern *test_data_string[TEST_DATA_STRING_SIZE];
    // printf("\n\nunsort \n\n");
    // for (int i=0; i<TEST_DATA_STRING_SIZE; ++i){
    //     printf("%s ", test_data_string[i]);
    // }
    // printf("\n\n");
    Arr_to_cn(test_data_string, TEST_DATA_STRING_SIZE, p2t, vector2);
    CN_sort(vector2, CMP_STR);
    for(It first=CN_first(vector2); !It_equal(first, CN_last(vector2)); first=It_next(first)){
        Tv v1 = It_dref(first);
        Tv v2 = It_dref(It_next(first));
        CU_ASSERT_TRUE(CMP_STR(v1, v2) < 0);
    }
    // printf(" asc sort \n\n");
    // CN_travel(vector2, PRINTF_IT_ON_STRING);
    // printf("\n\n");
    
    CN_sort(vector2, INCMP_STR);
    // printf("desc sort \n\n");
    // CN_travel(vector2, PRINTF_IT_ON_STRING);
    // printf("\n\n");
    for(It first=CN_first(vector2); !It_equal(first, CN_last(vector2)); first=It_next(first)){
        Tv v1 = It_dref(first);
        Tv v2 = It_dref(It_next(first));
        CU_ASSERT_TRUE(CMP_STR(v1, v2) > 0);
    }
    
    Vector_uninit(vector2, NULL);
}
int do_vector_test (void) 
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("Test Vector Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if (NULL == CU_add_test(pSuite, "test vector size", test_vector_size) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test vector insert", test_vector_insert) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test vector remove", test_vector_remove) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test vector sort", test_vector_sort) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
}