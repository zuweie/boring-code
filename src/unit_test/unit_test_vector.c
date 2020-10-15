/*
 * @Author: your name
 * @Date: 2020-10-12 23:35:44
 * @LastEditTime: 2020-10-15 07:45:23
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
    CU_ASSERT(VECTOR_SIZE==CN_size(vector));
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

   Tv find = getTSf(9);
   It pos = CN_find(vector, find);
   CU_ASSERT(It_valid(pos));
   Vector_uninit(vector, NULL);
}

static void test_vector_remove() 
{
    CU_ASSERT(1);
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
}