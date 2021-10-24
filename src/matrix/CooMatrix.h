/*
 * @Author: your name
 * @Date: 2020-10-22 13:30:48
 * @LastEditTime: 2021-10-24 09:21:52
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/matrix/CooMatrix.h
 */
#ifndef __COO_MATRIX_H__
#define __COO_MATRIX_H__

#include "imatrix.h"
#include "container/cn.h"

typedef struct _coo_matrix {
    imatrix_t matrix;
    CN coo;
}CooMatrix;

CooMatrix* CooMatrix_create(size_t x, size_t y);
CooMatrix* CooMatrix_load(size_t x, size_t y, vfloat_t* data);
int CooMatrix_destroy(CooMatrix*);

#endif

