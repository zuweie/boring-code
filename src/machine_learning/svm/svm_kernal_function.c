/*
 * @Author: your name
 * @Date: 2021-07-20 11:48:31
 * @LastEditTime: 2021-07-20 12:31:59
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/machine_learning/svm/svm_kernal_function.c
 */
#include <math.h>
#include "svm_kernel_function.h"

double kernel_function_calculate_liner(vfloat_t* v1, vfloat_t* v2, int size_v, double _alpha, double _beta)
{
    double dot = 0.f;
    for (int i=0; i<size_v; ++i) {
        dot += v1[i] * v2[i];
    }
    return _alpha * dot + _beta;
}

double kernel_function_calculate_poly(vfloat_t* v1, vfloat_t* v2, int size_v, double _degree)
{
    double linear_dot = kernel_calculate_liner(v1, v2, size_v, 1, 0);
    return pow(linear_dot, _degree);
}

double kernelf_function_calculate_sigmoid(vfloat_t* v1, vfloat_t* v2, int size_v, double _gammer, double _coef)
{
    double t = kernel_calculate_liner(v1, v2, size_v, -2*_gammer, -2*_coef );
    double e   = exp(-fabs(t));

    return t > 0 ? ((1.f - e) / (1.f + e)) : ((e - 1.f) / (e + 1.f));
}

double kernel_function_calculate_rbf(vfloat_t* v1, vfloat_t* v2, int size_v, double _garmmer) 
{
    double dis_q = 0.f;
    for (int i=0; i<size_v; ++i) {
        dis_q += (v1[i] - v2[i]) * (v1[i] - v2[i]);
    }
    return exp(dis_q * -_garmmer);
}