/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-04-04 10:06:30
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-06-15 13:13:24
 * @FilePath: /boring-code/src/statistical_learning/knn.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __KNN_H__
#define __KNN_H__

#include "vtype/vfloat_type.h"
#include "matrix2.h"

typedef struct {
    float    dist;
    int      label;
} dist_label_t;

int knn_predict(matrix2_t* _Input, matrix2_t* sample , matrix2_t* sample_label, int n, int* predict_label);

#endif