#ifndef __SOFTMAX_H__
#define __SOFTMAX_H__

#include "deep_learning/compute_graph2/cg_debug.h"
#include "deep_learning/compute_graph2/cg_operator.h"
#include "deep_learning/compute_graph2/cg_operand.h"

typedef struct softmax_opt
{
    cg_opreator_t operator;
    cg_operand_t* variant;

} softmax_opt_t;

static inline int __calculate(cg_node_t* thiz, cg_node_t* out) 
{
    softmax_opt*      operator = (cg_operator_t*) thiz;
    cg_operand_t*  out_opreand = (cg_operand_t*) out;
    cg_operand_t*  varaint     = operator->variant;

    CG_DEBUG("calculate @ %s: \n", CG_NODE_ID(operator));
    CG_DEBUG("%s = softmax(%s) \n", CG_NODE_ID(out_opreand), CG_NODE_ID(varaint));
    return 0;
}

static inline int __differentiate(cg_node_t* thiz, cg_node_t* variant, cg_ref_t env_gradient)
{
    softmax_opt* operator = (cg_operator_t*) thiz;

    CG_DEBUG("differentiate @ %s: \n", CG_NODE_ID(operator));
    if (variant == operator->variant) {
        CG_DEBUG("delta %s = delta softmax(%s) X env_gradient", CG_NODE_ID(operator->variant), CG_NODE_ID(operator->variant));
    } else {
        CG_DEBUG("ERROR! %s is not belong %s\n", CG_NODE_ID(variant), CG_NODE_ID(operator));
        return -1;
    }
    return 0;
}

static inline softmax_opt_t* softmax_opt_create(cg_operator_t* thiz, const char* id, cg_operand_t* variant)
{
    softmax_opt_t* softmax_opt = (softmax_opt_t*) malloc (sizeof(softmax_opt_t));
    softmax_opt->variant       = variant;
    cg_operator_init(softmax_opt, id, __calculate, __differentiate);
    return softmax_opt;
}

static inline int softmax_opt_recycle(cg_operator_t* thiz) 
{
    cg_operator_reset(thiz);
    free(thiz);
    return 0;
}


#endif