/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-04-04 10:06:30
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-04-26 10:25:43
 * @FilePath: /boring-code/src/statistical_learning/knn.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __KNN_H__
#define __KNN_H__

#include "vtype/vfloat_type.h"
#include "matrix2.h"

typedef struct _kd_tree_node kd_tree_node_t;

struct _kd_tree_node {

    int x_index;
    vfloat_t middle_value;
    kd_tree_node_t* parent;
    kd_tree_node_t* left;
    kd_tree_node_t* right;
} ;


kd_tree_node_t* knn_build_kd_tree(matrix2_t* _X);
int knn_destroy_kd_tree(kd_tree_node_t* tree);
int knn_perdict(matrix2_t* _Input, int n_near_by, vfloat_t* perdict);
#endif