/*
 * @Author: your name
 * @Date: 2021-07-01 12:22:37
 * @LastEditTime: 2025-06-09 12:15:05
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/machine_learning/svm/svm_probrom.h
 */
#ifndef __SVM_PROBLEM_H__
#define __SVM_PROBLEM_H__
#include "container/Cont.h"

typedef struct _svm_classify_problem {

    double tagA;
    double tagB;
    CN class_ls_A;
    CN class_ls_B;
    
    // 保存
    double c_weight_A;
    double c_weight_B;

} svm_classify_problem_t;

int svm_classify_problem_finalize(CN problem);

#endif