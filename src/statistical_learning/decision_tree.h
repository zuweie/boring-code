#ifndef __DECISION_TREE_H__
#define __DECISION_TREE_H__
#include "vtype/vfloat_type.h"
#include "matrix2.h"

typedef struct _dct_node dct_node_t;

struct _dct_node
{
    dct_node_t** sub_nodes;
    vfloat_t _predict;
    void* router;
    int attr_index;
};

typedef struct _dct_term {

    double gain_esp;
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