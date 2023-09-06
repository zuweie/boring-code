/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-08-15 14:51:24
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-09-06 11:15:16
 * @FilePath: /boring-code/src/statistical_learning/adaboost.h
 * @Description: 因为 decision_tree 那个 C 文件已经做了分类树，那么这个 adaboost_tree 就做回归树吧
 */
#ifndef __ADABOOST_H__
#define __ADABOOST_H__
#include "vtype/vfloat_type.h"
#include "matrix2.h"

typedef struct {

    int split_index;
    vfloat_t binal_value;
    vfloat_t expect_label;

} adaboost_gx_t

int adaboost_tree_train(matrix2_t* train_data, matrix2_t* train_label, int M, double* alpahs,  adaboost_gx_t[] tree);
int adaboost_tree_predict(vfloat_t* predict);

#endif