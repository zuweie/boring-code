/*
 * @Author: your name
 * @Date: 2021-05-10 13:15:30
 * @LastEditTime: 2021-05-22 17:52:22
 * @LastEditors: Please set LastEditors
 * @Description: 软间隔支持向量机的实现
 * @FilePath: /boring-code/src/machine_learning/svm.h
 */
#ifndef _SUPPORT_VECTOR_MACHINES_H_
#define _SUPPORT_VECTOR_MACHINES_H_

#include "vtype/vfloat_type.h"

typedef struct _u_array u_array_t;

enum { C_SVC, NU_SVC, ONE_CLASS, EPSILON_SVR, NU_SVR }; /* SVM type */
enum { LINEAR, POLY, BRF, SIGMOID, PRECOMPUTED }; /* kernel function type */

#endif