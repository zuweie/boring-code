/*
 * @Author: your name
 * @Date: 2021-06-22 14:54:00
 * @LastEditTime: 2021-11-22 14:54:53
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/machine_learning/svm/svm_model.h
 */

#ifndef __SVM_MODEL_H__
#define __SVM_MODEL_H__

#include "solver.h"
typedef struct _u_array u_array_t;

typedef struct _svm_model {

    float tagA;
    float tagB;
    
    SVM_type type;
    SVM_kernel kernel;
    double _star_rho;
    double _star_r;
    
    u_array_t _star_alpha;
    u_array_t _star_Y;
    u_array_t _star_X;

    int sv_count;
    kernel_param_t k_param;
    
    u_array_t (*calculate_kernel)(struct _svm_model* model, u_array_t* sample);
} svm_model_t;


int svm_model_finalize(svm_model_t* model);

u_array_t svm_model_calculate_liner(svm_model_t* model, u_array_t* sample);
u_array_t svm_model_calculate_poly(svm_model_t* model, u_array_t* sample);
u_array_t svm_model_calculate_sigmoid(svm_model_t* model, u_array_t* sample);
u_array_t svm_model_calculate_rbf(svm_model_t* model, u_array_t* sample);

#endif