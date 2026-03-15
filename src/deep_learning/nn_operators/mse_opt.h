#ifndef __MSE_OPT_H__
#define __MSE_OPT_H__

#include "deep_learning/compute_graph2/cg_debug.h"
#include "deep_learning/compute_graph2/cg_operator.h"
#include "deep_learning/compute_graph2/cg_operand.h"

typedef struct mse_opt {

    cg_operator_t operator;
    cg_operand_t* y_hat;
    cg_operand_t* labels;

} mse_opt_t;

static inline int __calculate(cg_node_t* thiz, cg_node_t* out)
{
    mse_opt_t* operator   = (mse_opt_t*) thiz;
    cg_operand_t* y_hat   = operator->y_hat;
    cg_operand_t* labels  = operator->labels;

    CG_DEBUG("calculate @ %s\n", CG_NODE_ID(operator));
    CG_DEBUG("%s = (%s - %s)^2 \n", CG_NODE_ID(out), CG_NODE_ID(y_hat), CG_NODE_ID(labels));
    return 0;
}

static inline int __differentiate(cg_node_t* thiz, cg_node_t* variant, cg_ref_t env_gradient)
{
    mse_opt_t* operator = (mse_opt_t*) thiz;
    cg_operand_t* y_hat  = operator->variant;
    cg_operand_t* labels = operator->labels;
    
    CG_DEBUG("differentiate @ %s\n", CG_NODE_ID(operator));
    if (variant == y_hat) {
        CG_DEBUG("delta %s = (%s - %s) X env_gradient\n", CG_NODE_ID(y_hat), CG_NODE_ID(y_hat), CG_NODE_ID(labels));
    } else if (variant == labels) {
        CG_DEBUG("delta %s is useless\n", CG_NODE_ID(labels));
    } else {
        CG_DEBUG("ERROR! %s is not belong %s\n", CG_NODE_ID(variant), CG_NODE_ID(operator));
    }
    return 0;
}

static inline mse_opt_t* mse_opt_create(cg_operator_t* thiz, const char* id, cg_operand_t* variant, cg_operand_t* labels) {

    mse_opt_t* operator = (mse_opt_t*) malloc (sizeof(mse_opt_t));

    cg_operator_init(operator, id, );

    operator->variant   = variant;
    operator->labels    = labels;
    return operator;
}

#endif