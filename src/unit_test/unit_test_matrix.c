/*
 * @Author: your name
 * @Date: 2021-04-06 22:15:11
 * @LastEditTime: 2021-12-05 10:30:11
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/unit_test_matrix.h
 */
#include <stdio.h>
#include <CUnit/Basic.h>
#include "matrix/matrix.h"

#define PRINTF_MATRIX(matrix) \
    ({ \
        printf("Max rows: %d, cols: %d \n", Mat_rows(&matrix), Mat_cols(&matrix)); \
        Mat_eptr(&matrix, ptr);\
        for (size_t i=0; i<Mat_rows(&matrix); ++i) { \
            for (size_t j=0; j<Mat_cols(&matrix); ++j) { \
                printf("%lf ", ptr[i][j]); \
            } \
            printf("\n"); \
        } \
    })

static int  suite_success_init (void) 
{
    printf("\nMatrix suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("\nMatrix suite success clean\n");
}

static void test_insert_row()
{

}

static void test_delete_row()
{

}

static void test_insert_col()
{

}

static void test_delete_col()
{
    
}

static void test_move_row() 
{
    matrix_t mat = Mat_create(3, 3);
    Mat_arange(&mat, 1, 9);
    printf("\n");
    PRINTF_MATRIX(mat);

    Mat_move_rows(&mat, 0, 2);
    printf("\n");
    PRINTF_MATRIX(mat);
    
    Mat_move_rows(&mat,0, -2);
    printf("\n");
    PRINTF_MATRIX(mat);

    Mat_move_rows(&mat, 0, 2);
    printf("\n");
    PRINTF_MATRIX(mat);

    Mat_move_rows(&mat, 2, -4);
    printf("\n");
    PRINTF_MATRIX(mat);

    Mat_move_rows(&mat, 1, 20);
    printf("\n");
    PRINTF_MATRIX(mat);

    Mat_destroy(&mat);
    
}

static void test_move_col()
{
    matrix_t mat = Mat_create(3,3);
    Mat_arange(&mat, 1, 9);
    printf("\n");
    PRINTF_MATRIX(mat);

    Mat_move_cols(&mat, 0, 2);
    printf("\n");
    PRINTF_MATRIX(mat);

    Mat_move_cols(&mat, 0, -2);
    printf("\n");
    PRINTF_MATRIX(mat);

    Mat_destroy(&mat);
}

static void test_mat_dot() 
{
    matrix_t mat1 = Mat_create(2,3);
    Mat_arange(&mat1, 1.f, 6.f);

    printf("\n");
    PRINTF_MATRIX(mat1);

    matrix_t mat2 = Mat_create(3, 2);
    Mat_fill(&mat2, 1.f);

    printf("\n");
    PRINTF_MATRIX(mat2);

    Mat_dot(&mat1, &mat2);

    printf("\n");
    PRINTF_MATRIX(mat1);
    Mat_destroy(&mat1);

}

static void test_mat_op(void) 
{
    int v1=1, v2=2;
    mat_op(v1, v2, op_add);
    CU_ASSERT_EQUAL(v1, 3);

    v1 = 4; v2 = 2;
    mat_op(v1, v2, op_sub);
    CU_ASSERT_EQUAL(v1, 2);

    v1 = 4; v2 = 2;
    mat_op(v1, v2, op_multi);
    CU_ASSERT_EQUAL(v1, 8);

    v1 = 4; v2 = 2;
    mat_op(v1, v2, op_div);
    CU_ASSERT_EQUAL(v1, 2);
}


static void test_mat_deflate(void) 
{
    matrix_t mat = Mat_create(7, 3);
    for (int i=0; i<mat.rows*mat.cols; ++i) {
        mat.pool[i] = (float)1;
    }
    Mat_deflate(&mat, dimen_col, op_add);
    PRINTF_MATRIX(mat);
}

int do_matrix_test (void) 
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("Test matrix Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }

    // if (NULL == CU_add_test(pSuite, "test move row", test_move_row) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "test move col", test_move_col) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "test mat dot ", test_mat_dot) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "test mat dot ", test_mat_op) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    if (NULL == CU_add_test(pSuite, "test mat dot ", test_mat_deflate) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
}