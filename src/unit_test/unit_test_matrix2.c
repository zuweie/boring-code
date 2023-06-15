#include <stdio.h>
#include <CUnit/Basic.h>
#include "statistical_learning/matrix2.h"
#include "statistical_learning/matrix2_operator.h"
#define PRINTF_DOUBLES(x) printf("%lf ", (x));

static int  suite_success_init (void) 
{
    printf("\n matrix2 suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("\n matrix2 suite success clean\n");
}

static void test_mat2_rescale(void) {

    matrix2_t* mat = Mat2_create(4,4);
    Mat2_arange(mat, 1, 4*4);
    MAT2_INSPACT(mat);

    matrix2_t* mat2 = Mat2_create_cpy(mat);
    MAT2_INSPACT(mat2);

    __mat2_rescale(
        &(mat2->pool),
        &(mat2->rows),
        &(mat2->cols),
        mat->pool,
        mat->rows,
        mat->cols,
        -2,
        -2,
        2,
        2,
        7
    );
    MAT2_INSPACT(mat2);

    matrix2_t* mat3 = Mat2_create(1,1);

    __mat2_rescale(
        &(mat3->pool),
        &(mat3->rows),
        &(mat3->cols),
        mat2->pool,
        mat2->rows,
        mat2->cols,
        1,
        1,
        -1,
        -1,
        6
    );
    MAT2_INSPACT(mat3);

    Mat2_destroy(mat);
    Mat2_destroy(mat2);
    Mat2_destroy(mat3);
}

static void test_matrix_add_sub_scalar_mulity(void) {
    matrix2_t* mat1 = Mat2_create(4,4);
    matrix2_t* mat2 = Mat2_create(4,4);

    Mat2_fill(mat1, 1);
    Mat2_fill(mat2, 3);

    Mat2_add(mat1, mat2);

    MAT2_INSPACT(mat1);

    Mat2_sub(mat1, mat2);
    MAT2_INSPACT(mat1);

    Mat2_scalar_multiply(mat1, 0.5);
    MAT2_INSPACT(mat1);

    Mat2_destroy(mat1);
    Mat2_destroy(mat2);
}

static void test_matrix_slice(void) {

    matrix2_t* mat1 = Mat2_create(4,4);
    Mat2_arange(mat1, 1, 4*4);
    MAT2_INSPACT(mat1);

    matrix2_t* mat2 = Mat2_create(1,1);
    Mat2_slice_row_to(mat2, mat1, 2);
    MAT2_INSPACT(mat2);

    Mat2_slice_col_to(mat2, mat1, 3);
    MAT2_INSPACT(mat2);
    
    Mat2_destroy(mat1);
    Mat2_destroy(mat2);
}

static void test_matrix_slice2(void) 
{
    matrix2_t* mat1 = Mat2_create(4,4);
    Mat2_arange(mat1, 1, 4*4);
    //MAT2_INSPACT(mat1);

    matrix2_t* mat2 = Mat2_create(1,1);

    // cut the cols from mat1 to mat2
    Mat2_slice_cols_to(mat2, mat1, 1, mat1->cols);
    CU_ASSERT_EQUAL(mat2->cols, 3);
    //MAT2_INSPACT(mat2);

    Mat2_slice_cols_to(mat2, mat1, 1, 3);
    CU_ASSERT_EQUAL(mat2->cols, 2);
    //MAT2_INSPACT(mat2);

    Mat2_slice_col_to(mat2, mat1, 3);
    CU_ASSERT_EQUAL(mat2->cols, 1);
    //MAT2_INSPACT(mat2);

    Mat2_slice_rows_to(mat2, mat1, 1, mat1->rows);
    CU_ASSERT_EQUAL(mat2->rows, 3);
    //MAT2_INSPACT(mat2);

    Mat2_slice_rows_to(mat2, mat1, 1, 3);
    CU_ASSERT_EQUAL(mat2->rows, 2);
    //MAT2_INSPACT(mat2);

    Mat2_slice_row_to(mat2, mat1, 2);
    CU_ASSERT_EQUAL(mat2->rows, 1);
    //MAT2_INSPACT(mat2);

    Mat2_destroy(mat1);
    Mat2_destroy(mat2);
    
}

static void test_matrix_load_csv(void) 
{
    matrix2_t* mat = Mat2_create(1,1);
    Mat2_load_csv(mat, "../src/unit_test/mnist/mnist_train.csv");
    MAT2_INSPACT(mat);
    Mat2_destroy(mat);
}

int do_matrix2_test (void) 
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("Test Matrix2 Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }

    // if (NULL == CU_add_test(pSuite, "test mat rescale ", test_mat2_rescale) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }
    
    // if (NULL == CU_add_test(pSuite, "test mat add sub mulity ", test_matrix_add_sub_scalar_mulity) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "test mat add sub mulity ", test_matrix_slice) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, " load mnist csv ", test_matrix_load_csv) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    if (NULL == CU_add_test(pSuite, "test mat add sub mulity ", test_matrix_slice2) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

}