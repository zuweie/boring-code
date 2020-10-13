/*
 * @Author: your name
 * @Date: 2020-10-12 23:35:44
 * @LastEditTime: 2020-10-13 11:25:11
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/vetcor_test.c
 */

#include <stdio.h>
#include <CUnit/Basic.h>
#include "container/Vector.h"

static int  suite_success_init (void) 
{
    printf("suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("suite success clean\n");
}

static void test_vector_size(void)
{
    Vector vector;
    Vector_init(vector, NULL);
    

}
int do_vector_test () 
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("Vector suite", suite_success_init, suite_success_clean);
    printf("do vector_test \n");    
}