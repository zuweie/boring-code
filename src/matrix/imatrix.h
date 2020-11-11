/*
 * @Author: your name
 * @Date: 2020-10-20 11:04:24
 * @LastEditTime: 2020-11-11 15:14:28
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/matrix/__matrix.h
 */
#ifndef _MATRIX_INTERFACE_H_
#define _MATRIX_INTERFACE_H_

#include <stdlib.h>

typedef struct _imatrix imatrix_t;
struct _imatrix {
    float (*get)(imatrix_t*, size_t row, size_t col);
    int   (*set)(imatrix_t*, size_t row, size_t col, float);
};

// #define Matrix_create(label, ...) label##_create(##__VAR_ARGS__);
// #define Martix_load(lable, ...) label##_load(##__VAR_ARGS__);
// #define Matrix_destroy(label, ...) label##_destroy(##__VAR_ARGS__);

#define initialize_matrix(matrix, _get, _set) do {     \
    ((imatrix_t*)(matrix))->get     = (_get);          \
    ((imatrix_t*)(matrix))->set     = (_set);          \
} while (0)

#define Matrix_set(matrix_ptr, row, col, value) ((imatrix_t*)(matrix_ptr))->set(((imatrix_t*)(matrix_ptr)), row, col, value)
#define Matrix_get(matrix_ptr, row, col) ((imatrix_t*)(matrix_ptr))->get(((imatrix_t*)(matrix_ptr)),row, col)
#endif