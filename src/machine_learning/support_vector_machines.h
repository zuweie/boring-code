/*
 * @Author: your name
 * @Date: 2021-05-10 13:15:30
 * @LastEditTime: 2021-06-01 15:38:26
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

#define IS_LOWER_BOUND(alpha_status, i) (alpha_status[i] < 0)
#define IS_UPPER_BOUND(alpha_status, i) (alpha_status[i] > 0)
// class
int solve_c_svc();
int solve_nu_svc();
int solve_one_class();
int solve_e_svr();
int solve_nu_svr();
int select_working_set(int* out_i, int* out_j);

// smo 终极算法。
int solve_generic(u_array_t* alpha, u_array_t* Q);



#endif