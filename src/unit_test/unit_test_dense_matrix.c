/*
 * @Author: your name
 * @Date: 2020-11-12 12:23:02
 * @LastEditTime: 2021-03-27 14:02:16
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/unit_test_dense_matrix.c
 */
#include <stdio.h>
#include <CUnit/Basic.h>
#include "matrix/DenseMatrix.h"
#include "test_data.h"


#define PR_DENSEMATRIX(matrix_ptr) do { \
    for (int i=0; i<Matrix_rows(matrix_ptr); ++i) { \
        for (int j=0; j<Matrix_cols(matrix_ptr); ++j) { \
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
    extern vfloat_t test_data_float[TEST_DATA_SIZE];
    DenseMatrix* matrix = DenseMatrix_load(25, 4, test_data_float);
    // printf("\n\n");
    // printf("rows %d cols %d\n", Matrix_rows(matrix), Matrix_cols(matrix));
    // PR_DENSEMATRIX(matrix);
    DenseMatrix_destroy(matrix);
    CU_ASSERT_TRUE(1);
}

static void test_dense_matrix_trans (void) 
{
    extern vfloat_t test_data_float[TEST_DATA_SIZE];
    DenseMatrix* matrix = DenseMatrix_load(25, 4, test_data_float);
    // printf("\n\n");
    // printf("rows %d cols %d\n", Matrix_rows(matrix), Matrix_cols(matrix));
    // PR_DENSEMATRIX(matrix);
    // printf("\n\n");
    // Matrix_trans(matrix);
    // printf("rows %d cols %d\n", Matrix_rows(matrix), Matrix_cols(matrix));
    // PR_DENSEMATRIX(matrix);
    // printf("\n\n");
    // Matrix_trans(matrix);
    // printf("rows %d cols %d\n", Matrix_rows(matrix), Matrix_cols(matrix));
    // PR_DENSEMATRIX(matrix);
    DenseMatrix_destroy(matrix);
    CU_ASSERT_TRUE(1);
}


static void test_dense_matrix_product(void)
{
    //extern float test_data_float[TEST_DATA_SIZE];
    vfloat_t m1[20] = {1, 3 ,-2, 0, 4, -2, -1, 5, -7, 2, 0, 8, 4, 1, -5, 3, -3, 2, -4, 1};
    vfloat_t m2[15] = {4, 5, -1, 2 ,-2, 6, 7, 8, 1, 0, 3, -5, 9, 8, -6};
    DenseMatrix* matrix1 = DenseMatrix_load(4, 5, m1);
    DenseMatrix* matrix2 = DenseMatrix_load(5, 3, m2);

    // printf("\n\n");
    // printf("rows %d cols %d\n", Matrix_rows(matrix1), Matrix_cols(matrix1));
    // PR_DENSEMATRIX(matrix1);


    // printf("\n\n");
    // printf("rows %d cols %d\n", Matrix_rows(matrix2), Matrix_cols(matrix2));
    // PR_DENSEMATRIX(matrix2);

    DenseMatrix* product = DenseMatrix_create(Matrix_rows(matrix1), Matrix_cols(matrix2));
    product = Matrix_product(matrix1, matrix2, product);

    //printf("\n\n");
    //printf("rows %d cols %d\n", Matrix_rows(product), Matrix_cols(product));
    //PR_DENSEMATRIX(product);

    DenseMatrix_destroy(matrix1);
    DenseMatrix_destroy(matrix2);
    DenseMatrix_destroy(product);
}

static void test_dense_matrix_lu (void) 
{
    vfloat_t m[6] = {2, 6, 4, 5, 7, 9};
    DenseMatrix* m1 = DenseMatrix_load(2, 3, m);

    DenseMatrix_lu(m1);

    DenseMatrix_elem_ptr(m1, ptr);
    CU_ASSERT_TRUE(ptr[0][0] == 2.f);
    CU_ASSERT_TRUE(ptr[0][1] == 6.f);
    CU_ASSERT_TRUE(ptr[0][2] == 4.f);
    CU_ASSERT_TRUE(ptr[1][0] == 2.5f);
    CU_ASSERT_TRUE(ptr[1][1] == -8.f);
    CU_ASSERT_TRUE(ptr[1][2] == -1.f);
    //PR_DENSEMATRIX(m1);

    DenseMatrix_destroy(m1);
}

static void test_dense_matrix_solve(void) 
{
    vfloat_t X[4] = {4,5,3,2};
    vfloat_t Y[2] = {37, 19};
    DenseMatrix* m1 = DenseMatrix_load(2,2,X);
    DenseMatrix_solve(m1, Y, 2);
    CU_ASSERT_TRUE(Y[0] == 3.f);
    CU_ASSERT_TRUE(Y[1] == 5.f);
    DenseMatrix_destroy(m1);
}

static void test_dense_matrix_inverse(void) 
{
    vfloat_t m[4] = {4,5,3,2};
    DenseMatrix* m2 = DenseMatrix_load(2,2,m);
    DenseMatrix* m1 = DenseMatrix_load(2,2,m);
    DenseMatrix* m3 = DenseMatrix_create(2,2);
    
    DenseMatrix_inverse(m2);
    DenseMatrix_dot(m1, m2, m3);
    // printf("\n original: \n");
    // PR_DENSEMATRIX(m1);
    // printf("\n inverse \n");
    // PR_DENSEMATRIX(m2);
    printf("\n dot \n");
    PR_DENSEMATRIX(m3);

    DenseMatrix_elem_ptr(m3, ptr);

    DenseMatrix_destroy(m1);
    DenseMatrix_destroy(m2);
    DenseMatrix_destroy(m3);
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

    if (NULL == CU_add_test(pSuite, "test DenseMatrix product", test_dense_matrix_product) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test DenseMatrix lu decomp", test_dense_matrix_lu) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test DenseMatrix solve", test_dense_matrix_solve) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test DenseMatrix inverse", test_dense_matrix_inverse) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
}