/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-02-19 14:20:43
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-03-15 16:08:43
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_operand.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __CG_OPERAND_H__
#define __CG_OPERAND_H__

#include "cg_ref.h"
#include "cg_node.h"

typedef struct cg_operand {

    cg_node_t _base;
    cg_ref_t x;
    cg_ref_t Gx;
    
} cg_operand_t;

static inline int cg_operand_init(cg_operand_t* thiz, const char* id)
{
    cg_node_init(thiz, id, e_operand);
    return 0;
}   

static inline int cg_operand_reset(cg_operand_t* thiz) 
{
    return cg_node_reset(thiz);
}

static inline cg_node_t* cg_operand_get_operator(cg_operand_t* thiz) 
{
    if (CG_NODE_TYPE(thiz) == e_operand) {
        return cg_list_get(thiz->_base.vertex.in, 0);
    } else {
        CG_DEBUG("ERROR: cg_operand_get_operator, thiz is not a operand [%d]", __LINE__);
    }
    return NULL;
}
#endif