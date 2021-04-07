/*
 * @Author: your name
 * @Date: 2021-04-05 14:51:16
 * @LastEditTime: 2021-04-07 10:47:37
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/matrix/matrix.h
 */
#ifndef _MATRIX_H_
#define _MATRIX_H_
#include <stdlib.h>

typedef struct _matrix
{
    /* data */
    float* elems;
    size_t rows;
    size_t cols;
    
} matrix_t;

matrix_t Mat_create(size_t rows, size_t cols);
matrix_t Mat_load(size_t rows, size_t cols, float* elems);
matrix_t Mat_copy(matrix_t* mat);
int Mat_destroy(matrix_t* mat);

int Mat_inverse(matrix_t* mat);
int Mat_pseudo_inverse(matrix_t* mat);
int Mat_solve(matrix_t* mat, float Y[]);
int Mat_lu(matrix_t* mat);
int Mat_dot(matrix_t* mat1, matrix_t* mat2);
int Mat_transpose(matrix_t* mat);
int Mat_copy_elems(matrix_t* mat, float buffer[]);
int Mat_get_row(matrix_t* mat, size_t row_index, float row[]);
int Mat_get_col(matrix_t* mat, size_t col_index, float col[]);
int Mat_move_rows(matrix_t* mat, int picked, int step);
int Mat_move_cols(matrix_t* mat, int picked, int step);
int Mat_insert_row_by_value(matrix_t* mat, size_t i, float v);
int Mat_insert_row_by_arr(matrix_t* mat, size_t i, float arr[]);
int Mat_insert_col_by_value(matrix_t* mat, size_t i, float v);
int Mat_insert_col_by_arr(matrix_t* mat, size_t i, float arr[]);
int Mat_fill(matrix_t* mat, float fill);
int Mat_arange(matrix_t* mat, float from, float to);

#define Mat_rows(pmat) ((pmat)->rows)
#define Mat_cols(pmat) ((pmat)->cols)
#define Mat_eptr(pmat, ptr) float (*ptr)[(pmat)->cols] = (pmat)->elems

#endif