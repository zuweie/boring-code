/*
 * @Author: your name
 * @Date: 2021-07-02 14:26:30
 * @LastEditTime: 2021-07-14 16:20:51
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/unit_test_svm.c
 */
#include <stdio.h>
#include <CUnit/Basic.h>
#include "container/List.h"
#include "ultra_array/ultra_array.h"
#include "machine_learning/svm/support_vector_machines.h"
#include "machine_learning/svm/svm_problem.h"

static vfloat_t X_data[60][4] = {
        {5.1f, 3.5f, 1.4f, 0.2f}, {4.9f, 3.0f, 1.4f, 0.2f}, {4.7f, 3.2f, 1.3f, 0.2f}, 
        {4.6f, 3.1f, 1.5f, 0.2f}, {5.0f, 3.6f, 1.4f, 0.2f}, {5.4f, 3.9f, 1.7f, 0.4f},
        {4.6f, 3.4f, 1.4f, 0.3f}, {5.0f, 3.4f, 1.5f, 0.2f}, {4.4f, 2.9f, 1.4f, 0.2f},
        {4.9f, 3.1f, 1.5f, 0.1f}, {5.4f, 3.4f, 1.5f, 0.2f}, {4.8f, 3.4f, 1.6f, 0.2f},
        {4.8f, 3.0f, 1.4f, 0.1f}, {4.3f, 3.0f, 1.1f, 0.1f}, {5.8f, 4.0f, 1.2f, 0.2f}, 
        {5.7f, 4.4f, 1.5f, 0.4f}, {5.4f, 3.9f, 1.3f, 0.4f}, {5.1f, 3.5f, 1.4f, 0.3f},
        {5.7f, 3.8f, 1.7f, 0.3f}, {5.1f, 3.8f, 1.5f, 0.3f},
        {7.0f, 3.2f, 4.7f, 1.4f}, {6.4f, 3.2f, 4.5f, 1.5f}, {6.9f, 3.1f, 4.9f, 1.5f}, 
        {5.5f, 2.3f, 4.0f, 1.3f}, {6.5f, 2.8f, 4.6f, 1.5f}, {5.7f, 2.8f, 4.5f, 1.3f}, 
        {6.3f, 3.3f, 4.7f, 1.6f}, {4.9f, 2.4f, 3.3f, 1.0f}, {6.6f, 2.9f, 4.6f, 1.3f},
        {5.2f, 2.7f, 3.9f, 1.4f}, {5.0f, 2.0f, 3.5f, 1.0f}, {5.9f, 3.0f, 4.2f, 1.5f}, 
        {6.0f, 2.2f, 4.0f, 1.0f}, {6.1f, 2.9f, 4.7f, 1.4f}, {5.6f, 2.9f, 3.6f, 1.3f}, 
        {6.7f, 3.1f, 4.4f, 1.4f}, {5.6f, 3.0f, 4.5f, 1.5f}, {5.8f, 2.7f, 4.1f, 1.0f}, 
        {6.2f, 2.2f, 4.5f, 1.5f}, {5.6f, 2.5f, 3.9f, 1.1f},
        {6.3f, 3.3f, 6.0f, 2.5f}, {5.8f, 2.7f, 5.1f, 1.9f}, {7.1f, 3.0f, 5.9f, 2.1f},
        {6.3f, 2.9f, 5.6f, 1.8f}, {6.5f, 3.0f, 5.8f, 2.2f}, {7.6f, 3.0f, 6.6f, 2.1f}, 
        {4.9f, 2.5f, 4.5f, 1.7f}, {7.3f, 2.9f, 6.3f, 1.8f}, {6.7f, 2.5f, 5.8f, 1.8f}, 
        {7.2f, 3.6f, 6.1f, 2.5f}, {6.5f, 3.2f, 5.1f, 2.0f}, {6.4f, 2.7f, 5.3f, 1.9f}, 
        {6.8f, 3.0f, 5.5f, 2.1f}, {5.7f, 2.5f, 5.0f, 2.0f}, {5.8f, 2.8f, 5.1f, 2.4f},
        {6.4f, 3.2f, 5.3f, 2.3f}, {6.5f, 3.0f, 5.5f, 1.8f}, {7.7f, 3.8f, 6.7f, 2.2f}, 
        {7.7f, 2.6f, 6.9f, 2.3f}, {6.0f, 2.2f, 5.0f, 1.5f} 
    };

static vfloat_t Y_data[60][4] = {
        'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 
        'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S',
        'V', 'V', 'V', 'V', 'V', 'V', 'V', 'V', 'V', 'V', 
        'V', 'V', 'V', 'V', 'V', 'V', 'V', 'V', 'V', 'V', 
        'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 
        'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 
    };

static int  suite_success_init (void) 
{
    printf("\nSVM suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("\nSVM suite success clean\n");
}

static void 
test_sample_classify_problems() 
{
    // svm 多分类的效果。
    u_array_t X = _UArray2d(60, 4);
    u_array_t Y = _UArray1d(60);

    UA_load(&X, X_data);
    UA_load(&Y, Y_data);

    List problems = _List(NULL);
    int class_nr = svm_classify_problem(&X, &Y, &problems);

    for (It first=CN_first(&problems); !It_equal(first, CN_tail(&problems)); first = It_next(first)) {
        svm_classify_problem_t* problem = It_getptr(first);

        printf(" %c, %c ", (int)problem->tagA, (int)problem->tagB);
        printf("\n");

        for (It a=CN_first(problem->class_ls_A); !It_equal(a, CN_tail(problem->class_ls_A)); a = It_next(a)) {
            printf(" %d ", It_getint(a));
        }
        printf("\n");
        for (It b=CN_first(problem->class_ls_B); !It_equal(b, CN_tail(problem->class_ls_B)); b = It_next(b) ){
            printf(" %d ", It_getint(b));
        }
        printf("\n");
    }

    svm_classify_problem_finalize(&problems);

    UArray_(&X);
    UArray_(&Y);
}

static void test_c_svc_solve (void)
{
    u_array_t X = _UArray2d(60, 4);
    u_array_t Y = _UArray1d(60);

    UA_load(&X, X_data);
    UA_load(&Y, Y_data);

    List list = _List(NULL);
    
    svm_solve_c_svc(
        &X, &Y, BRF, 10, 8.0f, 0.0f, 0.01, 100000, &list);
    );

    for (It first=CN_first(&list); !It_equal(first, CN_tail(&list)); first=It_next(first)) {

        svm_model_t* model = It_getptr(first);
        printf(" .. model info: ...");
    }
}

int do_svm_test (void) 
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("Test Toutiao Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test svm sample classify", test_sample_classify_problems) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test svm c svc solver", test_c_svc_solve) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
}