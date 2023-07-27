#ifndef __DECISION_TREE_H__
#define __DECISION_TREE_H__
#include "vtype/vfloat_type.h"
#include "matrix2.h"

typedef struct _cart_node cart_node_t;

struct _cart_node
{
    cart_node_t* left;
    cart_node_t* right;
    vfloat_t _predict;
    int _xi; 
};


int decision_tree_classification_train(matrix2_t* data, matrix2_t* label, cart_node_t** _tree);
int decision_tree_regression_train(matrix2_t* data, matrix2_t* label, cart_node_t** _tree);

int decision_tree_classification_predict(cart_node_t* _tree, vfloat_t* predict);
int decision_tree_regression_predict(cart_node_t* _tree, vfloat_t* predict);

int decision_tree_release_dct(cart_node_t* _tree);

#endif