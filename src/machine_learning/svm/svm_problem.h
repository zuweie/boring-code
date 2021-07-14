/*
 * @Author: your name
 * @Date: 2021-07-01 12:22:37
 * @LastEditTime: 2021-07-14 11:09:07
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/machine_learning/svm/svm_probrom.h
 */
#ifndef _SVM_PROBLEM_H_
#define _SVM_PROBLEM_H_
#include "container/List.h"

typedef struct _svm_classify_problem {

    double tagA;
    double tagB;
    List* class_ls_A;
    List* class_ls_B;
    
    double c_weight_A;
    double c_weight_B;

} svm_classify_problem_t;

#endif