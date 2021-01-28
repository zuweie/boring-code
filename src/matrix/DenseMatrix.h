/*
 * @Author: your name
 * @Date: 2020-10-23 13:29:43
 * @LastEditTime: 2021-01-28 14:48:09
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/matrix/DenseMatrix.h
 */
#ifndef _DENSE_MATRIX_H_
#define _DENSE_MATRIX_H_
#include "matrix.h"

typedef struct _dense_matrix {
    imatrix_t matrix;
    float* elems;

}DenseMatrix;

DenseMatrix* DenseMatrix_create(size_t x, size_t y);
DenseMatrix* DenseMatrix_load(size_t x, size_t y, mx_float_t* data);
DenseMatrix* DenseMatrix_set(size_t x, size_t y, mx_float_t* data);
int DenseMatrix_destroy(DenseMatrix*);

#endif