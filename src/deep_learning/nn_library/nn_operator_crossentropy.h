/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-03-15 20:37:57
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-08-30 12:37:02
 * @FilePath: /boring-code/src/deep_learning/nn_operators/crossentropy_opt.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __NN_OPERATOR_CROSSENTROPY_H__
#define __NN_OPERATOR_CROSSENTROPY_H__

#include "deep_learning/compute_graph2/cg_debug.h"
#include "deep_learning/compute_graph2/cg_operator.h"
#include "nn_operand.h"

typedef struct nn_operand nn_operand_t;

typedef struct crossentropy_opt {

    cg_operator_t operator;
    nn_operand_t* x;
    nn_operand_t* labels;

} crossentropy_opt_t;

static inline int __calculate(cg_node_t* thiz, cg_node_t* out)
{
    crossentropy_opt_t* operator   = (crossentropy_opt_t*) thiz;
    nn_operand_t* x       = operator->x;
    nn_operand_t* labels  = operator->labels;

    CG_DEBUG("calculate @ %s\n", CG_NODE_ID(operator));
    CG_DEBUG("%s = (%s - %s)^2 \n", CG_NODE_ID(out), CG_NODE_ID(x), CG_NODE_ID(labels));
    return 0;
}

static inline int __differentiate(cg_node_t* thiz, cg_node_t* variant, cg_ref_t env_gradient)
{
    crossentropy_opt_t* operator = (crossentropy_opt_t*) thiz;
    nn_operand_t* x      = operator->x;
    nn_operand_t* labels = operator->labels;
    
    CG_DEBUG("differentiate @ %s\n", CG_NODE_ID(operator));
    if (variant == x) {
        CG_DEBUG("delta %s = (%s - %s) X env_gradient\n", CG_NODE_ID(x), CG_NODE_ID(x), CG_NODE_ID(labels));
    } else if (variant == labels) {
        CG_DEBUG("delta %s is useless\n", CG_NODE_ID(labels));
    } else {
        CG_DEBUG("ERROR! %s is not belong %s\n", CG_NODE_ID(variant), CG_NODE_ID(operator));
    }
    return 0;
}

static inline crossentropy_opt_t* crossentropy_opt_create( const char* id, nn_operand_t* variant, nn_operand_t* labels) {

    crossentropy_opt_t* operator = (crossentropy_opt_t*) malloc (sizeof(crossentropy_opt_t));

    cg_operator_init(operator, id, __calculate, __differentiate, NULL);

    operator->x       = variant;
    operator->labels  = labels;
    return operator;
}

#endif