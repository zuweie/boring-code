#ifndef __CG_OPERAND_H__
#define __CG_OPERAND_H__

#include "cg_ref.h"
#include "cg_node.h"

typedef struct cg_operand {

    cg_node_t _base;
    cg_ref_t x;
    cg_ref_t Gx;
    
} cg_operand_t;

static inline int cg_operand_init(
    cg_operand_t* thiz, 
    const char* id,
) 
{
    cg_node_init(&thiz->_base, id);
    return 0;
}   

static inline int cg_operand_reset(cg_operand_t* thiz) 
{
    cg_node_reset(&thiz->_base);
    return 0;
}

static inline cg_node_t* cg_operand_get_operator(cg_operand_t* thiz) 
{
    cg_list_get(thiz->_base.vertex.in, 0)
}
#endif