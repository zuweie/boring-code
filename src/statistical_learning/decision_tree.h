#ifndef __DECISION_TREE_H__
#define __DECISION_TREE_H__
#include "vtype/vfloat_type.h"
#include "matrix2.h"

typedef struct _cart_node cart_node_t;

struct _cart_node
{
    cart_node_t** sub_nodes;
    vfloat_t _predict;
    void* router;
    int attr_index;
};


cart_node_t* decision_tree_classification_train(matrix2_t* data, matrix2_t* label, void (*progress)(char*, unsigned long, unsigned long));

int decision_tree_regression_train(matrix2_t* data, matrix2_t* label, cart_node_t** _tree);

int decision_tree_classification_predict(matrix2_t* _x, cart_node_t* _tree, vfloat_t* predict);

int decision_tree_regression_predict(cart_node_t* _tree, vfloat_t* predict);

int decision_tree_release(cart_node_t* _tree);


#endif