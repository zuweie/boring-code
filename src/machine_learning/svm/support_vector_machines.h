/*
 * @Author: your name
 * @Date: 2021-05-10 13:15:30
 * @LastEditTime: 2021-06-25 14:58:47
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

typedef struct {

    int class_nr;
    int *class_index_map;
    int *y_classification;
    float *y_class_weight;

} Y_classification_t;

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

int Svm_train(u_array_t* X, u_array_t* Y, SVM_type type, SVM_kernel kernel, svm_model_t* model);
float Svm_predict(svm_model_t* model, u_array_t* sample);
 
// smo 终极算法。
int solve_generic(solver_t* solver, svm_model_t* model);

int Y_classify(u_array_t* _Y, Y_classification_t* classify);
int Y_classication_release(Y_classification_t* classify);

#endif