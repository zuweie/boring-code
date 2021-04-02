/*
 * @Author: your name
 * @Date: 2021-03-22 15:04:11
 * @LastEditTime: 2021-04-02 07:59:09
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/machine_learning/linear_regression.h
 */

#ifndef _LINEAR_REGRESSION_H_
#define _LINEAR_REGRESSION_H_

#include <stdlib.h>

typedef struct _u_array u_array_t;
// 解方程的解法
int Linear_Regression_solve(u_array_t* X, u_array_t* Y, double* W, double* b);
// 伪逆的解法。
int Linear_Regression_pseudo_inverse(u_array_t* X, u_array_t* Y, double* W, double* b);
double Linear_Regression_predict(u_array_t* X, double* w, size_t wn, double b);

#endif