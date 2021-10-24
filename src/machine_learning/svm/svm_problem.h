/*
 * @Author: your name
 * @Date: 2021-07-01 12:22:37
 * @LastEditTime: 2021-10-24 09:41:25
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/machine_learning/svm/svm_probrom.h
 */
#ifndef __SVM_PROBLEM_H__
#define __SVM_PROBLEM_H__
#include "container/List.h"

typedef struct _svm_classify_problem {

    double tagA;
    double tagB;
    List* class_ls_A;
    List* class_ls_B;
    
    // 保存
    double c_weight_A;
    double c_weight_B;

} svm_classify_problem_t;

#endif