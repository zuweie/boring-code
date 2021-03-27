/*
 * @Author: your name
 * @Date: 2020-10-23 13:29:43
 * @LastEditTime: 2021-03-27 10:44:41
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

int DenseMatrix_destroy(DenseMatrix*);
DenseMatrix* DenseMatrix_dot(DenseMatrix* matrix_ptr1, DenseMatrix* matrix_ptr2, DenseMatrix* product);


int DenseMatrix_lu(DenseMatrix* matrix);
int DenseMatrix_solve(DenseMatrix* matrix, mx_float_t[], size_t);
int DenseMatrix_plu(DenseMatrix* matrix, mx_float_t[]);


int DenseMatrix_diagonalization(DenseMatrix*);

#define DenseMatrix_elem_ptr(pm, ptr) mx_float_t(*ptr)[pm->matrix.cols] = pm->elems
#endif