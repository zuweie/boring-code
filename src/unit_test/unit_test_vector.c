/*
 * @Author: your name
 * @Date: 2020-10-12 23:35:44
 * @LastEditTime: 2021-10-26 15:58:10
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/vetcor_test.c
 */

#include <stdio.h>
#include <CUnit/Basic.h>
#include "container/cn.h"
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

    CN vector = CN_create(VECTOR, int_t);

    for (int i=0; i<VECTOR_SIZE; ++i) {
        int v = tsd_get_int(i);
        CN_add(vector, v);
    }
    CU_ASSERT_TRUE(VECTOR_SIZE==CN_size(vector));
    
    CN_finalize(vector, NULL);
}

static void test_vector_insert(void) 
{
    const int VECTOR_SIZE = 10;
    
    CN vector = CN_create(VECTOR, fl_t);

    for (int i=0; i<VECTOR_SIZE; ++i) {
        float v = tsd_get_float(i);
        CN_add(&vector, v);
    }

    float find = tsd_get_float(5);
    CU_ASSERT_TRUE(!It_equal(CN_find(vector, find), CN_tail(vector)));

    find = tsd_get_float(11);
    CU_ASSERT_FALSE(It_equal(CN_find(vector, find), CN_tail(vector)));
   
    CN_finalize(vector, NULL);
}

static void test_vector_remove(void) 
{
    const int VECTOR_SIZE = 10;
    CN vector = CN_create(VECTOR, fl_t);

    for (int i=0; i<VECTOR_SIZE; ++i) {
       float v = tsd_get_float(i);
       CN_add(vector, v);
    }

    float target = tsd_get_float(5);
    float ret;
    T_def* float_def = T_def_get(fl_t);
    It remove = CN_find(vector, target);
    CU_ASSERT_TRUE(CN_remove_at(vector, remove, &ret) == 0);
    CU_ASSERT_TRUE(float_def->ty_cmp(&remove, &ret) == 0);
    CU_ASSERT_FALSE(CN_has(vector, target));
    CN_finalize(vector, NULL);
// --------------------------------------------------------------
    CN vector2 = CN_create(VECTOR, str_t);
    T_def* str_def = T_def_get(str_t);

    for (int i=0; i<VECTOR_SIZE; ++i) {
        char* v = tsd_get_str(i);
        CN_add(vector2, v);
    }
    char* target2 = tsd_get_str(5);
    char* ret2;
    It remove2 = CN_find(vector2, target);
    CU_ASSERT_TRUE(CN_remove_at(vector2, remove2, &ret2) == 0);
    CU_ASSERT_TRUE(str_def->ty_cmp(&target2, &ret2));
    CU_ASSERT_FALSE(CN_has(vector2, target2));
    
    CN_finalize(&vector2, NULL);
}

void test_vector_sort(void)
{
    CN vector = CN_create(VECTOR, fl_t);
    // printf("\n\nunsort \n\n");
    // for (int i=0; i<VECTOR_SIZE; ++i){
    //     printf("%f ", test_data_float[i]);
    // }
    // printf("\n\n");
    for (int i=0; i<100; ++i) {
        CN_add(vector, tsd_get_float(i));
    }
    // 从小到大的排序
    CN_sort(&vector, NULL);

    // printf(" asc sort \n\n");
    // CN_travel(vector, PRINTF_IT_ON_FLOAT);
    // printf("\n\n");
    T_def* flt_def = T_def_get(fl_t);

    for(It first=CN_first(vector); !It_equal(first, CN_last(vector)); It_next(first)){
        It next = first;
        It_next(next);
        CU_ASSERT_TRUE(flt_def->ty_cmp(It_refer(first), It_refer(next)) < 0);
    }

    // 从大到小的排序
    //printf(" desc sort \n\n");
    //CN_travel(vector, PRINTF_IT_ON_FLOAT);
    // printf("\n\n");
    // for(It first=CN_first(vector); !It_equal(first, CN_last(vector)); first=It_next(first)){
    //     Tv v1 = It_dref(first);
    //     Tv v2 = It_dref(It_next(first));
    //     CU_ASSERT_TRUE(Tv_cmpf(v1, v2) > 0);
    // }
    CN_finalize(vector,NULL);
//---------------------------------------------------------------------

    // 测试 string 的排序。
    CN vector2 = CN_create(VECTOR, str_t);
    // 灌入数据
    T_def* str_def = T_def_get(str_t);
    // printf("\n\nunsort \n\n");
    // for (int i=0; i<TEST_DATA_STRING_SIZE; ++i){
    //     printf("%s ", test_data_string[i]);
    // }
    // printf("\n\n");
    for (int i=0; i<TEST_DATA_STR_SIZE; ++i) {
        CN_add(vector2, tsd_get_str(i));
    }

    CN_sort(vector2, NULL);

    for(It first=CN_first(vector2); !It_equal(first, CN_last(vector2)); It_next(first)){
        It next = first;
        It_next(next);
        CU_ASSERT_TRUE( str_def->ty_cmp(It_refer(first), It_refer(next)) < 0);
    }
}

static void test_vector_unique(void) 
{
    CN vector = CN_create(VECTOR, int_t);

    CN_add(vector, 11);
    CN_add(vector, 11);
    CN_add(vector, 12);
    CN_add(vector, 32);
    CN_add(vector, 44);
    CN_add(vector, 55);
    CN_add(vector, 55);

    CN_to_unique(vector);
    
    for(It first = CN_first(vector); !It_equal(first, CN_last(vector)); It_next(first)){

        It next = first;
        It_next(next);
        int v1 = It_int(first);
        int v2 = It_int(next);
        CU_ASSERT_FALSE( v1 == v2);
    }

    CN_finalize(vector, NULL);
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

    if (NULL == CU_add_test(pSuite, "test vector unique", test_vector_unique) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
}