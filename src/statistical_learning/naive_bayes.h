/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-06-16 14:50:57
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-06-20 15:43:28
 * @FilePath: /boring-code/src/statistical_learning/naive_bayes.h
 * @Description: 朴素贝叶斯算法
 */
#ifndef __NAIVE_BAYES_H__
#define __NAIVE_BAYES_H__

#include "matrix2.h"

int navie_bayes_train(matrix2_t* train_mat, matrix2_t* train_label_mat, void** Py_probability, void** Pxy_probability);
int navie_bayes_predict(matrix2_t* _X, void* Py_probability, void* Pxy_probability);

#endif