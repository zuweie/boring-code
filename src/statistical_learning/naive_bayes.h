/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-06-16 14:50:57
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-06-26 09:30:38
 * @FilePath: /boring-code/src/statistical_learning/naive_bayes.h
 * @Description: 朴素贝叶斯算法
 */
#ifndef __NAIVE_BAYES_H__
#define __NAIVE_BAYES_H__

#include "matrix2.h"

int navie_bayes_counting(matrix2_t* train_mat, matrix2_t* train_label_mat, void** Py_counting, void** Pxy_counting_table);

int navie_bayes_train_MGD_edit(matrix2_t* train_mat, matrix2_t* train_label_mat, void** mus, void** sigma_table);

int navie_bayes_predict(matrix2_t* _X, void* Py_counting, void* Pxy_counting_table, int lambda, vfloat_t* predict);

int navie_bayes_predict_continuous_edit(matrix2_t* _X, void** out);

int navie_bayes_release_counting(void* Py_counting, void* Pxy_count_table);

#endif