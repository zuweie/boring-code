/*
 * @Author: your name
 * @Date: 2021-04-05 14:51:16
 * @LastEditTime: 2021-04-09 09:30:48
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/matrix/matrix.h
 */
#ifndef _MATRIX_H_
#define _MATRIX_H_
#include <stdlib.h>
#include "vtype/vfloat_type.h"

typedef struct _matrix
{
    /* data */
    vfloat_t* elems;
    size_t rows;
    size_t cols;
    size_t pool_size;
    
} matrix_t;

matrix_t Mat_create(size_t rows, size_t cols);
matrix_t Mat_load(size_t rows, size_t cols, vfloat_t* elems);
matrix_t Mat_copy(matrix_t* mat);
int Mat_destroy(matrix_t* mat);

int Mat_inverse(matrix_t* mat);
int Mat_pseudo_inverse(matrix_t* mat);
int Mat_solve(matrix_t* mat, vfloat_t Y[], size_t n);
int Mat_lu(matrix_t* mat);
int Mat_dot(matrix_t* mat1, matrix_t* mat2);
int Mat_transpose(matrix_t* mat);
int Mat_copy_elems(matrix_t* mat, vfloat_t buffer[]);
int Mat_get_row(matrix_t* mat, size_t row_index, vfloat_t row[]);
int Mat_get_col(matrix_t* mat, size_t col_index, vfloat_t col[]);
int Mat_move_rows(matrix_t* mat, int picked, int step);
int Mat_move_cols(matrix_t* mat, int picked, int step);
int Mat_insert_row_by_value(matrix_t* mat, int i, vfloat_t v);
int Mat_insert_row_by_arr(matrix_t* mat, int i, vfloat_t arr[]);
int Mat_insert_col_by_value(matrix_t* mat, int i, vfloat_t v);
int Mat_insert_col_by_arr(matrix_t* mat, int i, vfloat_t arr[]);
int Mat_fill(matrix_t* mat, vfloat_t fill);
int Mat_arange(matrix_t* mat, vfloat_t from, vfloat_t to);

#define Mat_rows(pmat) ((pmat)->rows)
#define Mat_cols(pmat) ((pmat)->cols)
#define Mat_eptr(pmat, ptr) vfloat_t(*ptr)[(pmat)->cols] = (pmat)->elems

#endif