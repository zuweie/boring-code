#ifndef __CG_OPERAND_H__
#define __CG_OPERAND_H__

#include "cg_ref.h"
#include "cg_node.h"

typedef struct cg_operand {

    cg_node_t _base;
    cg_ref_t x;
    cg_ref_t Gx;
    int version;
} cg_operand_t;

static inline int cg_operand_init(
    cg_operand_t* thiz, 
    const char* id, 
    cg_ref_t (*operand)(cg_node_t*), 
    cg_ref_t (*gradient)(cg_node_t*)
) 
{
    cg_node_init(&thiz->_base, id);
    *thiz = (cg_operand_t) {
        .operand  = operand,
        .gradient = gradient 
    };
    return 0;
}   

static inline int cg_operand_reset(cg_operand_t* thiz) 
{
    cg_node_reset(&thiz->_base);
    return 0;
}

#endif