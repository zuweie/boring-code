#ifndef __MATRIX2_H__
#define __MATRIX2_H__
#include <stdlib.h>
#include "matrix2_count.h"
#include "matrix2_operator.h"
#include "container/cn.h"
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

vfloat_t Mat2_get(matrix2_t* mat, size_t i, size_t j);
int Mat2_put(matrix2_t* mat, size_t i, size_t j, vfloat_t v);

int Mat2_load_csv(matrix2_t* mat, char* file_csv);

int Mat2_get_co_to(matrix2_t* dest, matrix2_t* src, int p, int q);
int Mat2_det(matrix2_t* mat, vfloat_t* out);
int Mat2_get_adjoint_to(matrix2_t* dest, matrix2_t* src);
int Mat2_inverse(matrix2_t* mat);
int Mat2_is_vector(matrix2_t* mat);
int Mat2_qr(matrix2_t* q, matrix2_t* r, matrix2_t* a);

int Mat2_eig(matrix2_t** eigvalue_mat, CN eigvector_mats, matrix2_t* m1);

#define MAT2_POOL_PTR(mat, ptr_name) vfloat_t(*ptr_name)[(mat)->cols]=(mat)->pool
#define MAT2_DIFF_LIST_PTR(diff_ptr) 


#define MAT2_INSPECT(pmat) \
({ \
    printf("\n <rows:%ld, cols:%ld>\n", (pmat)->rows, (pmat)->cols); \
    for (int i=0; i<(pmat)->rows; ++i) { \
        for (int j=0; j<(pmat)->cols; ++j) { \
            printf("%0.3f ", (pmat)->pool[i*(pmat)->cols + j]); \
        } \
        printf("\n");\
    } \
})

#define MAT2_INSPECT_COUNTING(counting)\
({\
    int*     size_ptr   = MAT2_COUNTING_SIZE_PTR(counting);\
    vfloat_t* diff_ptr  = MAT2_COUNTING_LIST_PTR(counting);\
    int*     number_ptr = MAT2_COUNTING_NUMBERS_PTR(counting);\
    printf("\n <diff size: %d>: \n", (*size_ptr));\
    for (int i=0; i<*size_ptr; ++i) {\
        vfloat_t target = diff_ptr[i];\
        int number      = __mat2_get_element_number(counting, target);\
        printf("<%f: %d>\n", target, number);\
    }\
})

#endif