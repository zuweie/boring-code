#ifndef __NN_OPERATOR_EMBEDDING_H__
#define __NN_OPERATOR_EMBEDDING_H__

#include "deep_learning/compute_graph2/cg_operator.h"

typedef struct embedding_opt
{
    cg_operator_t operator;
} embedding_opt_t;

static inline int __embedding_cal (cg_operator_t* thiz, cg_operand_t* out) 
{
    return 0;
}

static inline int __embedding_diff (cg_operator_t* thiz, cg_operand_t* variant, cg_operand_t* in_gards) 
{
    return 0;
}

static inline embedding_opt_t* embedding_opt_create( const char* id )
{
    
}

#endif