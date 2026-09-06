#ifndef __NN_OPERATOR_SUM_H__
#define __NN_OPERATOR_SUM_H__

#include "deep_learning/compute_graph2/cg_debug.h"
#include "deep_learning/compute_graph2/cg_list.h"
#include "deep_learning/compute_graph2/cg_operator.h"
#include "nn_operand.h"

typedef struct sum_opt
{
    cg_operator_t operator;
    cg_list_t* xs;
    
} sum_opt_t;

static inline int __sum_reset(cg_operator_t* thiz) 
{
    return cg_list_recycle( ((sum_opt_t*)thiz)->xs );
}

static inline int __sum_calc(cg_operator_t* thiz, cg_operand_t* out)
{
    sum_opt_t* thiz_opt  = thiz;
    nn_operand_t x       = NULL;
    CG_DEBUG("calculate @ %s:\n %s = ", CG_NODE_ID(thiz), CG_NODE_ID(out));
    cg_list_node_t* first = CG_LIST_TOP(thiz_opt->xs);
    while(first != CG_LIST_HEAD(thiz_opt->xs)) {
        
        x = first->ref;
        if (first->prev != CG_LIST_HEAD(thiz_opt->xs)) {
            CG_DEBUG( "%s + ", CG_NODE_ID(x));
        } else {
            CG_DEBUG("%s \n", CG_NODE_ID(x) );
        }
        first = first->prev;
    }
    return 0;
}

static inline int __sum_diff(cg_operator_t* thiz, cg_operand_t* variant, cg_operand_t* in_gard)
{
    CG_DEBUG("differentiate @ %s: delta %s = 1 X in_gard(%s) \n", CG_NODE_ID(thiz), CG_NODE_ID(variant), CG_NODE_ID(in_gard));
    return 0;
}

static inline int sum_opt_add_variant(sum_opt_t* opt, nn_operand_t* variant) 
{
    return cg_list_push(opt->xs, variant);
}

static inline sum_opt_t* sum_opt_create(const char* id) 
{
    sum_opt_t* sum_opt = (sum_opt_t*) malloc (sizeof(sum_opt_t));
    sum_opt->xs        = cg_list_create();
    cg_operator_init(sum_opt, id, __sum_calc, __sum_diff, __sum_reset);
    return sum_opt;
}
#endif