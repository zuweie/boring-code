#ifndef __LINEAR_OPT_H__
#define __LINEAR_OPT_H__

#include "deep_learning/compute_graph2/cg_debug.h"
#include "deep_learning/compute_graph2/cg_list.h"
#include "deep_learning/compute_graph2/cg_tensor.h"
#include "deep_learning/compute_graph2/cg_operand.h"
#include "deep_learning/compute_graph2/cg_operator.h"

typedef struct linear_opt {
    cg_operator_t operator;
    cg_operand_t* x;
    cg_operand_t* W;
    cg_operand_t* b;
} linear_opt_t;


static inline int __calculate(cg_node_t* thiz, cg_node_t* out)
{
    cg_operand_t* out_operand = (cg_operand_t*) out;
    linear_opt_t* operator    = (linear_opt_t*) thiz;
    cg_operand_t* x           = operator->x;
    cg_operand_t* W           = operator->W;
    cg_operand_t* b           = operator->b;

    CG_DEBUG("calculate @ %s operator:\n", CG_NODE_ID(operator));
    CG_DEBUG(" %s = %s X %s + %s\n", CG_NODE_ID(out_operand), CG_NODE_ID(W), CG_NODE_ID(x), CG_NODE_ID(b));
    return 0;
}

static inline int __differentiate(cg_node_t* thiz,  cg_node_t* variant, cg_ref_t env_gradient)
{
    linear_opt_t* operator = (linear_opt_t*) thiz;
    cg_operand_t* x        = operator->x;
    cg_operand_t* W        = operator->W;
    cg_operand_t* b        = operator->b;
    
    CG_DEBUG("differentinate @ %s operator\n", CG_NODE_ID(operator))
    if (variant == x) {
        
        CG_DEBUG("delta %s = (%s)^T X env_gradient\n", CG_NODE_ID(x), CG_NODE_ID(W));

    } else if (variant == W) {

        CG_DEBUG("delta %s = (%s)^T X env_gradient\n", CG_NODE_ID(W), CG_NODE_ID(x));

    } else if (variant == b) {

        CG_DEBUG("delta %s = 1 X env_gradient\n", CG_NODE_ID(b));

    } else {

        CG_DEBUG("Error! object to been different is no belong %s\n", CG_NODE_ID(operator));

    }
    return 0;
}

static inline linear_opt_t* linear_opt_create(const char* id, cg_operand_t x, cg_operand_t* W, cg_operand_t* b) 
{
    linear_opt_t* linear_opt = (linear_opt_t*) malloc (sizeof(linear_opt_t));
    cg_operator_init(linear_opt, id, __calculate, __differentiate);
    linear_opt->x = x;
    linear_opt->W = W;
    linear_opt->b = b;

    return linear_opt;
}

static inline int linear_opt_recycle(linear_opt_t* thiz) 
{
    cg_operator_reset(thiz);
    free(thiz);
    return 0;
}

#endif