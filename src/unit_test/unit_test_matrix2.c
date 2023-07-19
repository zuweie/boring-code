#include <stdio.h>
#include <CUnit/Basic.h>
#include "statistical_learning/matrix2.h"
#include "statistical_learning/matrix2_operator.h"
#include "statistical_learning/counting.h"
#include "container/cn.h"
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
    MAT2_INSPECT(mat);

    matrix2_t* mat2 = Mat2_create_cpy(mat);
    MAT2_INSPECT(mat2);

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
    MAT2_INSPECT(mat2);

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
    MAT2_INSPECT(mat3);

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

    MAT2_INSPECT(mat1);

    Mat2_sub(mat1, mat2);
    MAT2_INSPECT(mat1);

    Mat2_scalar_multiply(mat1, 0.5);
    MAT2_INSPECT(mat1);

    Mat2_destroy(mat1);
    Mat2_destroy(mat2);
}

static void test_matrix_slice(void) {

    matrix2_t* mat1 = Mat2_create(4,4);
    Mat2_arange(mat1, 1, 4*4);
    MAT2_INSPECT(mat1);

    matrix2_t* mat2 = Mat2_create(1,1);
    Mat2_slice_row_to(mat2, mat1, 2);
    MAT2_INSPECT(mat2);

    Mat2_slice_col_to(mat2, mat1, 3);
    MAT2_INSPECT(mat2);
    
    Mat2_destroy(mat1);
    Mat2_destroy(mat2);
}

static void test_matrix_slice2(void) 
{
    matrix2_t* mat1 = Mat2_create(4,4);
    Mat2_arange(mat1, 1, 4*4);
    //MAT2_INSPECT(mat1);

    matrix2_t* mat2 = Mat2_create(1,1);

    // cut the cols from mat1 to mat2
    Mat2_slice_cols_to(mat2, mat1, 1, mat1->cols);
    CU_ASSERT_EQUAL(mat2->cols, 3);
    //MAT2_INSPECT(mat2);

    Mat2_slice_cols_to(mat2, mat1, 1, 3);
    CU_ASSERT_EQUAL(mat2->cols, 2);
    //MAT2_INSPECT(mat2);

    Mat2_slice_col_to(mat2, mat1, 3);
    CU_ASSERT_EQUAL(mat2->cols, 1);
    //MAT2_INSPECT(mat2);

    Mat2_slice_rows_to(mat2, mat1, 1, mat1->rows);
    CU_ASSERT_EQUAL(mat2->rows, 3);
    //MAT2_INSPECT(mat2);

    Mat2_slice_rows_to(mat2, mat1, 1, 3);
    CU_ASSERT_EQUAL(mat2->rows, 2);
    //MAT2_INSPECT(mat2);

    Mat2_slice_row_to(mat2, mat1, 2);
    CU_ASSERT_EQUAL(mat2->rows, 1);
    //MAT2_INSPECT(mat2);

    Mat2_destroy(mat1);
    Mat2_destroy(mat2);
    
}

static void test_matrix_load_csv(void) 
{
    matrix2_t* mat = Mat2_create(1,1);
    Mat2_load_csv(mat, "../src/unit_test/mnist/mnist_train.csv");
    MAT2_INSPECT(mat);
    Mat2_destroy(mat);
}

static void test_matrix_counting(void) {

    const char* train_csv_file = "/Users/zuweie/code/c-projects/boring-code/build/../src/unit_test/mnist/mnist_train.csv";
    matrix2_t* train_mat = Mat2_create(1,1);
    matrix2_t* col = Mat2_create(1,1);

    //Mat2_load_on_shape(mat, test_data, sizeof(test_data)/(sizeof(vfloat_t)*2), 2);
    Mat2_load_csv(train_mat, train_csv_file);
    MAT2_POOL_PTR(train_mat, train_mat_ptr);

    for (int i=0; i<train_mat->rows; ++i) {
        
        for (int j=0; j<train_mat->cols; ++j) {

            if (train_mat_ptr[i][j] < 63) {
                train_mat_ptr[i][j] = 0;
            } else if (train_mat_ptr[i][j] > 63 && train_mat_ptr[i][j] < 127) {
                train_mat_ptr[i][j] = 1;
            } else if (train_mat_ptr[i][j] > 127 && train_mat_ptr[i][j] < 192) {
                train_mat_ptr[i][j] = 2;
            } else {
                train_mat_ptr[i][j] = 3;
            }

        }
    }
    
    

    // void* counting;
    // MAT2_INSPECT_COUNTING(counting);
    // free(counting);

    // Mat2_list_difference_in_col(mat, 1, &counting);
    // MAT2_INSPECT_COUNTING(counting);
    // free(counting);

    Mat2_slice_col_to(col, train_mat, 456);

    void* counting;
    //__mat2_count_element(col->pool, col->rows, &counting);
    counting_Y(col, &counting);

    INSPECT_COUNTING(counting);

    //MAT2_INSPECT(col);

    Mat2_destroy(train_mat);
    Mat2_destroy(col);
    free(counting);
}


static void test_matrix_cofactor(void) {

    matrix2_t* m1 = Mat2_create(4,4);
    matrix2_t* m2 = Mat2_create(1,1);

    Mat2_arange(m1, 1, 16);
    //MAT2_INSPECT(m1);

    Mat2_get_co_to(m2, m1, 2, 2);

    MAT2_INSPECT(m2);

    Mat2_destroy(m1);
    Mat2_destroy(m2);

}

static void test_matrix_det(void) {
    vfloat_t m1_data[][3] = {
        {1, 2, 3},
        {4, 5, 6},
        {1, 2, 2}
    };
    matrix2_t* m1 = Mat2_create(1,1);
    Mat2_load_on_shape(m1, m1_data, 3, 3);
    MAT2_INSPECT(m1);

    vfloat_t det;
    Mat2_det(m1, &det);


    printf("\n det: %0.2f \n", det);

    Mat2_destroy(m1);

}

static void test_matrix_adjoint(void) 
{
    vfloat_t m1_data[][3] = {
        {1,2,3},
        {4,5,6},
        {7,8,9}
    };

    matrix2_t* m1 = Mat2_create(1,1);
    Mat2_load_on_shape(m1, m1_data, 3,3);

    matrix2_t* m2 = Mat2_create(1,1);

    Mat2_get_adjoint_to(m2, m1);

    MAT2_INSPECT(m2);

    Mat2_destroy(m1);
    Mat2_destroy(m2);

    // respect:
    //  <rows:3, cols:3>
    // -3.000 6.000 -3.000
    // -6.000 12.000 -6.000
    // -3.000 6.000 -3.000
}

static void test_matrix_inverse(void) 
{
    vfloat_t m1_data[][3] = {
        {.035, .808, -0.065},
        {0.808, 122.917, 2.917},
        {-0.065, 2.917, .917}
    };

    matrix2_t* m1 = Mat2_create(1,1);

    Mat2_load_on_shape(m1, m1_data, 3,3);

    Mat2_inverse(m1);

    MAT2_INSPECT(m1);

    Mat2_destroy(m1);

    // respect :
    // <rows:3, cols:3>
    // 46.880 -0.419 4.655
    // 0.419 -0.013 0.070
    // 4.655 -0.070 1.642

}

static void test_matrix_dot(void) {
    vfloat_t m1_data[][3] = {
        {1,2,3},
        //{4,5,6}
    };

    vfloat_t m2_data[][2] = {
        {4,1},
        {5,2},
        {6,3}
    };

    matrix2_t* m1 = Mat2_create(1,1);
    matrix2_t* m2 = Mat2_create(1,1);

    Mat2_load_on_shape(m1, m1_data, 1, 3);
    Mat2_load_on_shape(m2, m2_data, 3, 2);

    Mat2_dot(m1, m2);
    MAT2_INSPECT(m1);
    Mat2_destroy(m1);
    Mat2_destroy(m2);

    // respect:
    // 32, 14
}

static void test_matrix_T(void) 
{
    vfloat_t m1_data[][4] = {
        {1,2,3,4},
        {5,6,7,8}
    };

    matrix2_t* m1 = Mat2_create(1,1);
    Mat2_load_on_shape(m1, m1_data, 2, 4);

    Mat2_T(m1);

    MAT2_INSPECT(m1);

    Mat2_destroy(m1);
}

static void test_matrix_householder_transform(void) {

    // vfloat_t m1_data[][3] = {
    //     {1,2,3},
    //     {5,6,7},
    //     {9,3,2}
    // };

    matrix2_t* m1 = Mat2_create(3,3);
    Mat2_arange(m1, 1, 9);

    matrix2_t* mv = Mat2_create(1,1);
    Mat2_slice_col_to(mv, m1, 0);

    MAT2_INSPECT(mv);

    vfloat_t* p;
    size_t rows, cols;
    __mat2_householder_matrix(&p, &rows, &cols, mv->pool, mv->rows);

    MAT2_RAW_INSPECT(p, rows, cols);

    matrix2_t pmat;
    pmat.pool = p;
    pmat.rows = rows;
    pmat.cols = cols;

    Mat2_dot(&pmat, m1);
    MAT2_INSPECT(&pmat);

    Mat2_destroy(m1);
    Mat2_destroy(mv);
    
}

static void test_matrix_qr(void) {

    vfloat_t a_data[][3] = {
        {6, -3, 5},
        {-1, 4, -5},
        {-3, 3, 4}
    };

    matrix2_t* a = Mat2_create(9,2);
    Mat2_arange(a, 1, 18);

    matrix2_t* q = Mat2_create(1,1);
    matrix2_t* r = Mat2_create(1,1);
    //matrix2_t* qr = Mat2_create(1,1);

    Mat2_qr(q, r, a);
    MAT2_INSPECT(r);
    MAT2_INSPECT(q);

    Mat2_dot(q, r);
    MAT2_INSPECT(q);


    Mat2_destroy(q);
    Mat2_destroy(r);
    Mat2_destroy(a);
    return;
}

static void test_matrix_qr_decompe(void) 
{
    matrix2_t* a = Mat2_create(10, 4);
    Mat2_arange(a, 1, 1000);
    //Mat2_fill(a, 100);

    MAT2_INSPECT(a);

    vfloat_t* q = NULL;
    size_t q_rows;
    size_t q_cols;

    vfloat_t* r = NULL;
    size_t r_rows;
    size_t r_cols;

    __mat2_qr_decomp(&q, &q_rows, &q_cols, &r, &r_rows, &r_cols, a->pool, a->rows, a->cols, 0, 3);

    MAT2_RAW_INSPECT(q, q_rows, q_cols);
    MAT2_RAW_INSPECT(r, r_rows, r_cols);

    return;

}


static void test_matrix_eigenvectors(void) {

    vfloat_t* values = NULL;

    matrix2_t* a = Mat2_create(9,9);
    //Mat2_load_on_shape(a, a_data, 3,3);
    Mat2_arange(a, 100+1, 100+81);

    __mat2_eigenvalues(&values, a->pool, a->rows);

    printf("\n eig value: \n");
    for (int i=0; i<a->rows; ++i) {

        printf(" %e ", values[i]);
    }
    printf("\n");
    vfloat_t* vector = NULL;
    __mat2_eigenvector(&vector, a->pool, values[0], a->rows);

    printf("\n vector: \n");
    for (int i=0; i<a->rows; ++i) {
        printf(" %e ", vector[i]);
    }

    matrix2_t* vec = Mat2_create(1,1);
    Mat2_load_on_shape(vec, vector, a->rows, 1);

    MAT2_INSPECT(vec);

    Mat2_dot(a, vec);

    MAT2_INSPECT(a);

    printf("\n");
    for (int i=0; i<a->rows; ++i) {
        printf("%e/ %e = %e \n", a->pool[i], vector[i], a->pool[i] / vector[i]);
    }
    
    free(values);
    free(vector);
    Mat2_destroy(a);


    return;
}

static void test_matrix_solve_lu(void) {


    vfloat_t m1[][2] = {
        {3, 6},
        {4, -2},
    };

    vfloat_t Y[2] = {12, 6};

    __mat2_solve(m1, Y, 2);

    //printf("%0.2f, %0.2f", Y[0], Y[1]);
    CU_ASSERT_DOUBLE_EQUAL(Y[0], 2.f, 0.00001);
    CU_ASSERT_DOUBLE_EQUAL(Y[1], 1.f, 0.00001);
}

static void test_matrix_eig(void) 
{
    matrix2_t* a = Mat2_create(1,1);

    // vfloat_t a_data[][3] = {
    //     {6, -3, 5},
    //     {-1,4,-5},
    //     {-3, 3, -4}
    // };

    vfloat_t a_data[][3] = {
        {6, -3, 5},
        {-3,4,-5},
        {5, -5, -4}
    };


    Mat2_load_on_shape(a, a_data, 3, 3);

    matrix2_t* eigvalues  = Mat2_create(1,1);
    matrix2_t* eigvectors = Mat2_create(1,1);

    matrix2_t* vector = Mat2_create(1,1);

    matrix2_t* vd     = Mat2_create(1,1);

    Mat2_cpy(vd, a);

    Mat2_eig(eigvalues, eigvectors, a);

    Mat2_slice_col_to(vector, eigvectors, 2);
    //Mat2_slice_row_to(vector, eigvectors, 0);

    Mat2_dot(vd, vector);

    MAT2_INSPECT(eigvalues);

    MAT2_INSPECT(eigvectors);

    MAT2_INSPECT(vector);

    printf("\n");
    for (int i=0; i<vd->rows; ++i) {
        printf("lamada[%d] : %f \n", i, vd->pool[i] / vector->pool[i]);
    }

    
    Mat2_destroy(eigvalues);
    Mat2_destroy(eigvectors);
    return;


    //11.3617 -7.2927 1.9310

    // 0.7076 0.2661 0.6546
    // -0.5710 -0.3303 0.7516
    // 0.4162 -0.9056 -0.0818
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

    // if (NULL == CU_add_test(pSuite, "test mat add sub mulity ", test_matrix_slice2) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    if (NULL == CU_add_test(pSuite, "test mat add sub mulity ", test_matrix_counting) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }


    // if (NULL == CU_add_test(pSuite, " test mat cofactor ", test_matrix_cofactor) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, " test mat det ", test_matrix_det) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, " test mat adjoint ", test_matrix_adjoint) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, " test mat adjoint ", test_matrix_inverse) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }


    // if (NULL == CU_add_test(pSuite, " test mat dot ", test_matrix_dot) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, " test mat T ", test_matrix_T) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }
    // if (NULL == CU_add_test(pSuite, " test mat T ", test_matrix_householder_transform) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, " test mat T ", test_matrix_qr) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, " test mat eigen valus ", test_matrix_eig) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }


    // if (NULL == CU_add_test(pSuite, " test mat eigen valus ", test_matrix_eigenvectors) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, " test mat eigen valus ", test_matrix_solve_lu) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, " test mat T ", test_matrix_qr_decompe) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }
}