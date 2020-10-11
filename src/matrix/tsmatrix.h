/*
 * @Author: your name
 * @Date: 2020-06-13 10:13:53
 * @LastEditTime: 2020-06-14 01:10:40
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /kitc/src/matrix/tsmatrix.h
 */ 
#ifndef _TS_MATRIX_H_
#define _TS_MATRIX_H_

#include "container/link_array.h"

typedef struct _ts_coord {

    size_t x;
    size_t y;

} ts_coord;

typedef struct _ts_elem {
    ts_coord position;
    float value;
} ts_elem;

typedef struct _ts_matrix{

    size_t row;
    size_t col;
    LinkArray elems;

} TSMatrix;

typedef struct _matrix Matrix;

TSMatrix* TSMatrix_create(size_t, size_t);
TSMatrix* TSMatrix_create_by(size_t, size_t, float data[*][*]);
TSMatrix* TSMatrix_create_transpose(TSMatrix*);

Matrix* TSMatrix_create_matrix(TSMatrix*);

int TSMatrix_destroy(TSMatrix*);
float TSMatrix_get(TSMatrix*, int, int);
int TSMatrix_set(TSMatrix*, int, int, float);

#endif