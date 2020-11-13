/*
 * @Author: your name
 * @Date: 2020-11-12 12:23:02
 * @LastEditTime: 2020-11-13 12:26:19
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/unit_test_dense_matrix.c
 */
#include <stdio.h>
#include <CUnit/Basic.h>
#include "matrix/DenseMatrix.h"
#include "test_data.h"


#define PR_DENSEMATRIX(matrix_ptr, row, col) do { \
    for (int i=0; i<row; ++i) { \
        for (int j=0; j<col; ++j) { \
            printf("%02f ", Matrix_get(matrix_ptr, i, j)); \
        } \
        printf("\n"); \
    } \
} while(0)

static int  suite_success_init (void) 
{
    printf("\nDenseMatrix suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("\nDenseMatrix suite success clean\n");
}

static void test_dense_matrix_set_get (void)
{
    extern float test_data_float[TEST_DATA_SIZE];
    DenseMatrix* matrix = DenseMatrix_load(25, 4, test_data_float);
    printf("\n\n");
    printf("rows %d cols %d\n", Matrix_rows(matrix), Matrix_cols(matrix));
    PR_DENSEMATRIX(matrix, Matrix_rows(matrix), Matrix_cols(matrix));
    DenseMatrix_destroy(matrix);
    CU_ASSERT_TRUE(1);
}

static void test_dense_matrix_trans (void) 
{
    extern float test_data_float[TEST_DATA_SIZE];
    DenseMatrix* matrix = DenseMatrix_load(25, 4, test_data_float);
    printf("\n\n");
    printf("rows %d cols %d\n", Matrix_rows(matrix), Matrix_cols(matrix));
    PR_DENSEMATRIX(matrix, Matrix_rows(matrix), Matrix_cols(matrix));
    printf("\n\n");
    Matrix_trans(matrix);
    printf("rows %d cols %d\n", Matrix_rows(matrix), Matrix_cols(matrix));
    PR_DENSEMATRIX(matrix, Matrix_rows(matrix), Matrix_cols(matrix));
    printf("\n\n");
    Matrix_trans(matrix);
    printf("rows %d cols %d\n", Matrix_rows(matrix), Matrix_cols(matrix));
    PR_DENSEMATRIX(matrix, Matrix_rows(matrix), Matrix_cols(matrix));
    DenseMatrix_destroy(matrix);
    CU_ASSERT_TRUE(1);
}

int do_dense_matrix_test (void) 
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("Test DenseMatrix Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test DenseMatrix set get", test_dense_matrix_set_get) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if (NULL == CU_add_test(pSuite, "test DenseMatrix trans", test_dense_matrix_trans) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
}