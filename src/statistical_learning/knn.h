/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-04-04 10:06:30
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-11-20 10:24:54
 * @FilePath: /boring-code/src/statistical_learning/knn.h
 * @Description: knn 算法
 */
#ifndef __KNN_H__
#define __KNN_H__

#include "vtype/vfloat_type.h"
#include "matrix2/matrix2.h"

typedef struct {
    float    dist;
    int      label;
} dist_label_t;

int knn_predict(matrix2_t* _Input, matrix2_t* sample , matrix2_t* sample_label, int n, int* predict_label);

#endif