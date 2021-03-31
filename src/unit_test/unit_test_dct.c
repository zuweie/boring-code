/*
 * @Author: your name
 * @Date: 2021-01-15 09:14:00
 * @LastEditTime: 2021-03-31 22:10:33
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/unit_test_dct.c
 */
#include <stdio.h>
#include <CUnit/Basic.h>
#include "test_data.h"
#include "cosine_transform/cosine_transform.h"

#define PRINTF_DOUBLES(x) printf("%lf ", (x));

static int  suite_success_init (void) 
{
    printf("\ndct suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("\ndct suite success clean\n");
}

static void test_dct_test(void) 
{
    int N = 4;
    double x[] = {
        2,3,3,2
    };
    double X[N];
    Discrete_cosine_transform(x, N, N, X, dct_ortho, dct_ii);
    printf("\n DCT: \n");
    for(int i=0; i<N; ++i) {
        PRINTF_DOUBLES(X[i]);
    }
    printf("\n");
}

int do_dct_test (void) 
{
    #if 0
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("Test dct Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test dct", test_dct_test) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    #endif 
}