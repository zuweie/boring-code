/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-06-15 16:10:10
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-12-15 12:50:09
 * @FilePath: /boring-code/src/statistical_learning/matrix2.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __MATRIX2_H__
#define __MATRIX2_H__
#include <stdlib.h>
#include "matrix2_operator.h"
#include "vtype/vfloat_type.h"

typedef struct {

    vfloat_t* pool;
    size_t rows;
    size_t cols;

} matrix2_t;

matrix2_t* Mat2_create(size_t rows, size_t cols);
matrix2_t* Mat2_create_cpy(matrix2_t* mat);
int Mat2_destroy(matrix2_t* mat);

int Mat2_arange(matrix2_t* mat, vfloat_t from, vfloat_t to);
int Mat2_load_on_shape(matrix2_t* mat, vfloat_t* data, size_t rows, size_t cols);

int Mat2_export(matrix2_t* mat, void* buff);
int Mat2_import(matrix2_t* mat, void* buff);

int Mat2_is_vector(matrix2_t* mat);
int Mat2_is_symmetric(matrix2_t* mat);
int Mat2_is_same_shape(matrix2_t* m1, matrix2_t* m2);

int Mat2_fill(matrix2_t* mat, vfloat_t v);
int Mat2_fill_random(matrix2_t* mat, double from, double to);
int Mat2_cpy(matrix2_t* dest, matrix2_t* src);
int Mat2_slice_row_to(matrix2_t* dest, matrix2_t* src, int row_idx);
int Mat2_slice_rows_to(matrix2_t* dest, matrix2_t* src, int begin, int open_end);
int Mat2_slice_col_to(matrix2_t* dest, matrix2_t* src, int col_idx);
int Mat2_slice_cols_to(matrix2_t* dest, matrix2_t* src, int begin, int open_end);
int Mat2_padding_left(matrix2_t* mat, int offset, vfloat_t fill);
int Mat2_padding_top(matrix2_t* mat, int offset, vfloat_t fill);
int Mat2_padding_right(matrix2_t* mat, int offset, vfloat_t fill);
int Mat2_padding_bottom(matrix2_t* mat, int offset, vfloat_t fill);
int Mat2_add(matrix2_t* dest, matrix2_t* src);
int Mat2_sub(matrix2_t* dest, matrix2_t* src);
int Mat2_scalar_multiply(matrix2_t* mat, vfloat_t scalar);
int Mat2_vect_dot(matrix2_t* mat1, matrix2_t* mat2, vfloat_t* out);
int Mat2_dot(matrix2_t* mat1, matrix2_t* mat2);
int Mat2_T(matrix2_t* mat);
int Mat2_merge_rows(matrix2_t* mat1, matrix2_t* mat2);
int Mat2_merge_cols(matrix2_t* mat1, matrix2_t* mat2);
int Mat2_reshape(matrix2_t* mat, size_t new_rows, size_t new_cols);

vfloat_t Mat2_get(matrix2_t* mat, size_t i, size_t j);
int Mat2_put(matrix2_t* mat, size_t i, size_t j, vfloat_t v);
int Mat2_2I(matrix2_t* mat, size_t side);

int Mat2_load_csv(matrix2_t* mat, char* file_csv);

int Mat2_get_co_to(matrix2_t* dest, matrix2_t* src, int p, int q);
int Mat2_det(matrix2_t* mat, vfloat_t* out);
int Mat2_get_adjoint_to(matrix2_t* dest, matrix2_t* src);
int Mat2_inverse(matrix2_t* mat);
int Mat2_hadamard_product(matrix2_t* mat1, matrix2_t* mat2);

int Mat2_qr(matrix2_t* q, matrix2_t* r, matrix2_t* a);
int Mat2_eig(matrix2_t* eigvalue_mat, matrix2_t* eigvectors_mat, matrix2_t* m1);

int Mat2_sum(matrix2_t* mat, int axis);
double Mat2_norm(matrix2_t* mat);

int Mat2_normalize_on_col(matrix2_t* mat);


#define MAT2_POOL_PTR(mat, ptr_name) vfloat_t(*ptr_name)[(mat)->cols]=(mat)->pool
#define MAT2_DIFF_LIST_PTR(diff_ptr) 
#define MAT2_ELEM_SIZE(mat) ((mat)->rows*(mat)->cols)


#define MAT2_INSPECT(pmat) \
({ \
    printf("\n <rows:%ld, cols:%ld>\n", (pmat)->rows, (pmat)->cols); \
    for (int __mat2_inspect_i=0; __mat2_inspect_i<(pmat)->rows; ++__mat2_inspect_i) { \
        for (int __mat2_inspect_j=0; __mat2_inspect_j<(pmat)->cols; ++__mat2_inspect_j) { \
            printf("%0.4f ", (pmat)->pool[__mat2_inspect_i*(pmat)->cols + __mat2_inspect_j]); \
        } \
        printf("\n");\
    } \
})

#endif