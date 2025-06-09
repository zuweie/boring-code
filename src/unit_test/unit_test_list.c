/*
 * @Author: your name
 * @Date: 2020-10-12 23:35:44
 * @LastEditTime: 2025-06-09 12:10:11
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/vetcor_test.c
 */

#include <stdio.h>
#include <CUnit/Basic.h>
#include "container/Cont.h"
#include "unit_test.h"
#include "test_data.h"

static int  suite_success_init (void) 
{
    printf("\nList suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("\nList suite success clean\n");
}

static void test_list_size(void)
{
    const int LIST_SIZE = 10;
    CN list = CN_create(LIST, int_t);

    for (int i=0; i<LIST_SIZE; ++i) {
        CN_add(list, tsd_get_int(i));
    }
    CU_ASSERT_TRUE(LIST_SIZE==CN_size(list));
    CN_finalize(list, NULL);
}

static void test_list_insert(void) 
{
    const int LIST_SIZE = 10;
    CN list = CN_create(LIST, fl_t);

    for (int i=0; i<LIST_SIZE; ++i) {
       //Tv v = getTSf(i);
       CN_add(list, tsd_get_float(i));
    }

    float find = tsd_get_float(5);
    CU_ASSERT_TRUE(CN_has(list, find));

    find = tsd_get_float(11);
    CU_ASSERT_FALSE(CN_has(list, find));
   
    CN_finalize(list, NULL);
}

static void test_list_remove() 
{
    const int LIST_SIZE = 10;
    CN list = CN_create(LIST, fl_t);
    
    for (int i=0; i<LIST_SIZE; ++i) {
       CN_add(list, tsd_get_float(i));
    }

    float target = tsd_get_float(5);
    float ret;
    Iter remove = CN_find(list, target);

    CU_ASSERT_TRUE(CN_remove_at(list, remove, &ret) == err_ok);
    CU_ASSERT_DOUBLE_EQUAL(target, ret, 0.0001);
    CU_ASSERT_FALSE(CN_has(list, target));
    CN_finalize(list, NULL);
// ------------------------------------------------------
}

void test_list_sort()
{
    CN list = CN_create(LIST, fl_t);
    const LIST_SIZE = 100;
    //T_def* flt_def = T_def_get(fl_t);

    for (int i=0; i<LIST_SIZE; ++i) {
        CN_add(list, tsd_get_float(i));
    }        
    // 从小到大的排序    
    CN_sort(list, NULL);

    for(Iter first=CN_first(list); !It_equal(first, CN_last(list)); first=It_next(first)){
        Iter next = It_next(first);
        float f1 = It_float(first);
        float f2 = It_float(next);
        CU_ASSERT_TRUE(f1 <= f2);
    }
}

static void test_list_unique(void) 
{
    CN list = CN_create(LIST, int_t);

    CN_add(list, 11);
    CN_add(list, 11);
    CN_add(list, 22);
    CN_add(list, 32);
    CN_add(list, 44);
    CN_add(list, 55);
    CN_add(list, 55);
    CN_add(list, 11);
    CN_add(list, 11);
    CN_add(list, 55);
    CN_add(list, 44);
    
    CN_to_unique(list);

    for(Iter first = CN_first(list); !It_equal(first, CN_last(list)); first=It_next(first)){
        
        Iter next = It_next(first);
        int i1 = It_int(first);
        int i2 = It_int(next);
        
        CU_ASSERT_FALSE(i1 == i2);
    }

    CN_finalize(list, NULL);
}


int do_list_test (void) 
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("Test List Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if (NULL == CU_add_test(pSuite, "test List size", test_list_size) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test List insert", test_list_insert) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test List remove", test_list_remove) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test List sort", test_list_sort) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test List unique", test_list_unique) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
}