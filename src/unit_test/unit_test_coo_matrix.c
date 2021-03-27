/*
 * @Author: your name
 * @Date: 2020-11-11 14:37:11
 * @LastEditTime: 2021-03-26 11:53:03
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/unit_test_coo_matrix.c
 */
#include <stdio.h>
#include <CUnit/Basic.h>
#include "matrix/CooMatrix.h"
#include "matrix/DenseMatrix.h"
#include "test_data.h"

#define PR_COOMATRIX(matrix_ptr) do { \
    for (int i=0; i<Matrix_rows(matrix_ptr); ++i) { \
        for (int j=0; j<Matrix_cols(matrix_ptr); ++j) { \
            printf("%02f ", Matrix_get(matrix_ptr, i, j)); \
        } \
        printf("\n"); \
    } \
} while(0)

static int  suite_success_init (void) 
{
    printf("\nCooMatrix suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("\nCooMatrix suite success clean\n");
}

static void test_coo_matrix_set_get (void)
{
    extern float test_data_float[TEST_DATA_SIZE];
    CooMatrix* matrix = CooMatrix_load(20, 5, test_data_float);
    printf("\n\n");
    PR_COOMATRIX(matrix);
    
    CooMatrix_destroy(matrix);
    CU_ASSERT_TRUE(1);
}
static void test_coo_matrix_trans (void) 
{
    extern float test_data_float[TEST_DATA_SIZE];
    CooMatrix* matrix = CooMatrix_load(2, 10, test_data_float);
    printf("\n\n");
    PR_COOMATRIX(matrix);

    Matrix_trans(matrix);

    printf("\n\n");
    PR_COOMATRIX(matrix);
    CooMatrix_destroy(matrix);
    CU_ASSERT_TRUE(1);
}

static void test_coo_matrix_product (void) {

    float m1[20] = {1, 3 ,-2, 0, 4, -2, -1, 5, -7, 2, 0, 8, 4, 1, -5, 3, -3, 2, -4, 1};
    float m2[15] = {4, 5, -1, 2 ,-2, 6, 7, 8, 1, 0, 3, -5, 9, 8, -6};
    CooMatrix* matrix1 = CooMatrix_load(4, 5, m1);
    DenseMatrix* matrix2 = DenseMatrix_load(5, 3, m2);

    CooMatrix* product = CooMatrix_create(Matrix_rows(matrix1), Matrix_cols(matrix2));
    product = Matrix_product(matrix1, matrix2, product);
    printf("\n\n");
    printf("rows %d cols %d\n", Matrix_rows(product), Matrix_cols(product));
    PR_COOMATRIX(product);
    printf("\n\n");
    CooMatrix_destroy(matrix1);
    DenseMatrix_destroy(matrix2);
    CooMatrix_destroy(product);
}
int do_coo_matrix_test (void) 
{
    #if 0
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("Test CooMatrix Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test CooMatrix set get", test_coo_matrix_set_get) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test CooMatrix trans", test_coo_matrix_trans) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test CooMatrix product", test_coo_matrix_product) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    #endif
}