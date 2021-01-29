/*
 * @Author: your name
 * @Date: 2020-10-23 13:29:43
 * @LastEditTime: 2021-01-29 12:19:19
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/matrix/DenseMatrix.c
 */
#include "string.h"
#include  "DenseMatrix.h"

static 
void get_col(imatrix_t* matrix_ptr, size_t col_index, mx_float_t data[])
{
    DenseMatrix* denseMatrix = (DenseMatrix*)matrix_ptr;
    mx_float_t (*src)[matrix_ptr->cols] = denseMatrix->elems;

    for(int i=0; i<matrix_ptr->rows; ++i) {
        data[i] = src[i][col_index];
    }
    return;
}

static 
void get_row(imatrix_t* matrix_ptr, size_t row_index, mx_float_t data[])
{
    DenseMatrix* denseMatrix = (DenseMatrix*)matrix_ptr;
    mx_float_t(*src)[matrix_ptr->cols] = denseMatrix->elems;
    memcpy(data, src[row_index], sizeof(mx_float_t) * matrix_ptr->cols);
    return;
}

static  
mx_float_t get(imatrix_t* matrix_ptr, size_t x, size_t y) 
{
    DenseMatrix* densematrix = (DenseMatrix*) matrix_ptr;
    return ((mx_float_t(*)[Matrix_cols(matrix_ptr)])(densematrix->elems))[x][y];
}

static  
int set(imatrix_t* matrix_ptr, size_t x, size_t y, mx_float_t v) 
{
    DenseMatrix* densematrix = (DenseMatrix*) matrix_ptr;
    ((mx_float_t(*)[Matrix_cols(matrix_ptr)])(densematrix->elems))[x][y] = v;
    return 0;
}
static  
int trans (imatrix_t* matrix_ptr) 
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

imatrix_t* DenseMatrix_product(imatrix_t* matrix_ptr1, imatrix_t* matrix_ptr2, imatrix_t* product) 
{  
    // 简单大循环，算法
    DenseMatrix* mp1 = matrix_ptr1;
    DenseMatrix* mp2 = matrix_ptr2;
    DenseMatrix* mp = product;

    mx_float_t (*elem1)[mp1->matrix.cols] = mp1->elems;
    mx_float_t (*elem2)[mp2->matrix.cols] = mp2->elems;
    mx_float_t (*elemp)[mp->matrix.cols]  = mp->elems;

    for (int i=0; i<Matrix_rows(mp1); ++i) {
        for (int j=0; j<Matrix_cols((mp2)); ++j){

            mx_float_t v = 0.0f;
            for (int k=0; k<Matrix_cols(mp1); ++k) {
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
    initialize_matrix(matrix, get, set, trans, get_row, get_col, row, col);
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
    initialize_matrix(matrix, get, set, trans, get_row, get_col, row, col);
    matrix->elems = data;
    return matrix;
}

int DenseMatrix_foreach(DenseMatrix* m, void(*elem_func)(mx_float_t*, void*), void* param) 
{
    
    DenseMatrix_elem_ptr(m, pelem);
    for (int i=0; i<m->matrix.rows; ++i) {
        for (int j=0; j<m->matrix.cols; ++j) {
            elem_func(&pelem[i][j], param);
        }
    }
    return 0;
}

int DenseMatrix_foreach_row(DenseMatrix* m, void(*row_func)(mx_float_t*, size_t, void*), void* param) 
{
    DenseMatrix_elem_ptr(m, pelem);
    for (int i=0; i<m->matrix.rows; ++i) {
        row_func(pelem[i], m->matrix.cols, param);
    }
    return 0;
}

int DenseMatrix_foreach_col(DenseMatrix* m, void(*col_func)(mx_float_t*, size_t, void*), void*param) 
{
    trans(m);
    DenseMatrix_elem_ptr(m, pelem);
    DenseMatrix_foreach_row(m, col_func, param);
    trans(m);
    return 0;
}

int DenseMatrix_destroy(DenseMatrix* matrix)
{
    free(matrix);
    return 0;
}

