/*
 * @Author: your name
 * @Date: 2021-07-20 11:48:43
 * @LastEditTime: 2021-07-20 12:10:30
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/machine_learning/svm/svm_kernel_function.h
 */
#ifndef _SVM_KERNEL_FUNCTION_H_
#define _SVM_KERNEL_FUNCTION_H_
#include "vtype/vfloat_type.h"

double kernel_function_calculate_liner(vfloat_t* v1, vfloat_t* v2, int size_v, double _alpha, double _beta);
double kernel_function_calculate_poly(vfloat_t* v1, vfloat_t* v2, int size_v, double _degree);
double kernel_function_calculate_sigmoid(vfloat_t* v1, vfloat_t* v2, int size_v, double _gammer, double coef);
double kernel_function_calculate_rbf(vfloat_t* v1, vfloat_t* v2, int size_v, double _garmmer);

#endif