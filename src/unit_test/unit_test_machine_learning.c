/*
 * @Author: your name
 * @Date: 2021-03-31 22:05:01
 * @LastEditTime: 2021-04-08 21:43:09
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/unit_test_machine_learnling.c
 */
#include <stdio.h>
#include <CUnit/Basic.h>
#include "test_tool.h"
#include "ultra_array/ultra_array.h"
#include "machine_learning/linear_regression.h"

#define PRINTF_DOUBLES(x) printf("%lf ", (x));

static int  suite_success_init (void) 
{
    printf("\nmachine learning suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("\nmachine learning suite success clean\n");
}

static void test_linear_regression(void) 
{
    u_array_t X = _UArray2d(6, 1);
    u_array_t Y = _UArray1d(6);

    vfloat_t * X_ptr = UA_data_ptr(&X);
    vfloat_t * Y_ptr = UA_data_ptr(&Y);

    X_ptr[0] = 1.51f;
    X_ptr[1] = 1.64f;
    X_ptr[2] = 1.6f;
    X_ptr[3] = 1.73f;
    X_ptr[4] = 1.82f;
    X_ptr[5] = 1.87f;

    Y_ptr[0] = 1.63f;
    Y_ptr[1] = 1.7f;
    Y_ptr[2] = 1.71f;
    Y_ptr[3] = 1.72f;
    Y_ptr[4] = 1.76f;
    Y_ptr[5] = 1.86f;

    vfloat_t W[1];
    vfloat_t b;
    Linear_Regression_solve(&X, &Y, W, &b);
    CU_ASSERT_TRUE(equ_float(W[0],0.514133f));
    CU_ASSERT_TRUE(equ_float(b,0.858544f));
    
    return;
}

static void test_linear_regression_pinv(void) 
{
    u_array_t X = _UArray2d(6, 1);
    u_array_t Y = _UArray1d(6);

    vfloat_t * X_ptr = UA_data_ptr(&X);
    vfloat_t * Y_ptr = UA_data_ptr(&Y);

    X_ptr[0] = 1.51f;
    X_ptr[1] = 1.64f;
    X_ptr[2] = 1.6f;
    X_ptr[3] = 1.73f;
    X_ptr[4] = 1.82f;
    X_ptr[5] = 1.87f;

    Y_ptr[0] = 1.63f;
    Y_ptr[1] = 1.7f;
    Y_ptr[2] = 1.71f;
    Y_ptr[3] = 1.72f;
    Y_ptr[4] = 1.76f;
    Y_ptr[5] = 1.86f;

    vfloat_t W[1];
    vfloat_t b;
    Linear_Regression_pseudo_inverse(&X, &Y, W, &b);
    CU_ASSERT_TRUE(equ_float(W[0],0.514133f));
    CU_ASSERT_TRUE(equ_float(b,0.858544f));
    return;
}

int do_macine_learning_test (void) 
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("Test Machine Learning Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test linear regressing", test_linear_regression) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test linear regressing", test_linear_regression_pinv) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
}

