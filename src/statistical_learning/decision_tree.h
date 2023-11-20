/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-07-17 10:39:33
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-11-20 12:14:36
 * @FilePath: /boring-code/src/statistical_learning/decision_tree.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __DECISION_TREE_H__
#define __DECISION_TREE_H__
#include "vtype/vfloat_type.h"
#include "matrix2/matrix2.h"

typedef struct _dct_node dct_node_t;

struct _dct_node
{
    dct_node_t** sub_nodes;
    vfloat_t _predict;
    void* router;
    int attr_index;
};

typedef struct _dct_term {

    double gain_eps;
    int    least_limit;
    int    level_limit;

} dct_term_t;

typedef struct _dc_tree {
    dct_node_t* root;
    int         level;
} dc_tree_t;




int decision_tree_classification_train(matrix2_t* data, matrix2_t* label, dc_tree_t* tree, dct_term_t* trem, void (*progress)(char*, unsigned long, unsigned long));

// int decision_tree_regression_train(matrix2_t* data, matrix2_t* label, cart_node_t** _tree);

int decision_tree_classification_predict(matrix2_t* _x, dc_tree_t* _tree, vfloat_t* predict);

// int decision_tree_regression_predict(cart_node_t* _tree, vfloat_t* predict);

int decision_tree_release(dc_tree_t* _tree);


#endif