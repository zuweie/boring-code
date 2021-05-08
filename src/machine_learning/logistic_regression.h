/*
 * @Author: your name
 * @Date: 2021-04-04 15:30:46
 * @LastEditTime: 2021-05-07 08:42:26
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/machine_learning/logistic_regression.h
 */

#ifndef _LOGISTIC_REGRESSION_H_
#define _LOGISTIC_REGRESSION_H_
#include "vtype/vfloat_type.h"

typedef struct _u_array u_array_t;

/**
 * eta 学习率
 * epochs 迭代上限
 * epsilon 梯度模长的上限
 */
int logistic_regression(u_array_t* X, u_array_t* y, vfloat_t eta, vfloat_t epochs, vfloat_t epsilon, vfloat_t* W);

#endif