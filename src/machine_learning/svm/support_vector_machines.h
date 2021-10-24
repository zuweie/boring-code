/*
 * @Author: your name
 * @Date: 2021-05-10 13:15:30
 * @LastEditTime: 2021-10-24 09:40:51
 * @LastEditors: Please set LastEditors
 * @Description: 软间隔支持向量机的实现
 * @FilePath: /boring-code/src/machine_learning/svm.h
 */
#ifndef __SUPPORT_VECTOR_MACHINES_H__
#define __SUPPORT_VECTOR_MACHINES_H__

#include "vtype/vfloat_type.h"
#include "container/List.h"
#include "solver.h"
#include "svm_model.h"

#define TUA 1e-12
typedef struct _u_array u_array_t;

// class
int svm_solve_c_svc( \
        u_array_t* X, u_array_t* Y, \
        SVM_kernel SVM_kernel, \
        double _C, double _gammer, \ 
        double _coef, double _degree, \
        double eps, \
        int max_iter, \
        List* classify_models \
    );

int svm_solve_nu_svc();
int svm_solve_one_class();
int svm_solve_e_svr();
int svm_solve_nu_svr();
int svm_solve_generic(solver_t* solver);

// svm 预测函数。
double svm_c_svc_predict(List* classify_models, u_array_t* sample);
double svm_c_svm_predict_one(svm_model_t* model, u_array_t* sample);

int svm_classify_problems_finalize(List* problems);
int svm_classify_problem(u_array_t* X, u_array_t* Y, List* problems);
int svm_models_finalize(List* models);
int svm_models_export(List* models);
#endif