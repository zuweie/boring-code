/*
 * @Author: your name
 * @Date: 2020-11-11 14:37:11
 * @LastEditTime: 2021-11-10 14:53:51
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
    extern vfloat_t test_data_float[TEST_DATA_SIZE];
    CooMatrix* matrix = CooMatrix_load(20, 5, test_data_float);
    // printf("\n\n");
    // PR_COOMATRIX(matrix);
    
    vfloat_t v1 = Matrix_get(matrix, 0, 0);
    vfloat_t v2 = Matrix_get(matrix, 19, 4);
    vfloat_t v3 = tsd_get_float(0);
    vfloat_t v4 = tsd_get_float(99);
    CU_ASSERT_DOUBLE_EQUAL(v1, v3, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(v2, v4, 0.0001);
    CooMatrix_destroy(matrix);
    //CU_ASSERT_TRUE(1);
}
static void test_coo_matrix_trans (void) 
{
    extern vfloat_t test_data_float[TEST_DATA_SIZE];
    CooMatrix* matrix = CooMatrix_load(2, 10, test_data_float);
    //printf("\n\n");
    //PR_COOMATRIX(matrix);
    vfloat_t v1 = Matrix_get(matrix, 0, 7);
    vfloat_t v2 = Matrix_get(matrix, 1, 5);
    Matrix_trans(matrix);

    vfloat_t v3 = Matrix_get(matrix, 7, 0);
    vfloat_t v4 = Matrix_get(matrix, 5, 1);
    
    CU_ASSERT_DOUBLE_EQUAL(v1, v3, 0.0001);
    CU_ASSERT_DOUBLE_EQUAL(v2, v4, 0.0001);
    //printf("\n\n");
    //PR_COOMATRIX(matrix);
    CooMatrix_destroy(matrix);
    //CU_ASSERT_TRUE(1);
}

static void test_coo_matrix_product (void) {

    vfloat_t m1[20] = {1, 3 ,-2, 0, 4, -2, -1, 5, -7, 2, 0, 8, 4, 1, -5, 3, -3, 2, -4, 1};
    vfloat_t m2[15] = {4, 5, -1, 2 ,-2, 6, 7, 8, 1, 0, 3, -5, 9, 8, -6};
    CooMatrix* matrix1 = CooMatrix_load(4, 5, m1);
    DenseMatrix* matrix2 = DenseMatrix_load(5, 3, m2);

    CooMatrix* product = CooMatrix_create(Matrix_rows(matrix1), Matrix_cols(matrix2));
    product = Matrix_product(matrix1, matrix2, product);
    //printf("\n\n");
    //printf("rows %d cols %d\n", Matrix_rows(product), Matrix_cols(product));
    int row = Matrix_rows(product);
    int col = Matrix_cols(product);
    CU_ASSERT_EQUAL(row, 4);
    CU_ASSERT_EQUAL(col, 3);
    
    //PR_COOMATRIX(product);
    //printf("\n\n");
    vfloat_t v1 = Matrix_get(product, 0, 1);
    vfloat_t v2 = Matrix_get(product, 3, 2);
    
    CU_ASSERT_DOUBLE_EQUAL(v1, 15.f, 0.0001f);
    CU_ASSERT_DOUBLE_EQUAL(v2, -5.f, 0.0001);

    CooMatrix_destroy(matrix1);
    DenseMatrix_destroy(matrix2);
    CooMatrix_destroy(product);
}
int do_coo_matrix_test (void) 
{
    
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
}