/*
 * @Author: your name
 * @Date: 2020-12-02 17:37:16
 * @LastEditTime: 2020-12-07 17:38:23
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/unit_test_heap_sort.c
 */

#include <stdio.h>
#include <CUnit/Basic.h>
#include "base/__iterator.h"
#include "base/operate/__heap_sort.h"
#include "base/__vector.h"
#include "base/__list.h"
#include "test_data.h"
#include "unit_test.h"

#include "container/Cn.h"
#include "container/MxQueue.h"
static int  suite_success_init (void) 
{
    printf("\n Heapsort suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("\n Heapsort suite success clean\n");
}

static void test_vector_heap_sort(void) 
{
    vector_t* vector = container_create(vector);
    for (int i=0; i<10; ++i) {
        container_insert_tail(vector, getTSi(i));
    }
    // printf("\n\n befort sort \n\n");
    // for(iterator_t first = container_first(vector); !iterator_equal(first, container_tail(vector)); first = iterator_next(first)) {
    //     printf("%d ", vtype_int(iterator_dereference(first)));
    // }

    // printf("\n\n after sort \n\n");

    heap_sort(vector, CMP_INT);

    for(iterator_t first = container_first(vector); !iterator_equal(first, container_last(vector)); first = iterator_next(first)) {
        //printf("%d ", vtype_int(iterator_dereference(first)));
        type_value_t t1 = iterator_dereference(first);
        type_value_t t2 = iterator_dereference(iterator_next(first));
        CU_ASSERT_TRUE( vtype_cmp_on_int(t1, t2) <= 0 );
    }
    // printf("\n\n");
    container_destroy(vector, vector);
}

static void test_list_heap_sort(void)
{
    list_t* list = container_create(list);
    for (int i=0; i<10; ++i) {
        container_insert_tail(list, getTSi(i));
    }
    // printf("\n\n befort sort \n\n");
    // for(iterator_t first = container_first(list); !iterator_equal(first, container_tail(list)); first = iterator_next(first)) {
    //     printf("%d ", vtype_int(iterator_dereference(first)));
    // }

    // printf("\n\n after sort \n\n");

    heap_sort(list, INCMP_INT);

    // printf("\n\n after sort \n\n");
    
    // for(iterator_t first = container_first(list); !iterator_equal(first, container_tail(list)); first = iterator_next(first)) {
    //     printf("%d ", vtype_int(iterator_dereference(first)));
    // }

    for(iterator_t first = container_first(list); !iterator_equal(first, container_last(list)); first = iterator_next(first)) {
        //printf("%d ", vtype_int(iterator_dereference(first)));
        type_value_t t1 = iterator_dereference(first);
        type_value_t t2 = iterator_dereference(iterator_next(first));
        CU_ASSERT_TRUE( vtype_cmp_on_int(t1, t2) > 0 );
    }
    // printf("\n\n");
    container_destroy(list, list);
}

static void test_mx_queue (void) 
{
    MXQueue q = _MxQueue(CMP_INT);

    MxQueue_add(q, i2t(3));
    MxQueue_add(q, i2t(1));
    MxQueue_add(q, i2t(9));
    MxQueue_add(q, i2t(6));

    MxQueue_add(q, i2t(8));
    MxQueue_add(q, i2t(5));
    MxQueue_add(q, i2t(4));
    MxQueue_add(q, i2t(12));

    for (int i=0; i<8; ++i) {
        Tv v;
        MxQueue_extract(q, v);
        PRINTF_TV_ON_INT(v);
    }

    MxQueue_(q, NULL);
    CU_ASSERT_TRUE(1);
}

int do_heapsort_test(void) 
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("The Heapsort Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if (NULL == CU_add_test(pSuite, "test vector heapsort ", test_vector_heap_sort) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if (NULL == CU_add_test(pSuite, "test list heapsort ", test_list_heap_sort) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if (NULL == CU_add_test(pSuite, "test test mx queue  ", test_mx_queue) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
}