/*
 * @Author: your name
 * @Date: 2020-10-23 13:29:43
 * @LastEditTime: 2021-10-24 10:56:24
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/matrix/DenseMatrix.c
 */
#include "string.h"
#include  "DenseMatrix.h"

static 
void __get_col(imatrix_t* matrix_ptr, unsigned int col_index, vfloat_t data[])
{
    DenseMatrix* denseMatrix = (DenseMatrix*)matrix_ptr;
    vfloat_t (*src)[matrix_ptr->cols] = denseMatrix->elems;

    for(unsigned int i=0; i<matrix_ptr->rows; ++i) {
        data[i] = src[i][col_index];
    }
    return;
}

static 
void __get_row(imatrix_t* matrix_ptr, unsigned int row_index, vfloat_t data[])
{
    DenseMatrix* denseMatrix = (DenseMatrix*)matrix_ptr;
    vfloat_t(*src)[matrix_ptr->cols] = denseMatrix->elems;
    memcpy(data, src[row_index], sizeof(vfloat_t) * matrix_ptr->cols);
    return;
}

static  
vfloat_t __get(imatrix_t* matrix_ptr, unsigned int x, unsigned int y) 
{
    DenseMatrix* densematrix = (DenseMatrix*) matrix_ptr;
    return ((vfloat_t(*)[Matrix_cols(matrix_ptr)])(densematrix->elems))[x][y];
}

static  
int __set(imatrix_t* matrix_ptr, unsigned int x, unsigned int y, vfloat_t v) 
{
    DenseMatrix* densematrix = (DenseMatrix*) matrix_ptr;
    ((vfloat_t(*)[Matrix_cols(matrix_ptr)])(densematrix->elems))[x][y] = v;
    return 0;
}
static  
int __trans (imatrix_t* matrix_ptr) 
{
    DenseMatrix* denmatrix = (DenseMatrix*) matrix_ptr;
    vfloat_t tmp_data[matrix_ptr->cols][matrix_ptr->rows];

    for(int i=0; i<matrix_ptr->cols; ++i) {
        Matrix_get_col(denmatrix, i, tmp_data[i]);
    }
    memcpy(denmatrix->elems, tmp_data, sizeof(vfloat_t)*matrix_ptr->cols*matrix_ptr->rows);
    unsigned int o_cols = matrix_ptr->cols;
    matrix_ptr->cols = matrix_ptr->rows;
    matrix_ptr->rows = o_cols;
    return 0;
}

int __solve_l(DenseMatrix* lu, vfloat_t* Z, unsigned int n)
{
    DenseMatrix_elem_ptr(lu, lu_ptr);

    for (unsigned int i=1; i<n; ++i) {
        for (unsigned int j=0; j<i; ++j) {
            vfloat_t debug = lu_ptr[i][j];
            Z[i] = Z[i] - lu_ptr[i][j] * Z[j];
        }
    }
    return 0;
}

int __solve_u(DenseMatrix* lu, vfloat_t* X, unsigned int n)
{   
    DenseMatrix_elem_ptr(lu, lu_ptr);
    
    for (unsigned int k=0; k<n; ++k ) {
        unsigned int i = n-k-1;
        for (unsigned int j=i+1; j<n; ++j) {
            vfloat_t d1 = lu_ptr[i][j];
            X[i] = X[i] - lu_ptr[i][j] * X[j];
        }
        vfloat_t d2 = lu_ptr[i][i];
        X[i] = X[i] / lu_ptr[i][i];
    }
    return 0;
}

int __solve_lu(DenseMatrix *lu, vfloat_t* Y, unsigned int n) 
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

    vfloat_t (*elem1)[mp1->matrix.cols] = mp1->elems;
    vfloat_t (*elem2)[mp2->matrix.cols] = mp2->elems;
    vfloat_t (*elemp)[mp->matrix.cols]  = mp->elems;

    for (unsigned int i=0; i<Matrix_rows(mp1); ++i) {
        for (unsigned int j=0; j<Matrix_cols((mp2)); ++j){

            vfloat_t v = 0.0f;
            for (unsigned int k=0; k<Matrix_cols(mp1); ++k) {
                v += elem1[i][k] * elem2[k][j];
            }
            elemp[i][j] = v;
        }
    }
    return mp;
}

DenseMatrix* DenseMatrix_create(unsigned int row, unsigned int col)
{
    DenseMatrix* matrix = malloc(sizeof(DenseMatrix) + sizeof(vfloat_t)*row*col);
    initialize_matrix(matrix, __get, __set, __trans, __get_row, __get_col, row, col);
    matrix->elems = &matrix[1];
    return matrix;
}

DenseMatrix* DenseMatrix_load(unsigned int row, unsigned int col, vfloat_t* data)
{
    DenseMatrix* matrix = DenseMatrix_create(row, col);
    memcpy(matrix->elems, data, sizeof(vfloat_t) * row * col);
    return matrix;
}

DenseMatrix* DenseMatrix_wrap(unsigned int row, unsigned int col, vfloat_t* data) 
{
    DenseMatrix* matrix = malloc(sizeof(DenseMatrix));
    initialize_matrix(matrix, __get, __set, __trans, __get_row, __get_col, row, col);
    matrix->elems = data;
    return matrix;
}

DenseMatrix* DenseMatrix_copy(DenseMatrix* mat) 
{
    unsigned int row, col;
    row = Matrix_rows(mat);
    col = Matrix_cols(mat);
    DenseMatrix_elem_ptr(mat, ptr);

    return DenseMatrix_load(row, col, ptr);
}

int DenseMatrix_lu(DenseMatrix* matrix)
{
    unsigned int rows = Matrix_rows(matrix);
    unsigned int cols = Matrix_cols(matrix);

    unsigned int s = rows < cols ? rows : cols;

    DenseMatrix_elem_ptr(matrix, mat_ptr);

    for (unsigned int k=0; k<s; ++k) {
        vfloat_t x = 1.0f / mat_ptr[k][k];
        
        for (unsigned int i=k+1; i<rows; ++i) {
            mat_ptr[i][k] = mat_ptr[i][k] * x;
        }

        for (unsigned int i=k+1; i<rows; ++i) {
            for (unsigned int j=k+1; j<cols; ++j) {
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
int DenseMatrix_solve(DenseMatrix* matrix, vfloat_t Y[], unsigned int n) 
{
    DenseMatrix_lu(matrix);
    return __solve_lu(matrix, Y, n);
}


int DenseMatrix_inverse(DenseMatrix* matrix) 
{
    unsigned int rows = Matrix_rows(matrix);
    unsigned int cols = Matrix_cols(matrix);

    vfloat_t inverse[rows][cols];
    DenseMatrix_lu(matrix);

    for (unsigned int i=0; i<rows; ++i) {

        for (unsigned int j=0; j<cols; ++j) {
            if (i == j) inverse[i][j] = 1.f;
            else inverse[i][j] = 0.f;
        }
        
        __solve_lu(matrix, inverse[i], cols);
    }
    // 计算完结果将其覆盖到原来的矩阵当中去
    memcpy(matrix->elems, inverse, rows * cols * sizeof(vfloat_t));
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


int DenseMatrix_plu(DenseMatrix* matrix, vfloat_t P[])
{
    return 0;
}

int DenseMatrix_destroy(DenseMatrix* matrix)
{
    free(matrix);
    return 0;
}

