#ifndef __MATRIX2_OPERATOR_H__
#define __MATRIX2_OPERATOR_H__

#include "vtype/vfloat_type.h"

int __mat2_dot(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, size_t rows2, size_t cols2, vfloat_t* m3, size_t rows3, size_t cols3);
vfloat_t __mat2_vect_dot(vfloat_t* v1, vfloat_t* v2, size_t n);
int __mat2_add(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, size_t rows2, size_t cols2);
int __mat2_sub(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, size_t rows2, size_t cols2);
int __mat2_scalar_multiply(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, size_t rows2, size_t cols2, vfloat_t scaler);
int __mat2_T(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, size_t rows2, size_t cols2);
int __mat2_rescale(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, size_t rows2, size_t cols2, int left, int top, int right, int bottom, vfloat_t fill);

vfloat_t __mat2_determinant(vfloat_t* v1, size_t n);
int __mat2_inverse(vfloat_t** m1, size_t* row1, size_t* cols1, vfloat_t* m2, size_t n);
int __mat2_co(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, size_t rows2, size_t cols2, int p, int q);
int __mat2_adjoint(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, int n);
#endif