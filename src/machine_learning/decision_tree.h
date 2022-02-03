/*
 * @Author: your name
 * @Date: 2022-01-17 11:00:41
 * @LastEditTime: 2022-02-03 13:59:38
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /boring-code/src/machine_learning/desicion_tree.h
 */
#ifndef __DECISION_TREE_H__
#define __DECISION_TREE_H__

#include "ultra_array/ultra_array.h"

typedef struct _cart_node_t cart_node_t;

struct _cart_node_t{
    float gini;
    float cr;
    int attribe_id;
    cart_node_t* parent;
    cart_node_t* lef;
    cart_node_t* right;
} cart_node_t;

typedef struct _car_tree {
    cart_node_t* node;
    unsigned char is_classifier;
} car_tree_t;

int cart_training(u_array_t* X, u_array_t* Y, car_tree_t* tree, unsigned char is_classifier);
int cart_finalize(car_tree_t* car_tree);
int cart_predict(u_array_t* sample, car_tree_t* car_tree);

#endif