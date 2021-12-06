/*
 * @Author: your name
 * @Date: 2021-04-05 14:51:16
 * @LastEditTime: 2021-12-05 09:29:16
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/matrix/matrix.h
 */
#ifndef __MATRIX_H__
#define __MATRIX_H__
#include <stdlib.h>
#include "vtype/vfloat_type.h"
 
#define mat_op(v1, v2, op) (op == op_add ? ((v1) += (v2)) : (op == op_sub ? ((v1) -= (v2)) : (op == op_multi? ((v1) *= (v2)) : ((v1) /= (v2)))))

typedef struct _matrix
{
    /* data */
    vfloat_t* pool;
    size_t rows;
    size_t cols;
    size_t pool_size;
    
} matrix_t;

typedef enum {
    op_add = 1,
    op_sub,
    op_multi, 
    op_div
} mat_op_t;

typedef enum {
    dimen_row = 1,
    dimen_col
} mat_dimen_t;
matrix_t Mat_create(size_t rows, size_t cols);
matrix_t Mat_load(size_t rows, size_t cols, vfloat_t* elems);
matrix_t Mat_copy(matrix_t* mat);
void Mat_set(matrix_t* mat, size_t rows, size_t cols, vfloat_t* pool);
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
int Mat_reshape(matrix_t* mat, size_t new_rows, size_t new_cols);
int Mat_reload(matrix_t* mat, size_t new_rows, size_t new_cols, vfloat_t* data);
int Mat_save(matrix_t* mat, void* buf);
int Mat_op_mat(matrix_t* mat1, matrix_t* mat2, mat_op_t op);
int Mat_op_numberic(matrix_t* mat, vfloat_t v, mat_op_t op);
int Mat_deflate(matrix_t* mat, mat_dimen_t dimen, mat_op_t op);

#define Mat_rows(pmat) ((pmat)->rows)
#define Mat_cols(pmat) ((pmat)->cols)
#define Mat_eptr(pmat, ptr) vfloat_t(*ptr)[(pmat)->cols] = (pmat)->pool
#define Mat_put(pmat, x, y, v) ((pmat)->pool[(x)*(pmat)->cols + y] = (v))//({ Mat_eptr(pmat, ptr); ptr[x][y] = v; })

#endif