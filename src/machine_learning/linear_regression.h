/*
 * @Author: your name
 * @Date: 2021-03-22 15:04:11
 * @LastEditTime: 2021-10-24 09:20:17
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/machine_learning/linear_regression.h
 */

#ifndef __LINEAR_REGRESSION_H__
#define __LINEAR_REGRESSION_H__

#include <stdlib.h>
#include "vtype/vfloat_type.h"
typedef struct _u_array u_array_t;
// 解方程的解法
int Linear_Regression_solve(u_array_t* X, u_array_t* Y, vfloat_t* W, vfloat_t* b);
// 伪逆的解法。
int Linear_Regression_pseudo_inverse(u_array_t* X, u_array_t* Y, vfloat_t* W, vfloat_t* b);
vfloat_t Linear_Regression_predict(u_array_t* X, vfloat_t* w, size_t wn, vfloat_t b);

#endif