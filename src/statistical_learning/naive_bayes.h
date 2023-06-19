/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-06-16 14:50:57
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-06-19 11:09:10
 * @FilePath: /boring-code/src/statistical_learning/naive_bayes.h
 * @Description: 朴素贝叶斯算法
 */
#ifndef __NAIVE_BAYES_H__
#define __NAIVE_BAYES_H__

#include "matrix2.h"

/**
 * @brief 这个数据结构是关于标签，属性以及属性类别的统计。
 * 
 */
typedef struct {

    // label 的数量。
    int label_number;
    // 属性的数量
    int feature_number;
    // 最大的哪个属性类别数量。
    int max_feature_type_number; 
    // 一个统计好的label
    int**   label_archive;
    int**** label_feature_archive;

} navie_bayes_statistics_table_t;

int navie_bayes_statistics(matrix2_t* train_mat, matrix2_t* train_label_mat, navie_bayes_statistics_table_t* table);

int navie_bayes_predict(matrix2_t* _X, navie_bayes_statistics_table_t* table);

int navie_bayes_release_statistics_table(navie_bayes_statistics_table_t* table);

int navie_bayes_inspect_statistics_table(navie_bayes_statistics_table_t* table);

#endif