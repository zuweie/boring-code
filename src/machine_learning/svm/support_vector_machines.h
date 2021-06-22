/*
 * @Author: your name
 * @Date: 2021-05-10 13:15:30
 * @LastEditTime: 2021-06-22 15:11:14
 * @LastEditors: Please set LastEditors
 * @Description: 软间隔支持向量机的实现
 * @FilePath: /boring-code/src/machine_learning/svm.h
 */
#ifndef _SUPPORT_VECTOR_MACHINES_H_
#define _SUPPORT_VECTOR_MACHINES_H_

#include "vtype/vfloat_type.h"
#include "solver.h"
#include "svm_model.h"

#define TUA 1e-12
typedef struct _u_array u_array_t;



#define IS_LOWER_BOUND(alpha_status, i) (alpha_status[i] < 0)
#define IS_UPPER_BOUND(alpha_status, i) (alpha_status[i] > 0)
// class
int solve_c_svc( \
        u_array_t* X, u_array_t* Y, \
        SVM_type svm_type,  \
        SVM_kernel SVM_kernel, \
        vfloat_t _C, vfloat_t _gammer, \ 
        vfloat_t _coef, vfloat_t _degree, \
        double eps, \
        int max_iter \
    );

int solve_nu_svc();
int solve_one_class();
int solve_e_svr();
int solve_nu_svr();

// smo 终极算法。
int solve_generic(solver_t* solver, svm_model_t* model);

#endif