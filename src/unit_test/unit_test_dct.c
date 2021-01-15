/*
 * @Author: your name
 * @Date: 2021-01-15 09:14:00
 * @LastEditTime: 2021-01-15 09:15:40
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/unit_test_dct.c
 */
#include <stdio.h>
#include <CUnit/Basic.h>
#include "test_data.h"

static int  suite_success_init (void) 
{
    printf("\ndct suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("\ndct suite success clean\n");
}

int do_dct_test (void) 
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("Test dct Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }
}