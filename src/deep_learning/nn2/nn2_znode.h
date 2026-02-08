#ifndef __NN2_ZNODE_H__
#define __NN2_ZNODE_H__

#include "nn2_znode_base.h"
typedef struct matrix2 matrix2_t;

typedef struct nn2_linear 
{
    nn2_znode_base_t _base;
    matrix2_t* W;
    matrix2_t* GW;
    matrix2_t* b;
    matrix2_t* Gb;
    
} nn2_linear_t;

typedef struct nn2_relu {
    nn2_znode_base_t _base;
} nn2_relu_t;

typedef struct nn2_softmax {
    nn2_znode_base_t _base;
} nn2_softmax_t;

typedef struct nn2_mse_loss {
    nn2_znode_base_t _base;
} nn2_mse_loss_t;

typedef struct nn2_crossentropy_loss {
    nn2_znode_base_t _base;
} nn2_crossentropy_loss_t;


nn2_znode_base_t* create_linear(int in_dimens, int out_dimens);
nn2_znode_base_t* create_relu();
nn2_znode_base_t* create_softmax();
nn2_znode_base_t* create_mse_loss();
nn2_znode_base_t* create_crossentropy_loss();

#endif
