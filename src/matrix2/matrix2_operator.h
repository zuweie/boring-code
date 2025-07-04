/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-06-15 16:10:10
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-06-03 13:26:02
 * @FilePath: /boring-code/src/statistical_learning/matrix2_operator.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

#ifndef __MATRIX2_OPERATOR_H__
#define __MATRIX2_OPERATOR_H__

#include "vtype/vfloat_type.h"

#define MAT2_RAW_INSPECT(pmat, rows, cols) \
do{ \
    printf("\n raw mat Inspect:\n <rows:%ld, cols:%ld>\n", (rows), (cols)); \
    for (int __mat2_raw_inspect_i=0; __mat2_raw_inspect_i<(rows); ++__mat2_raw_inspect_i) { \
        for (int __mat2_raw_inspect_j=0; __mat2_raw_inspect_j<(cols); ++__mat2_raw_inspect_j) { \
            printf("%0.5f ", (pmat)[__mat2_raw_inspect_i*(cols) + __mat2_raw_inspect_j]); \
        } \
        printf("\n");\
    } \
} while(0)

int __mat2_dot(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, size_t rows2, size_t cols2, vfloat_t* m3, size_t rows3, size_t cols3);
int __mat2_hadamard_product(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, size_t rows2, size_t cols2, vfloat_t* m3, size_t rows3, size_t cols3);
vfloat_t __mat2_vect_dot(vfloat_t* v1, vfloat_t* v2, size_t n);
vfloat_t __mat2_vect_norm(vfloat_t* v1, size_t n);
int __mat2_vect_normalize(vfloat_t* v1, size_t n);
int __mat2_add(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, size_t rows2, size_t cols2);
int __mat2_sub(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, size_t rows2, size_t cols2);
int __mat2_scalar_multiply(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, size_t rows2, size_t cols2, vfloat_t scaler);
int __mat2_T(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, size_t rows2, size_t cols2);
int __mat2_merge_rows(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, size_t row2, size_t cols2, vfloat_t* m3, size_t rows3, size_t cols3);
int __mat2_merge_cols(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, size_t row2, size_t cols2, vfloat_t* m3, size_t rows3, size_t cols3);
int __mat2_rescale(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, size_t rows2, size_t cols2, int left, int top, int right, int bottom, vfloat_t fill);


vfloat_t __mat2_determinant(vfloat_t* v1, size_t n);
int __mat2_inverse(vfloat_t** m1, size_t* row1, size_t* cols1, vfloat_t* m2, size_t n);
int __mat2_co(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, size_t rows2, size_t cols2, int p, int q);
int __mat2_adjoint(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, int n);
int __mat2_svd();
int __mat2_qr_decomp(vfloat_t** q, size_t* q_rows, size_t* q_cols, vfloat_t** r, size_t* r_rows, size_t* r_cols, vfloat_t* mat, size_t mat_rows, size_t mat_cols, int start, int end);
int __mat2_qr_alg(vfloat_t** a, size_t* a_rows, size_t* a_cols, vfloat_t** q, size_t* q_rows, size_t* q_cols, vfloat_t* m, size_t n);
int __mat2_householder_matrix(vfloat_t** p, size_t* p_rows, size_t* p_cols, vfloat_t* a, int n);
int __mat2_lu_decomp(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, size_t rows2, size_t cols2);
int __mat2_solve(vfloat_t* m1, vfloat_t* y, size_t n);
int __mat2_solve_l(vfloat_t* ul, vfloat_t* z, size_t n);
int __mat2_solve_u(vfloat_t* ul, vfloat_t* x, size_t n);
int __mat2_eigenvalues(vfloat_t** eigen_values,vfloat_t* m1, size_t n);
int __mat2_eigenvector(vfloat_t** eigen_vector, vfloat_t* m1, vfloat_t eigen_value, size_t n);

#endif