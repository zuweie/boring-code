/*
 * @Author: your name
 * @Date: 2020-12-02 17:37:16
 * @LastEditTime: 2020-12-04 09:45:14
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/unit_test_heap_sort.c
 */

#include <stdio.h>
#include <CUnit/Basic.h>
#include "base/__iterator.h"
#include "base/operate/__heap_sort.h"
#include "base/__vector.h"
#include "test_data.h"
#include "container/Cn.h"
static int  suite_success_init (void) 
{
    printf("\n Heapsort suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("\n Heapsort suite success clean\n");
}

static void test_heap_sort(void) 
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
        CU_ASSERT_TRUE( vtype_cmp_on_int(t1, t2) < 0 );
    }
    // printf("\n\n");
    container_destroy(vector, vector);
}

int do_heapsort_test(void) 
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("The Heapsort Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if (NULL == CU_add_test(pSuite, "test heapsort ", test_heap_sort) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
}