/*
 * @Author: your name
 * @Date: 2020-10-23 13:29:43
 * @LastEditTime: 2021-10-24 09:22:02
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/matrix/DenseMatrix.h
 */
#ifndef __DENSE_MATRIX_H__
#define __DENSE_MATRIX_H__
#include "imatrix.h"

typedef struct _dense_matrix {
    imatrix_t matrix;
    vfloat_t* elems;
}DenseMatrix;

DenseMatrix* DenseMatrix_create(size_t x, size_t y);
DenseMatrix* DenseMatrix_load(size_t x, size_t y, vfloat_t* data);
DenseMatrix* DenseMatrix_wrap(size_t x, size_t y, vfloat_t* data);
DenseMatrix* DenseMatrix_copy(DenseMatrix* mat);

int DenseMatrix_destroy(DenseMatrix*);
DenseMatrix* DenseMatrix_dot(DenseMatrix* matrix_ptr1, DenseMatrix* matrix_ptr2, DenseMatrix* product);

int DenseMatrix_lu(DenseMatrix* matrix);
int DenseMatrix_solve(DenseMatrix* matrix, vfloat_t[], size_t);
int DenseMatrix_inverse(DenseMatrix* matrix);
int DenseMatrix_pseudo_inverse(DenseMatrix* matrix, DenseMatrix* pinv);
int DenseMatrix_plu(DenseMatrix* matrix, vfloat_t[]);


int DenseMatrix_diagonalization(DenseMatrix*);

#define DenseMatrix_elem_ptr(pm, ptr) vfloat_t(*ptr)[pm->matrix.cols] = pm->elems
#endif