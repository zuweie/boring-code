/*
 * @Author: your name
 * @Date: 2021-04-05 14:51:16
 * @LastEditTime: 2025-06-03 13:25:48
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/matrix/matrix.h
 */
#ifndef __MATRIX_H__
#define __MATRIX_H__
#include <stdlib.h>
#include "vtype/vfloat_type.h"
 
#define mat_op(v1, v2, op) ((op) == op_add ? ((v1) += (v2)) : ((op) == op_sub ? ((v1) -= (v2)) : ((op) == op_multi? ((v1) *= (v2)) : ((v1) /= (v2)))))

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
matrix_t Mat_load(size_t rows, size_t cols, vfloat_t elems[]);
matrix_t Mat_create_cpy(matrix_t* mat);
void Mat_set(matrix_t* mat, size_t rows, size_t cols, vfloat_t* pool);
int Mat_destroy(matrix_t* mat);

int Mat_inverse(matrix_t* mat);
int Mat_pseudo_inverse(matrix_t* mat);
int Mat_solve(matrix_t* mat, vfloat_t Y[], size_t n);
int Mat_lu(matrix_t* mat);
int Mat_dot(matrix_t* mat1, matrix_t* mat2);
int Mat_transpose(matrix_t* mat);
int Mat_export(matrix_t* mat, vfloat_t buffer[]);
int Mat_get_row(matrix_t* mat, size_t row_index, vfloat_t row[]);
int Mat_get_col(matrix_t* mat, size_t col_index, vfloat_t col[]);
int Mat_move_rows(matrix_t* mat, int picked, int step);
int Mat_move_cols(matrix_t* mat, int picked, int step);
int Mat_insert_row_by_value(matrix_t* mat, int i, vfloat_t v);
int Mat_insert_row_by_arr(matrix_t* mat, int i, vfloat_t arr[]);
int Mat_insert_col_by_value(matrix_t* mat, int i, vfloat_t v);
int Mat_insert_col_by_arr(matrix_t* mat, int i, vfloat_t arr[]);
//int Mat_rescale(matrix_t* mat, int left, int right, int up, int down, vfloat_t fill);
int Mat_rescale(matrix_t* mat, int left, int top, int right, int bottom, vfloat_t fill);
int Mat_fill(matrix_t* mat, vfloat_t fill);
int Mat_arange(matrix_t* mat, vfloat_t from, vfloat_t to);
int Mat_reshape(matrix_t* mat, size_t new_rows, size_t new_cols);
int Mat_reload(matrix_t* mat, size_t new_rows, size_t new_cols, vfloat_t* data);
int Mat_save(matrix_t* mat, void* buf);
int Mat_op_mat(matrix_t* mat1, matrix_t* mat2, mat_op_t op);
int Mat_op_numberic(matrix_t* mat, vfloat_t v, mat_op_t op);
int Mat_deflate(matrix_t* mat, mat_dimen_t dimen, mat_op_t op);
int Mat_vector_dot(matrix_t* m1, matrix_t* m2, vfloat_t* out);
int Mat_copy(matrix_t* dest, matrix_t* src);

#define Mat_rows(pmat) ((pmat)->rows)
#define Mat_cols(pmat) ((pmat)->cols)
#define Mat_eptr(pmat, ptr) vfloat_t(*ptr)[(pmat)->cols] = (pmat)->pool
#define Mat_put(pmat, x, y, v) ((pmat)->pool[(x)*(pmat)->cols + y] = (v))//({ Mat_eptr(pmat, ptr); ptr[x][y] = v; })
#define Mat_inspect(pmat) \
do{ \
    for (int i=0; i<(pmat)->rows; ++i) { \
        for (int j=0; j<(pmat)->cols; ++j) { \
            printf("%f ", (pmat)->pool[i*(pmat)->cols + j]); \
        } \
        printf("\n");\
    } \
} while(0)

#endif