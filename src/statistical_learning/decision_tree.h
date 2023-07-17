#ifndef __DECISION_TREE_H__
#define __DECISION_TREE_H__
#include "vtype/vfloat_type.h"
#include "matrix2.h"

typedef struct _dct_node dct_node_t;

struct _dct_node
{
    dct_node_t* left;
    dct_node_t* right;
};


int decision_tree_classify_train(matrix2_t* data, matrix2_t* label, dct_node_t* _tree);
int decision_tree_regression_train(matrix2_t* data, matrix2_t* label, dct_node_t* _tree);

int decision_tree_classify_predict(dct_node_t* _tree, vfloat_t* predict);
int decision_tree_regression_train(dct_node_t* _tree, vfloat_t* predict);

int decision_tree_release_dct(dct_node_t* _tree);

#endif