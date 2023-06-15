/*
 * @Author: your name
 * @Date: 2021-04-04 15:30:46
 * @LastEditTime: 2021-10-24 09:21:38
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/machine_learning/logistic_regression.h
 */

#ifndef __LOGISTIC_REGRESSION_H__
#define __LOGISTIC_REGRESSION_H__
#include "vtype/vfloat_type.h"

typedef struct _u_array u_array_t;

/**
 * eta 学习率
 * epochs 迭代上限
 * epsilon 梯度模长的上限
 */
int logistic_regression_train(u_array_t* X, u_array_t* y, u_array_t* W, vfloat_t eta, int epochs, vfloat_t epsilon);

#endif