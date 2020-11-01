/*
 * @Author: your name
 * @Date: 2020-10-20 11:04:24
 * @LastEditTime: 2020-10-30 09:01:11
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/matrix/__matrix.h
 */
#ifndef _MATRIX_INTERFACE_H_
#define _MATRIX_INTERFACE_H_

#include <stdlib.h>

typedef struct _imatrix imatrix_t;
struct _imatrix {
    float (*get)(size_t x, size_t y);
    int (*set)(size_t x, size_t y, float);
};

#define Matrix_create(label, ...) label##_create(##__VAR_ARGS__);
#define Martix_load(lable, ...) label##_load(##__VAR_ARGS__);
#define Matrix_destroy(label, ...) label##_destroy(##__VAR_ARGS__);

#define initialize_matrix(matrix, get, set) do {     \
    ((imatrix_t*)(matrix))->get     = (get);         \
    ((imatrix_t*)(matrix))->set     = (set);         \
} while (0)

#endif