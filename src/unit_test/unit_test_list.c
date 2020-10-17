/*
 * @Author: your name
 * @Date: 2020-10-12 23:35:44
 * @LastEditTime: 2020-10-17 09:08:28
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/vetcor_test.c
 */

#include <stdio.h>
#include <CUnit/Basic.h>
#include "container/List.h"
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
    List list;
    List_init(list, NULL);
    for (int i=0; i<LIST_SIZE; ++i) {
        Tv v = getTSi(i);
        CN_add_tail(list, v);
    }
    CU_ASSERT_TRUE(LIST_SIZE==CN_size(list));
    List_uninit(list, NULL);
}

static void test_list_insert(void) 
{
    const int LIST_SIZE = 10;
    List list;
    List_init(list, NULL);
    for (int i=0; i<LIST_SIZE; ++i) {
       Tv v = getTSf(i);
       CN_add_tail(list, v);
    }

    Tv find = getTSf(5);
    CU_ASSERT_TRUE(CN_has(list, find));

    find = getTSf(11);
    CU_ASSERT_FALSE(CN_has(list, find));
   
    List_uninit(list, NULL);
}

static void test_list_remove() 
{
    const int LIST_SIZE = 10;
    List list;
    List_init(list, NULL);
    for (int i=0; i<LIST_SIZE; ++i) {
       Tv v = getTSf(i);
       CN_add_tail(list, v);
    }

    Tv target = getTSf(5);
    Tv ret;
    CU_ASSERT_TRUE(CN_rm_target(list, target, &ret) == 0);
    CU_ASSERT_TRUE(Tv_equl(target, ret)==0);
    CU_ASSERT_FALSE(CN_has(list, target));
    List_uninit(list, NULL);

    List list2;
    List_init(list2, CMP_STR);
    for (int i=0; i<LIST_SIZE; ++i) {
        Tv v = getTSs(i);
        CN_add_tail(list2, v);
    }
    target = getTSs(5);
    ret;
    CU_ASSERT_TRUE(CN_rm_target(list2, target, &ret) == 0);
    CU_ASSERT_TRUE(Tv_equl(target, ret)==0);
    CU_ASSERT_FALSE(CN_has(list2, target));
    List_uninit(list2, NULL);
}

void test_list_sort()
{
    List list;
    List_init(list, NULL);
    const VECTOR_SIZE = 10;
    extern float test_data_float[TEST_DATA_SIZE];
    
    Arr_to_cn(test_data_float, VECTOR_SIZE, f2t, list);
    // 从小到大的排序
    CN_sort(list, CMP_FLT);

    for(It first=CN_first(list); !It_equal(first, CN_last(list)); first=It_next(first)){
        Tv v1 = It_dref(first);
        Tv v2 = It_dref(It_next(first));
        CU_ASSERT_TRUE(Tv_cmpf(v1, v2) < 0);
    }
    // 从大到小的排序
    CN_sort(list, INCMP_FLT);
    //printf(" desc sort \n\n");
    //CN_travel(list, PRINTF_IT_ON_FLOAT);
    // printf("\n\n");
    // for(It first=CN_first(list); !It_equal(first, CN_last(list)); first=It_next(first)){
    //     Tv v1 = It_dref(first);
    //     Tv v2 = It_dref(It_next(first));
    //     CU_ASSERT_TRUE(Tv_cmpf(v1, v2) > 0);
    // }
    List_uninit(list, NULL);
    
    // 测试 string 的排序。
    List list2;
    List_init(list2, NULL);
    // 灌入数据

    extern char* test_data_string[];
    // printf("\n\nunsort \n\n");
    // for (int i=0; i<TEST_DATA_STRING_SIZE; ++i){
    //     printf("%s ", test_data_string[i]);
    // }
    // printf("\n\n");
    Arr_to_cn(test_data_string, TEST_DATA_STR_SIZE, p2t, list2);
    CN_sort(list2, CMP_STR);
    for(It first=CN_first(list2); !It_equal(first, CN_last(list2)); first=It_next(first)){
        Tv v1 = It_dref(first);
        Tv v2 = It_dref(It_next(first));
        CU_ASSERT_TRUE(CMP_STR(v1, v2) <= 0);
    }
    // printf(" asc sort \n\n");
    // CN_travel(list2, PRINTF_IT_ON_STRING);
    // printf("\n\n");
    
    CN_sort(list2, INCMP_STR);
    // printf("desc sort \n\n");
    // CN_travel(list2, PRINTF_IT_ON_STRING);
    // printf("\n\n");
    for(It first=CN_first(list2); !It_equal(first, CN_last(list2)); first=It_next(first)){
        Tv v1 = It_dref(first);
        Tv v2 = It_dref(It_next(first));
        CU_ASSERT_TRUE(CMP_STR(v1, v2) >= 0);
    }
    
    List_uninit(list2, NULL);
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
}