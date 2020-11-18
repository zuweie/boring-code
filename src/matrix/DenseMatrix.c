/*
 * @Author: your name
 * @Date: 2020-10-23 13:29:43
 * @LastEditTime: 2020-11-17 13:30:46
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/matrix/DenseMatrix.c
 */
#include "string.h"
#include  "DenseMatrix.h"

static 
void get_col(imatrix_t* matrix_ptr, size_t col_index, float data[])
{
    DenseMatrix* denseMatrix = (DenseMatrix*)matrix_ptr;
    float (*src)[matrix_ptr->cols] = denseMatrix->elems;

    for(int i=0; i<matrix_ptr->rows; ++i) {
        data[i] = src[i][col_index];
    }
    return;
}

static 
void get_row(imatrix_t* matrix_ptr, size_t row_index, float data[])
{
    DenseMatrix* denseMatrix = (DenseMatrix*)matrix_ptr;
    float(*src)[matrix_ptr->cols] = denseMatrix->elems;
    memcpy(data, src[row_index], sizeof(float) * matrix_ptr->cols);
    return;
}

static  
float get(imatrix_t* matrix_ptr, size_t x, size_t y) 
{
    DenseMatrix* densematrix = (DenseMatrix*) matrix_ptr;
    return ((float(*)[Matrix_cols(matrix_ptr)])(densematrix->elems))[x][y];
}

static  
int set(imatrix_t* matrix_ptr, size_t x, size_t y, float v) 
{
    DenseMatrix* densematrix = (DenseMatrix*) matrix_ptr;
    ((float(*)[Matrix_cols(matrix_ptr)])(densematrix->elems))[x][y] = v;
    return 0;
}
static  
int trans (imatrix_t* matrix_ptr) 
{
    DenseMatrix* denmatrix = (DenseMatrix*) matrix_ptr;
    float tmp_data[matrix_ptr->cols][matrix_ptr->rows];

    for(int i=0; i<matrix_ptr->cols; ++i) {
        Matrix_get_col(denmatrix, i, tmp_data[i]);
    }
    memcpy(denmatrix->elems, tmp_data, sizeof(float)*matrix_ptr->cols*matrix_ptr->rows);
    size_t o_cols = matrix_ptr->cols;
    matrix_ptr->cols = matrix_ptr->rows;
    matrix_ptr->rows = o_cols;
    return 0;
}
static 
imatrix_t* product(imatrix_t* matrix_ptr1, imatrix_t* matrix_ptr2, imatrix_t* product) 
{  
    // 简单大循环，算法
    for (int i=0; i<Matrix_rows(matrix_ptr1); ++i) {
        for (int j=0; j<Matrix_cols((matrix_ptr2)); ++j){

            float v = 0.0f;
            for (int k=0; k<Matrix_cols(matrix_ptr1); ++k) {
                 v += Matrix_get(matrix_ptr1, i, k) * Matrix_get(matrix_ptr2, k, j);
            }
            Matrix_set(product, i, j, v);
        }
    }
    return product;
}

DenseMatrix* DenseMatrix_create(size_t row, size_t col)
{
    DenseMatrix* matrix = malloc(sizeof(DenseMatrix) + sizeof(float)*row*col);
    initialize_matrix(matrix, get, set, trans, get_row, get_col, row, col);
    return matrix;
}

DenseMatrix* DenseMatrix_load(size_t row, size_t col, float* data)
{
    DenseMatrix* matrix = DenseMatrix_create(row, col);
    memcpy(matrix->elems, data, sizeof(float) * row * col);
    return matrix;
}

int DenseMatrix_destroy(DenseMatrix* matrix)
{
    free(matrix);
    return 0;
}

