#ifndef __NN_OPERATOR_EMBEDDING_H__
#define __NN_OPERATOR_EMBEDDING_H__

#include "deep_learning/compute_graph2/cg_operator.h"
#include "deep_learning/compute_graph2/cg_debug.h"
#include "nn_operand.h"

typedef struct embedding_opt{
    cg_operator_t operator;
    nn_operand_t* x;
} embedding_opt_t;

static inline int __embedding_cal (cg_operator_t* thiz, cg_operand_t* out) 
{
    CG_DEBUG("calculate @ %s\n", CG_NODE_ID(thiz));
    CG_DEBUG("%s = embedding_cal(%s)\n", CG_NODE_ID(out), ((embedding_opt_t*)thiz)->x);
    return 0;
}

static inline int __embedding_diff (cg_operator_t* thiz, cg_operand_t* variant, cg_operand_t* in_gards) 
{
    CG_DEBUG("diff @ %s\n", CG_NODE_ID(thiz));
    CG_DEBUG("delta %s = delta %s X env_griden(%s)\n", CG_NODE_ID(variant), CG_NODE_ID(in_gards));
    return 0;
}

static inline embedding_opt_t* embedding_opt_create( const char* id, nn_operand_t* variant)
{
    embedding_opt* operator = (embedding_opt*) malloc (sizeof(embedding_opt));
    operator->x = variant;
    cg_operator_init(operator, id, __embedding_cal, __embedding_diff, NULL);
    return operator;
}

#endif