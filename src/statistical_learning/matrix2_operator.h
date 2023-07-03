
#ifndef __MATRIX2_OPERATOR_H__
#define __MATRIX2_OPERATOR_H__

#include "vtype/vfloat_type.h"

#define MAT2_RAW_INSPECT(pmat, rows, cols) \
({ \
    printf("\n raw mat Inspect <rows:%ld, cols:%ld>\n", (rows), (cols)); \
    for (int i=0; i<(rows); ++i) { \
        for (int j=0; j<(cols); ++j) { \
            printf("%0.3f ", (pmat)[i*(cols) + j]); \
        } \
        printf("\n");\
    } \
})

int __mat2_dot(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, size_t rows2, size_t cols2, vfloat_t* m3, size_t rows3, size_t cols3);
vfloat_t __mat2_vect_dot(vfloat_t* v1, vfloat_t* v2, size_t n);
vfloat_t __mat2_vect_norm(vfloat_t* v1, size_t n);
int __mat2_add(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, size_t rows2, size_t cols2);
int __mat2_sub(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, size_t rows2, size_t cols2);
int __mat2_scalar_multiply(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, size_t rows2, size_t cols2, vfloat_t scaler);
int __mat2_T(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, size_t rows2, size_t cols2);
int __mat2_rescale(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, size_t rows2, size_t cols2, int left, int top, int right, int bottom, vfloat_t fill);

vfloat_t __mat2_determinant(vfloat_t* v1, size_t n);
int __mat2_inverse(vfloat_t** m1, size_t* row1, size_t* cols1, vfloat_t* m2, size_t n);
int __mat2_co(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, size_t rows2, size_t cols2, int p, int q);
int __mat2_adjoint(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, int n);
int __mat2_svd();
int __mat2_qr(vfloat_t** q, size_t* q_rows, size_t* q_cols, vfloat_t** r, size_t* r_rows, size_t* r_cols, vfloat_t* mat, size_t mat_rows, size_t* mat_cols);
int __mat2_householder_transform(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, int n);
int __mat2_householder_matrix(vfloat_t** p, size_t* p_rows, size_t* p_cols, vfloat_t* a, int n);

#endif