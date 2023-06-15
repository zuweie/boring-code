/*
 * @Author: your name
 * @Date: 2021-07-20 11:48:43
 * @LastEditTime: 2021-10-24 09:41:01
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/machine_learning/svm/svm_kernel_function.h
 */
#ifndef __SVM_KERNEL_FUNCTION_H__
#define __SVM_KERNEL_FUNCTION_H__
#include "vtype/vfloat_type.h"

double kernel_function_calculate_liner(vfloat_t* v1, vfloat_t* v2, int size_v, double _alpha, double _beta);
double kernel_function_calculate_poly(vfloat_t* v1, vfloat_t* v2, int size_v, double _degree);
double kernel_function_calculate_sigmoid(vfloat_t* v1, vfloat_t* v2, int size_v, double _gammer, double coef);
double kernel_function_calculate_rbf(vfloat_t* v1, vfloat_t* v2, int size_v, double _garmmer);

#endif