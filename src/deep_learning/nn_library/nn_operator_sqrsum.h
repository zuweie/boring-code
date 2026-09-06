#ifndef __NN_OPERATOR_SQRSUM_H__
#define __NN_OPERATOR_SQRSUM_H__

#include "deep_learning/compute_graph2/cg_debug.h"
#include "deep_learning/compute_graph2/cg_operator.h"
#include "nn_operand.h"

typedef struct  sqrsum_opt
{
    cg_operator_t operator;
    float lamada;
    nn_operand_t* x;

} sqrsum_opt_t;

static inline int __sqrsum_calc(cg_operator_t* thiz, cg_operand_t* out)
{
    nn_operand_t* x = ((nn_operand_t*) thiz)->x;
    CG_DEBUG("calculate @ %s: \n %s = \\lamada * sqr( %s * %s) \n", CG_NODE_ID(thiz), CG_NODE_ID(out), CG_NODE_ID(x), CG_NODE_ID(x));
    return 0;
}

static inline __sqrsum_diff(cg_operator_t* thiz, cg_operand_t* variant, cg_operand_t* in_gard)
{
    nn_operand_t* x = ((nn_operand_t*) thiz)->x;
    CG_DEBUG("differentiate @ %s :\n delta %s = lamada * 2 * %s\n", CG_NODE_ID(thiz), CG_NODE_ID(x), CG_NODE_ID(x));
    return 0;
}

static inline sqrsum_opt_t* sqrsum_opt_create(const char* id, cg_operand_t* variant, float lamada) 
{
    sqrsum_opt_t* sqrsum_opt = (sqrsum_opt_t*) malloc (sizeof(sqrsum_opt_t));
    sqrsum_opt->x            = variant;
    sqrsum_opt->lamada       = lamada;
    cg_operator_init(sqrsum_opt, id, __sqrsum_calc, __sqrsum_diff, NULL);
    return sqrsum_opt;
}

#endif
