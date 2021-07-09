/*
 * @Author: your name
 * @Date: 2021-06-22 14:54:00
 * @LastEditTime: 2021-07-08 12:31:47
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/machine_learning/svm/svm_model.h
 */

#ifndef _SVM_MODEL_H_
#define _SVM_MODEL_H_

#include "solver.h"

typedef struct _u_array u_array_t;

typedef struct _svm_model {

    SVM_type type;
    SVM_kernel kernel;
    double _start_rho;
    u_array_t _start_alpha;
    u_array_t _start_Y;
    u_array_t _start_X;

} svm_model_t;

#endif