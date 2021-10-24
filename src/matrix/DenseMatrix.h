/*
 * @Author: your name
 * @Date: 2020-10-23 13:29:43
 * @LastEditTime: 2021-10-24 10:54:52
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

DenseMatrix* DenseMatrix_create(unsigned int x, unsigned int y);
DenseMatrix* DenseMatrix_load(unsigned int x, unsigned int y, vfloat_t* data);
DenseMatrix* DenseMatrix_wrap(unsigned int x, unsigned int y, vfloat_t* data);
DenseMatrix* DenseMatrix_copy(DenseMatrix* mat);

int DenseMatrix_destroy(DenseMatrix*);
DenseMatrix* DenseMatrix_dot(DenseMatrix* matrix_ptr1, DenseMatrix* matrix_ptr2, DenseMatrix* product);

int DenseMatrix_lu(DenseMatrix* matrix);
int DenseMatrix_solve(DenseMatrix* matrix, vfloat_t[], unsigned int);
int DenseMatrix_inverse(DenseMatrix* matrix);
int DenseMatrix_pseudo_inverse(DenseMatrix* matrix, DenseMatrix* pinv);
int DenseMatrix_plu(DenseMatrix* matrix, vfloat_t[]);


int DenseMatrix_diagonalization(DenseMatrix*);

#define DenseMatrix_elem_ptr(pm, ptr) vfloat_t(*ptr)[pm->matrix.cols] = pm->elems
#endif