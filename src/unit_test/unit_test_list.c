/*
 * @Author: your name
 * @Date: 2020-10-12 23:35:44
 * @LastEditTime: 2020-11-11 10:44:24
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
    List list = _List(NULL);

    for (int i=0; i<LIST_SIZE; ++i) {
        Tv v = getTSi(i);
        CN_add_tail(list, v);
    }
    CU_ASSERT_TRUE(LIST_SIZE==CN_size(list));
    List_(list, NULL);
}

static void test_list_insert(void) 
{
    const int LIST_SIZE = 10;
    List list = _List(EQUL);

    for (int i=0; i<LIST_SIZE; ++i) {
       Tv v = getTSf(i);
       CN_add_tail(list, v);
    }

    Tv find = getTSf(5);
    CU_ASSERT_TRUE(CN_has(list, find));

    find = getTSf(11);
    CU_ASSERT_FALSE(CN_has(list, find));
   
    List_(list, NULL);
}

static void test_list_remove() 
{
    const int LIST_SIZE = 10;
    List list = _List(EQUL);
    
    for (int i=0; i<LIST_SIZE; ++i) {
       Tv v = getTSf(i);
       CN_add_tail(list, v);
    }

    Tv target = getTSf(5);
    Tv ret;
    CU_ASSERT_TRUE(CN_rm_target(list, target, &ret) == 0);
    CU_ASSERT_TRUE(Tv_equl(target, ret)==0);
    CU_ASSERT_FALSE(CN_has(list, target));
    List_(list, NULL);

    List list2 = _List(CMP_STR);
    for (int i=0; i<LIST_SIZE; ++i) {
        Tv v = getTSs(i);
        CN_add_tail(list2, v);
    }
    target = getTSs(5);
    ret;
    CU_ASSERT_TRUE(CN_rm_target(list2, target, &ret) == 0);
    CU_ASSERT_TRUE(Tv_equl(target, ret)==0);
    CU_ASSERT_FALSE(CN_has(list2, target));
    List_(list2, NULL);
}

void test_list_sort()
{
    List list = _List(EQUL);
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
    List_(list, NULL);
    
    // 测试 string 的排序。
    List list2 = _List(NULL);
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
    
    List_(list2, NULL);
}

static void test_list_unique(void) 
{
    List list = _List(EQUL);

    CN_add(list, i2t(11));
    CN_add(list, i2t(11));
    CN_add(list, i2t(22));
    CN_add(list, i2t(32));
    CN_add(list, i2t(44));
    CN_add(list, i2t(55));
    CN_add(list, i2t(55));
    CN_add(list, i2t(11));
    CN_add(list, i2t(11));
    CN_add(list, i2t(55));
    CN_add(list, i2t(44));
    
    // printf ("\n\n before unique\n");
    // CN_foreach(list, PRINTF_IT_ON_INT);
    CN_unique(list, CMP_INT, NULL);
    // printf("\n\n after unique\n");
    // CN_foreach(list, PRINTF_IT_ON_INT);
    // printf("\n\n");
    
    for(It first = CN_first(list); !It_equal(first, CN_last(list)); first=It_next(first)){

        It next = It_next(first);
        int v1 = It_getint(first);
        int v2 = It_getint(next);

        CU_ASSERT_FALSE( v1 == v2);
    }

    List_(list, NULL);
}

static void test_list_wring(void) 
{
    List list = _List( EQUL );

    CN_add(list, i2t(11));
    CN_add(list, i2t(11));
    CN_add(list, i2t(22));
    CN_add(list, i2t(23));
    CN_add(list, i2t(24));
    CN_add(list, i2t(24));
    CN_add(list, i2t(11));
    CN_add(list, i2t(44));
    CN_add(list, i2t(55));
    CN_add(list, i2t(55));
    CN_add(list, i2t(44));

    // printf ("\n\n before wring\n");
    // CN_foreach(list, PRINTF_IT_ON_INT);
    CN_wring(list, NULL);
    // printf ("\n\n after wring\n");
    // CN_foreach(list, PRINTF_IT_ON_INT);
    // printf ("\n\n");
    
    for(It first = CN_first(list); !It_equal(first, CN_last(list)); first=It_next(first)){

        It next = It_next(first);
        int v1 = It_getint(first);
        int v2 = It_getint(next);

        CU_ASSERT_FALSE( v1 == v2);
    }

    List_(list, NULL);
}

static void test_list(void) 
{
    printf("n\n\n测试 list, 插入10个整数: \n");
    list_t* list = list_create();
    container_insert_tail(list, int_vtype(1));
    container_insert_tail(list, int_vtype(2));
    container_insert_tail(list, int_vtype(3));
    container_insert_tail(list, int_vtype(4));
    container_insert_tail(list, int_vtype(5));
    container_insert_tail(list, int_vtype(6));
    container_insert_tail(list, int_vtype(7));
    container_insert_tail(list, int_vtype(8));
    container_insert_tail(list, int_vtype(9));
    container_insert_tail(list, int_vtype(10));

    for(iterator_t first = container_first(list); !iterator_equal(first, container_tail(list)); first=iterator_next(first)) {
        type_value_t t = iterator_dereference(first);
        int v = vtype_int(t);
        printf(" %d ", v);
    }

    printf("\n\n删除第一和最后一个元素:\n");
    container_remove_first(list, NULL);
    container_remove_last(list, NULL);

    for(iterator_t first = container_first(list); !iterator_equal(first, container_tail(list)); first=iterator_next(first)) {
        type_value_t t = iterator_dereference(first);
        printf(" %d ", vtype_int(t));
    }
    list_destroy(list);

    list_t* list2 = list_create();
    printf("\n\n\n\n测试 list2, 插入10个浮点: \n");
    container_insert_tail(list2, float_vtype(1.1));
    container_insert_tail(list2, float_vtype(2.2));
    container_insert_tail(list2, float_vtype(3.3));
    container_insert_tail(list2, float_vtype(4.4));
    container_insert_tail(list2, float_vtype(5.5));
    container_insert_tail(list2, float_vtype(6.6));
    container_insert_tail(list2, float_vtype(7.7));
    container_insert_tail(list2, float_vtype(8.8));
    container_insert_tail(list2, float_vtype(9.9));
    container_insert_tail(list2, float_vtype(10.11));

    for(iterator_t first = container_first(list2); !iterator_equal(first, container_tail(list2)); first=iterator_next(first)) {
        type_value_t t = iterator_dereference(first);
        printf(" %f ", vtype_float(t));
    }

    printf("\n\n 删除第一和最后一个元素:\n");
    container_remove_first(list2, NULL);
    container_remove_last(list2, NULL);

    for(iterator_t first = container_first(list2); !iterator_equal(first, container_tail(list2)); first=iterator_next(first)) {
        type_value_t t = iterator_dereference(first);
        printf(" %f ", vtype_float(t));
    }
    list_destroy(list2);

    list_t* list3 = list_create();
    printf("\n\n\n测试 list3, 插入7个字符串 \n");
    container_insert_tail(list3, pointer_vtype("hello6"));
    container_insert_tail(list3, pointer_vtype("hello5"));
    container_insert_tail(list3, pointer_vtype("hello4"));
    container_insert_tail(list3, pointer_vtype("hello3"));
    container_insert_tail(list3, pointer_vtype("hello2"));
    container_insert_tail(list3, pointer_vtype("hello1"));
    container_insert_tail(list3, pointer_vtype("hello0"));

    for(iterator_t first = container_first(list3); !iterator_equal(first, container_tail(list3)); first=iterator_next(first)) {
        type_value_t t = iterator_dereference(first);
        printf(" %s ", vtype_pointer(t));
    }

    printf("\n\n 删除第一和最后一个元素:\n");
    container_remove_first(list3, NULL);
    container_remove_last(list3, NULL);

    for(iterator_t first = container_first(list3); !iterator_equal(first, container_tail(list3)); first=iterator_next(first)) {
        type_value_t t = iterator_dereference(first);
        printf(" %s ", vtype_pointer(t));
    }
    list_destroy(list3);
    printf("\n\n\n\n");
};

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

    if (NULL == CU_add_test(pSuite, "test List wring", test_list_wring) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test List", test_list) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
}