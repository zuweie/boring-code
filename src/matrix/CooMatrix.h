/*
 * @Author: your name
 * @Date: 2020-10-22 13:30:48
 * @LastEditTime: 2020-10-22 16:08:57
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/matrix/CooMatrix.h
 */
#ifndef _COO_MATRIX_H_
#define _COO_MATRIX_H_

#include "__matrix.h"
#include "container/Hashmap.h"

#define COOHASH(x, y) \
    ({size_t sum = x + y; sum = sum * (sum + 1) / 2 + x;})

typedef struct _coo_matrix {
    imatrix_t matrix;
    Hashmap matrix_table;
    size_t x;
    size_t y;
}CooMatrix;

CooMatrix* CooMatrix_create(size_t x, size_t y);
CooMatrix* CooMatrix_load(size_t x, size_t y, float* data);
int CooMatrix_destroy(CooMatrix*);

#endif

