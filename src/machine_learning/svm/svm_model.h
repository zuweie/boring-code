/*
 * @Author: your name
 * @Date: 2021-06-22 14:54:00
 * @LastEditTime: 2021-06-25 11:46:20
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
    /* 以下三个参数决定判别函数的输出 */
    
    u_array_t* _Y;
    u_array_t* _Alpha;
    double _rho;

} svm_model_t;

#endif