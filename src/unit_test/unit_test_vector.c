/*
 * @Author: your name
 * @Date: 2020-10-12 23:35:44
 * @LastEditTime: 2025-06-09 12:09:53
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/vetcor_test.c
 */

#include <stdio.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "container/Cont.h"
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
        CN_add(vector, v);
    }

    float find = tsd_get_float(5);
    CU_ASSERT_TRUE(CN_has(vector, find));

    find = tsd_get_float(11);
    CU_ASSERT_FALSE(CN_has(vector, find));
   
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
    
    Iter remove = CN_find(vector, target);

    CU_ASSERT_TRUE(CN_remove_at(vector, remove, &ret) == 0);

    CU_ASSERT_DOUBLE_EQUAL(target, ret, 0.0001);

    CU_ASSERT_FALSE(CN_has(vector, target));
    CN_finalize(vector, NULL);
// --------------------------------------------------------------
    CN vector2 = CN_create(VECTOR, str_t);

    for (int i=0; i<VECTOR_SIZE; ++i) {
        char* v = tsd_get_str(i);
        CN_add(vector2, v);
    }
    char* target2 = tsd_get_str(5);
    char* ret2;
    Iter remove2 = CN_find(vector2, target2);
    CU_ASSERT_TRUE(CN_remove_at(vector2, remove2, &ret2) == 0);
    CU_ASSERT_STRING_EQUAL(target2, ret2);
    CU_ASSERT_FALSE(CN_has(vector2, target2));
    
    CN_finalize(vector2, NULL);
}

void test_vector_sort(void)
{
    CN vector = CN_create(VECTOR, fl_t);
    for (int i=0; i<10; ++i) {
        CN_add(vector, tsd_get_float(i));
    }
    // 从小到大的排序
    //printf("\n inspect befort sort \n");
    //CN_inspect(vector, PRINTF_T_ON_FLOAT);
    CN_sort(vector, NULL);
    //printf("\n inspect after sort \n");
    //CN_inspect(vector, PRINTF_T_ON_FLOAT);

    for(Iter first=CN_first(vector); !It_equal(first, CN_last(vector)); first = It_next(first)){
        Iter next = first;
        It_next(next);
        float f1 = It_float(first);
        float f2 = It_float(next);
        //printf("f1 %f, f2 %f ", f1, f2);
        CU_ASSERT_TRUE(f1 <= f2);
    }
    CN_finalize(vector,NULL);
//---------------------------------------------------------------------

    // 测试 string 的排序。
    CN vector2 = CN_create(VECTOR, str_t);
    // 灌入数据
    //char* strs[4] = {"klm", "hij", "efg", "abc"};
    for (int i=0; i<TEST_DATA_STR_SIZE; ++i) {
        CN_add(vector2, tsd_get_str(i));
    }

    //CN_inspect(vector2, PRINTF_T_ON_STRING);
    CN_sort(vector2, NULL);
    //CN_inspect(vector2, PRINTF_T_ON_STRING);

    for(Iter first=CN_first(vector2); !It_equal(first, CN_last(vector2)); first=It_next(first)){
        Iter next = first;
        It_next(next);
        char* s1 = It_str(first);
        char *s2 = It_str(next);
        CU_ASSERT_TRUE( strcmp(s1, s2) <=0 );
    }
    CN_finalize(vector2, NULL);
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
    
    for(Iter first = CN_first(vector); !It_equal(first, CN_last(vector)); first=It_next(first)){

        Iter next = It_next(first);
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
    return 0;
}