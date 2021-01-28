/*
 * @Author: your name
 * @Date: 2020-10-20 11:04:24
 * @LastEditTime: 2021-01-28 15:10:03
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/matrix/__matrix.h
 */
#ifndef _MATRIX_INTERFACE_H_
#define _MATRIX_INTERFACE_H_

#include <stdlib.h>

#define __DOUBLE__ 1

#if __DOUBLE__
    typedef double mx_float_t;
#else
    typedef float mx_float_t ;
#endif

typedef struct _imatrix imatrix_t;
struct _imatrix {
    mx_float_t (*get)(imatrix_t*, size_t row, size_t col);
    int   (*set)(imatrix_t*, size_t row, size_t col, mx_float_t);
    int   (*trans)(imatrix_t*);
    void  (*get_row)(imatrix_t*, size_t row_index, mx_float_t data[]);
    void  (*get_col)(imatrix_t*, size_t col_index, mx_float_t data[]);
    imatrix_t*  (*product)(imatrix_t*, imatrix_t*, imatrix_t*);
    size_t cols;
    size_t rows;
};

#define initialize_matrix(matrix, _get, _set, _trans, _get_row, _get_col, _rows, _cols) do {   \
    ((imatrix_t*)(matrix))->get     = (_get);                      \
    ((imatrix_t*)(matrix))->set     = (_set);                      \
    ((imatrix_t*)(matrix))->trans   = (_trans);                    \
    ((imatrix_t*)(matrix))->get_row = (_get_row);                  \
    ((imatrix_t*)(matrix))->get_col = (_get_col);                  \
    ((imatrix_t*)(matrix))->rows    = (_rows);                     \
    ((imatrix_t*)(matrix))->cols    = (_cols);                     \
} while (0)

#define Matrix_set(matrix_ptr, row, col, value) ((imatrix_t*)(matrix_ptr))->set(((imatrix_t*)(matrix_ptr)), row, col, value)
#define Matrix_get(matrix_ptr, row, col) ((imatrix_t*)(matrix_ptr))->get(((imatrix_t*)(matrix_ptr)),row, col)
#define Matrix_rows(matrix_ptr) ((imatrix_t*)(matrix_ptr))->rows
#define Matrix_cols(matrix_ptr) ((imatrix_t*)(matrix_ptr))->cols
#define Matrix_get_row(matrix_ptr, row_index, data) ((imatrix_t*)(matrix_ptr))->get_row(((imatrix_t*)(matrix_ptr)), row_index, data)
#define Matrix_get_col(matrix_ptr, col_index, data) ((imatrix_t*)(matrix_ptr))->get_col(((imatrix_t*)(matrix_ptr)), col_index, data)
#define Matrix_trans(matrix_ptr) ((imatrix_t*)(matrix_ptr))->trans((imatrix_t*)(matrix_ptr))

#define Matrix_product(matrix_ptr1, matrix_ptr2, matrix_product_ptr) \
    ({ \
        for (int i=0; i<Matrix_rows(matrix_ptr1); ++i) { \
            for (int j=0; j<Matrix_cols((matrix_ptr2)); ++j){ \
                mx_float_t v = 0.0f; \
                for (int k=0; k<Matrix_cols(matrix_ptr1); ++k) { \
                    v += Matrix_get(matrix_ptr1, i, k) * Matrix_get(matrix_ptr2, k, j); \
                } \
                Matrix_set(matrix_product_ptr, i, j, v); \
            } \
        } \
        matrix_product_ptr; \
    })

#endif