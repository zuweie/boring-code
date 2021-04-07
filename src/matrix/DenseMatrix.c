/*
 * @Author: your name
 * @Date: 2020-10-23 13:29:43
 * @LastEditTime: 2021-04-05 14:46:57
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/matrix/DenseMatrix.c
 */
#include "string.h"
#include  "DenseMatrix.h"

static 
void __get_col(imatrix_t* matrix_ptr, size_t col_index, mx_float_t data[])
{
    DenseMatrix* denseMatrix = (DenseMatrix*)matrix_ptr;
    mx_float_t (*src)[matrix_ptr->cols] = denseMatrix->elems;

    for(size_t i=0; i<matrix_ptr->rows; ++i) {
        data[i] = src[i][col_index];
    }
    return;
}

static 
void __get_row(imatrix_t* matrix_ptr, size_t row_index, mx_float_t data[])
{
    DenseMatrix* denseMatrix = (DenseMatrix*)matrix_ptr;
    mx_float_t(*src)[matrix_ptr->cols] = denseMatrix->elems;
    memcpy(data, src[row_index], sizeof(mx_float_t) * matrix_ptr->cols);
    return;
}

static  
mx_float_t __get(imatrix_t* matrix_ptr, size_t x, size_t y) 
{
    DenseMatrix* densematrix = (DenseMatrix*) matrix_ptr;
    return ((mx_float_t(*)[Matrix_cols(matrix_ptr)])(densematrix->elems))[x][y];
}

static  
int __set(imatrix_t* matrix_ptr, size_t x, size_t y, mx_float_t v) 
{
    DenseMatrix* densematrix = (DenseMatrix*) matrix_ptr;
    ((mx_float_t(*)[Matrix_cols(matrix_ptr)])(densematrix->elems))[x][y] = v;
    return 0;
}
static  
int __trans (imatrix_t* matrix_ptr) 
{
    DenseMatrix* denmatrix = (DenseMatrix*) matrix_ptr;
    mx_float_t tmp_data[matrix_ptr->cols][matrix_ptr->rows];

    for(int i=0; i<matrix_ptr->cols; ++i) {
        Matrix_get_col(denmatrix, i, tmp_data[i]);
    }
    memcpy(denmatrix->elems, tmp_data, sizeof(mx_float_t)*matrix_ptr->cols*matrix_ptr->rows);
    size_t o_cols = matrix_ptr->cols;
    matrix_ptr->cols = matrix_ptr->rows;
    matrix_ptr->rows = o_cols;
    return 0;
}

int __solve_l(DenseMatrix* lu, mx_float_t* Z, size_t n)
{
    DenseMatrix_elem_ptr(lu, lu_ptr);

    for (size_t i=1; i<n; ++i) {
        for (size_t j=0; j<i; ++j) {
            mx_float_t debug = lu_ptr[i][j];
            Z[i] = Z[i] - lu_ptr[i][j] * Z[j];
        }
    }
    return 0;
}

int __solve_u(DenseMatrix* lu, mx_float_t* X, size_t n)
{   
    DenseMatrix_elem_ptr(lu, lu_ptr);
    
    for (size_t k=0; k<n; ++k ) {
        size_t i = n-k-1;
        for (size_t j=i+1; j<n; ++j) {
            mx_float_t d1 = lu_ptr[i][j];
            X[i] = X[i] - lu_ptr[i][j] * X[j];
        }
        mx_float_t d2 = lu_ptr[i][i];
        X[i] = X[i] / lu_ptr[i][i];
    }
    return 0;
}

int __solve_lu(DenseMatrix *lu, mx_float_t* Y, size_t n) 
{
    __solve_l(lu, Y, n);
    __solve_u(lu, Y, n);
    return 0;
}



DenseMatrix* DenseMatrix_dot(DenseMatrix* matrix_ptr1, DenseMatrix* matrix_ptr2, DenseMatrix* product) 
{  
    // 简单大循环，算法
    DenseMatrix* mp1 = matrix_ptr1;
    DenseMatrix* mp2 = matrix_ptr2;
    DenseMatrix* mp = product;

    mx_float_t (*elem1)[mp1->matrix.cols] = mp1->elems;
    mx_float_t (*elem2)[mp2->matrix.cols] = mp2->elems;
    mx_float_t (*elemp)[mp->matrix.cols]  = mp->elems;

    for (size_t i=0; i<Matrix_rows(mp1); ++i) {
        for (size_t j=0; j<Matrix_cols((mp2)); ++j){

            mx_float_t v = 0.0f;
            for (size_t k=0; k<Matrix_cols(mp1); ++k) {
                v += elem1[i][k] * elem2[k][j];
            }
            elemp[i][j] = v;
        }
    }
    return mp;
}

DenseMatrix* DenseMatrix_create(size_t row, size_t col)
{
    DenseMatrix* matrix = malloc(sizeof(DenseMatrix) + sizeof(mx_float_t)*row*col);
    initialize_matrix(matrix, __get, __set, __trans, __get_row, __get_col, row, col);
    matrix->elems = &matrix[1];
    return matrix;
}

DenseMatrix* DenseMatrix_load(size_t row, size_t col, mx_float_t* data)
{
    DenseMatrix* matrix = DenseMatrix_create(row, col);
    memcpy(matrix->elems, data, sizeof(mx_float_t) * row * col);
    return matrix;
}

DenseMatrix* DenseMatrix_wrap(size_t row, size_t col, mx_float_t* data) 
{
    DenseMatrix* matrix = malloc(sizeof(DenseMatrix));
    initialize_matrix(matrix, __get, __set, __trans, __get_row, __get_col, row, col);
    matrix->elems = data;
    return matrix;
}

DenseMatrix* DenseMatrix_copy(DenseMatrix* mat) 
{
    size_t row, col;
    row = Matrix_rows(mat);
    col = Matrix_cols(mat);
    DenseMatrix_elem_ptr(mat, ptr);

    return DenseMatrix_load(row, col, ptr);
}

int DenseMatrix_lu(DenseMatrix* matrix)
{
    size_t rows = Matrix_rows(matrix);
    size_t cols = Matrix_cols(matrix);

    size_t s = rows < cols ? rows : cols;

    DenseMatrix_elem_ptr(matrix, mat_ptr);

    for (size_t k=0; k<s; ++k) {
        mx_float_t x = 1.0f / mat_ptr[k][k];
        
        for (size_t i=k+1; i<rows; ++i) {
            mat_ptr[i][k] = mat_ptr[i][k] * x;
        }

        for (size_t i=k+1; i<rows; ++i) {
            for (size_t j=k+1; j<cols; ++j) {
                mat_ptr[i][j] = mat_ptr[i][j] - mat_ptr[i][k] * mat_ptr[k][j];
            }
        }
    }
    return 0;
}

/**
 * 解线性方程
 * matrix 是包含系数的方阵
 * Y 是解
 */
int DenseMatrix_solve(DenseMatrix* matrix, mx_float_t Y[], size_t n) 
{
    DenseMatrix_lu(matrix);
    return __solve_lu(matrix, Y, n);
}


int DenseMatrix_inverse(DenseMatrix* matrix) 
{
    size_t rows = Matrix_rows(matrix);
    size_t cols = Matrix_cols(matrix);

    mx_float_t inverse[rows][cols];
    DenseMatrix_lu(matrix);

    for (size_t i=0; i<rows; ++i) {

        for (size_t j=0; j<cols; ++j) {
            if (i == j) inverse[i][j] = 1.f;
            else inverse[i][j] = 0.f;
        }
        
        __solve_lu(matrix, inverse[i], cols);
    }
    // 计算完结果将其覆盖到原来的矩阵当中去
    memcpy(matrix->elems, inverse, rows * cols * sizeof(mx_float_t));
    // 必须做转置才能还原逆矩阵
    Matrix_trans(matrix);
    return 0;
}

int DenseMatrix_pseudo_inverse(DenseMatrix* mat, DenseMatrix* pinv) 
{
    DenseMatrix* __mat_T = DenseMatrix_copy(mat);
    Matrix_trans(__mat_T);

    DenseMatrix* __mat_dot_matT = DenseMatrix_create(Matrix_rows(__mat_T), Matrix_cols(mat));
    DenseMatrix_dot(__mat_T, mat, __mat_dot_matT);
    DenseMatrix_inverse(__mat_dot_matT);

    DenseMatrix_dot(__mat_dot_matT, __mat_T, pinv);

    DenseMatrix_destroy(__mat_T);
    DenseMatrix_destroy(__mat_dot_matT);
    return 0;
}


int DenseMatrix_plu(DenseMatrix* matrix, mx_float_t P[])
{
    return 0;
}

int DenseMatrix_destroy(DenseMatrix* matrix)
{
    free(matrix);
    return 0;
}

