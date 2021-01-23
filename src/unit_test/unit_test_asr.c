/*
 * @Author: your name
 * @Date: 2021-01-24 03:03:52
 * @LastEditTime: 2021-01-24 03:27:43
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/unit_test_ars.c
 */
#include <stdio.h>
#include <CUnit/Basic.h>
#include "test_data.h"
#include "automatic_speech_recognition/signal_process.h"
#include "automatic_speech_recognition/mfcc.h"

static int  suite_success_init (void) 
{
    printf("\nasr suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("\nasr suite success clean\n");
}

int do_dct_test (void) 
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("Test asr Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }
}