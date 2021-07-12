/*
 * @Author: your name
 * @Date: 2021-06-22 14:54:00
 * @LastEditTime: 2021-07-10 08:14:26
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/machine_learning/svm/svm_model.h
 */

#ifndef _SVM_MODEL_H_
#define _SVM_MODEL_H_

#include "solver.h"

typedef struct _u_array u_array_t;

typedef struct _svm_model {

    float tagA;
    float tagB;
    SVM_type type;
    SVM_kernel kernel;
    double _star_rho;
    u_array_t _star_alpha;
    u_array_t _star_Y;
    u_array_t _star_X;
    
} svm_model_t;

#endif