/*
 * @Author: your name
 * @Date: 2020-12-02 17:37:16
 * @LastEditTime: 2025-06-09 12:12:29
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/unit_test_heap_sort.c
 */

#include <stdio.h>
#include <CUnit/Basic.h>
#include "base/__iterator.h"
#include "base/operate/__heap_sort.h"
#include "base/__vector.h"
#include "base/__list.h"
#include "base/type_value/__type_value_def.h"
#include "test_data.h"
#include "unit_test.h"

#include "container/Cont.h"
#include "container/MxQueue.h"
static int __icmp_int(T* t1, T* t2) 
{
    int i1 = type_value_(t1, int);
    int i2 = type_value_(t2, int);
    return ( i1 > i2 )? -1 : (i1 == i2) ? 0 : 1;
}
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
    T_clazz ty_clazz;
    ty_clazz._def = T_def_get(int_t);
    ty_clazz._adapter[e_setup] = T_adapter_get(ty_clazz._def.ty_id, e_setup);
    ty_clazz._adapter[e_cmp]   = T_adapter_get(ty_clazz._def.ty_id, e_cmp);

    vector_t* vector = container_create(vector, &ty_clazz);
    for (int i=0; i<10; ++i) {
        int v = tsd_get_int(i);
        container_insert(vector, container_tail(vector), &v);
    }


    heap_sort(vector, T_cmp(&ty_clazz));

    for(iterator_t first = container_first(vector); !iterator_equal(first, container_last(vector)); first = iterator_next(first)) {
        //printf("%d ", vtype_int(iterator_dereference(first)));
        iterator_t next = iterator_next(first);
        int i1 = type_value_(first.reference, int);
        int i2 = type_value_(next.reference, int);
        CU_ASSERT_TRUE( i1 <= i2 );
    }
    container_destroy(vector, vector);
}

static void test_list_heap_sort(void)
{
    T_clazz ty_clazz;
    ty_clazz._def = T_def_get(int_t);
    ty_clazz._adapter[e_setup] = T_adapter_get(ty_clazz._def.ty_id, e_setup);
    ty_clazz._adapter[e_cmp]   = T_adapter_get(ty_clazz._def.ty_id, e_cmp);
    list_t* list = container_create(list, &ty_clazz);

    for (int i=0; i<10; ++i) {
        int v = tsd_get_int(i);
        container_insert(list, container_tail(list), &v);
    }

    heap_sort(list, __icmp_int);

    for(iterator_t first = container_first(list); !iterator_equal(first, container_last(list)); first=iterator_next(first)) {
        //printf("%d ", vtype_int(iterator_dereference(first)));
        
        iterator_t next = iterator_next(first);
        int i1 = type_value_(first.reference, int);
        int i2 = type_value_(next.reference, int);

        CU_ASSERT_TRUE( i1 > i2 );
    }
    // printf("\n\n");
    container_destroy(list, list);
}

static void test_mx_queue (void) 
{
    // MxQueue q = _MxQueue(CMP_INT);

    // MxQueue_add(q, i2t(3));
    // MxQueue_add(q, i2t(1));
    // MxQueue_add(q, i2t(9));
    // MxQueue_add(q, i2t(6));

    // MxQueue_add(q, i2t(8));
    // MxQueue_add(q, i2t(5));
    // MxQueue_add(q, i2t(4));
    // MxQueue_add(q, i2t(12));

    // for (int i=0; i<8; ++i) {
    //     Tv v;
    //     MxQueue_extract(q, v);
    //     PRINTF_TV_ON_INT(v);
    // }

    // MxQueue_(q, NULL);
    // CU_ASSERT_TRUE(1);
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