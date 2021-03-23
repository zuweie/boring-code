/*
 * @Author: your name
 * @Date: 2020-10-23 13:29:43
 * @LastEditTime: 2021-03-23 17:00:29
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/matrix/DenseMatrix.h
 */
#ifndef _DENSE_MATRIX_H_
#define _DENSE_MATRIX_H_
#include "matrix.h"

typedef struct _dense_matrix {
    imatrix_t matrix;
    mx_float_t* elems;
}DenseMatrix;

DenseMatrix* DenseMatrix_create(size_t x, size_t y);
DenseMatrix* DenseMatrix_load(size_t x, size_t y, mx_float_t* data);
DenseMatrix* DenseMatrix_wrap(size_t x, size_t y, mx_float_t* data);
int DenseMatrix_foreach(DenseMatrix* m, void(*elem_func)(mx_float_t*, void*), void*);
int DenseMatrix_foreach_row(DenseMatrix* m, void(*row_func)(mx_float_t*, size_t, void*), void*);
int DenseMatrix_foreach_col(DenseMatrix* m, void(*col_func)(mx_float_t*, size_t, void*), void*);

int DenseMatrix_destroy(DenseMatrix*);
imatrix_t* DenseMatrix_product(imatrix_t* matrix_ptr1, imatrix_t* matrix_ptr2, imatrix_t* product);

#define DenseMatrix_elem_ptr(pm, ptr) mx_float_t(*ptr)[pm->matrix.cols] = pm->elems
#endif