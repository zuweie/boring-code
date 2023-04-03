/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-03-31 10:14:25
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-04-03 14:15:27
 * @FilePath: /boring-code/src/machine_learning2/matrix2.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __MATRIX2_H__
#define __MATRIX2_H__
#include <stdlib.h>
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

int Mat2_fill(matrix2_t* mat, vfloat_t v);
int Mat2_cpy(matrix2_t* dest, matrix2_t* src);
//int Mat2_rescale_to(matrix2_t* dest, matrix2_t* src, int left, int top, int right, int bottom, vfloat_t fill);
int Mat2_slice_row_to(matrix2_t* dest, matrix2_t* src, int row_idx);
int Mat2_slice_col_to(matrix2_t* dest, matrix2_t* src, int col_idx);
int Mat2_padding_left(matrix2_t* mat, int offset, vfloat_t fill);
int Mat2_padding_top(matrix2_t* mat, int offset, vfloat_t fill);
int Mat2_padding_right(matrix2_t* mat, int offset, vfloat_t fill);
int Mat2_padding_bottom(matrix2_t* mat, int offset, vfloat_t fill);
int Mat2_add(matrix2_t* dest, matrix2_t* src);
int Mat2_sub(matrix2_t* dest, matrix2_t* src);
int Mat2_scalar_multiply(matrix2_t* mat, vfloat_t scalar);
    
int Mat2_vect_dot(matrix2_t* mat1, matrix2_t* mat2, vfloat_t* out);


vfloat_t Mat2_get(matrix2_t* mat, size_t i, size_t j);
int Mat2_put(matrix2_t* mat, size_t i, size_t j, vfloat_t v);



#define MAT2_POOL_PTR(mat, ptr_name) vfloat_t(*ptr_name)[(mat)->cols]=(mat)->pool

#define MAT2_INSPACT(pmat) \
({ \
    printf("\n <rows:%ld, cols:%ld>\n", (pmat)->rows, (pmat)->cols); \
    for (int i=0; i<(pmat)->rows; ++i) { \
        for (int j=0; j<(pmat)->cols; ++j) { \
            printf("%f ", (pmat)->pool[i*(pmat)->cols + j]); \
        } \
        printf("\n");\
    } \
})

#endif