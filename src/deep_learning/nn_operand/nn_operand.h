#ifndef __NN_OPERAND_H__
#define __NN_OPERAND_H__
#include "deep_learning/nnalloc/nn_alloc.h"
#include "deep_learning/compute_graph2/cg_tensor.h"
#include "deep_learning/compute_graph2/cg_operand.h"

typedef struct nn_operand {

    cg_operand_t _base;
    cg_tensor_t* x;
    cg_tensor_t* Gx;

} nn_operand_t;

static inline int __reset(cg_operand_t* thiz) {
    cg_tensor_recycle(((nn_operand_t*)operand)->x);
    cg_tensor_recycle(((nn_operand_t*)operand)->Gx);
    return 0;
}

static inline nn_operand_t* nn_operand_create(const char* id, int in_dimens, int out_dimens)
{
    nn_operand_t* operand = (nn_operand_t*) malloc (sizoeof(nn_operand_t));
    cg_operand_init(operand, id, __reset);
    *operand = (nn_operand_t) {
        .x  = cg_tensor_create(Get_alloc(), 2, in_dimens, out_dimens),
        .Gx = cg_tensor_create(Get_alloc(), 2, in_dimens, out_dimens)
    };
    return operand;
}

#endif