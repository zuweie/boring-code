/*
 * @Author: your name
 * @Date: 2021-03-31 22:05:01
 * @LastEditTime: 2021-05-09 16:59:53
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/unit_test_machine_learnling.c
 */
#include <stdio.h>
#include <CUnit/Basic.h>
#include "test_tool.h"
#include "ultra_array/ultra_array.h"
#include "machine_learning/linear_regression.h"
#include "machine_learning/logistic_regression.h"

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

static void test_logistic_regression_train() 
{
    vfloat_t X_t[100][3] = {
        {1.,4.7,1.4},{1.,4.5,1.5},{1.,4.9,1.5},{1.,4.,1.3},{1.,4.6,1.5},{1.,4.5,1.3},{1.,4.7,1.6},{1.,3.3,1.,},{1.,4.6,1.3},{1.,3.9,1.4},\
        {1.,3.5,1.,},{1.,4.2,1.5},{1.,4.,1.,},{1.,4.7,1.4},{1.,3.6,1.3},{1.,4.4,1.4},{1.,4.5,1.5},{1.,4.1,1.,},{1.,4.5,1.5},{1.,3.9,1.1},\
        {1.,4.8,1.8},{1.,4.,1.3},{1.,4.9,1.5},{1.,4.7,1.2},{1.,4.3,1.3},{1.,4.4,1.4},{1.,4.8,1.4},{1.,5.,1.7},{1.,4.5,1.5},{1.,3.5,1.,},\
        {1.,3.8,1.1},{1.,3.7,1.,},{1.,3.9,1.2},{1.,5.1,1.6},{1.,4.5,1.5},{1.,4.5,1.6},{1.,4.7,1.5},{1.,4.4,1.3},{1.,4.1,1.3},{1.,4.,1.3},\
        {1.,4.4,1.2},{1.,4.6,1.4},{1.,4.,1.2},{1.,3.3,1.,},{1.,4.2,1.3},{1.,4.2,1.2},{1.,4.2,1.3},{1.,4.3,1.3},{1.,3.,1.1},{1.,4.1,1.3},\
        {1.,6.,2.5},{1.,5.1,1.9},{1.,5.9,2.1},{1.,5.6,1.8},{1.,5.8,2.2},{1.,6.6,2.1},{1.,4.5,1.7},{1.,6.3,1.8},{1.,5.8,1.8},{1.,6.1,2.5},\
        {1.,5.1,2.,},{1.,5.3,1.9},{1.,5.5,2.1},{1.,5.,2.,},{1.,5.1,2.4},{1.,5.3,2.3},{1.,5.5,1.8},{1.,6.7,2.2},{1.,6.9,2.3},{1.,5.,1.5},\
        {1.,5.7,2.3},{1.,4.9,2.,},{1.,6.7,2.,},{1.,4.9,1.8},{1.,5.7,2.1},{1.,6.,1.8},{1.,4.8,1.8},{1.,4.9,1.8},{1.,5.6,2.1},{1.,5.8,1.6},\
        {1.,6.1,1.9},{1.,6.4,2.,},{1.,5.6,2.2},{1.,5.1,1.5},{1.,5.6,1.4},{1.,6.1,2.3},{1.,5.6,2.4},{1.,5.5,1.8},{1.,4.8,1.8},{1.,5.4,2.1},\
        {1.,5.6,2.4},{1.,5.1,2.3},{1.,5.1,1.9},{1.,5.9,2.3},{1.,5.7,2.5},{1.,5.2,2.3},{1.,5.,1.9},{1.,5.2,2.,},{1.,5.4,2.3},{1.,5.1,1.8}
        };

    vfloat_t y_t[100] = {-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,\
                         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,\
                         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,\
                         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,\
                         -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,\
                          1, 1, 1, 1, 1, 1, 1, 1, 1, 1,\
                          1, 1, 1, 1, 1, 1, 1, 1, 1, 1,\
                          1, 1, 1, 1, 1, 1, 1, 1, 1, 1,\
                          1, 1, 1, 1, 1, 1, 1, 1, 1, 1,\
                          1, 1, 1, 1, 1, 1, 1, 1, 1, 1};

    vfloat_t W_t[3] = {0};

    u_array_t X = _UArray2d(100, 3);
    u_array_t y = _UArray2d(100, 1);
    u_array_t W = _UArray2d(3, 1);
    
    UA_load(&X, X_t);
    UA_load(&y, y_t);
    UA_load(&W, W_t);
    
    vfloat_t eta = 0.1f;
    vfloat_t epochs = 10000000;
    vfloat_t epsilon = 0.0005;

    logistic_regression_train(&X, &y, &W, eta, epochs, epsilon);

    UA_display(&W);
    
    UArray_(&X);
    UArray_(&y);
    UArray_(&W);
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

