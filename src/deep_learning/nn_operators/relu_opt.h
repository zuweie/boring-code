#ifndef __RELU_OPT_H__
#define __RELU_OPT_H__

#include "deep_learning/compute_graph2/cg_debug.h"
#include "deep_learning/compute_graph2/cg_operator.h"
#include "deep_learning/compute_graph2/cg_operand.h"

typedef struct relu_opt
{
    cg_opreator_t operator;
    cg_operand_t* variant;

} relu_opt_t;

static inline int __calculate(cg_node_t* thiz, cg_node_t* out) 
{
    relu_opt* operator         = (cg_operator_t*) thiz;
    cg_operand_t*  out_operand = (cg_operand_t*) out;
    cg_operand_t*  variant     = operator->variant;

    CG_DEBUG("calculate @ %s: \n", CG_NODE_ID(operator));
    CG_DEBUG("%s = relu(%s) \n", CG_NODE_ID(out_operand), CG_NODE_ID(variant));
    return 0;
}

static inline int __differentiate(cg_node_t* thiz, cg_node_t* variant, cg_ref_t env_gradient)
{
    relu_opt* operator = (cg_operator_t*) thiz;

    CG_DEBUG("differentiate @ %s: \n", CG_NODE_ID(operator));
    if (variant == operator->variant) {
        CG_DEBUG("delta %s = delta relu (%s) ⊙ env_gradient\n", CG_NODE_ID(operator->variant), CG_NODE_ID(operator->variant));

    } else {
        CG_DEBUG("ERROR! %s is not belong %s \n", CG_NODE_ID(variant), CG_NODE_ID(operator));
        return -1;
    }
    return 0;
}

static inline relu_opt_t* relu_opt_create(cg_operator_t* thiz, const char* id, cg_operand_t* variant)
{
    relu_opt_t* relu_opt = (relu_opt_t*) malloc (sizeof(relu_opt_t));
    relu_opt->variant    = variant;
    cg_operator_init(relu_opt, id, __calculate, __differentiate);
    return relu_opt;
}

static inline int relut_opt_recycle(cg_operator_t* thiz) 
{
    cg_operator_reset(thiz);
    free(thiz);
    return 0;
}

#endif