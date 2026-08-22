/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-03-15 13:06:19
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-08-22 22:42:38
 * @FilePath: /boring-code/src/deep_learning/nn_operator/linear_opt.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __NN_OPERATOR_LINEAR_H__
#define __NN_OPERATOR_LINEAR_H__

#include "deep_learning/compute_graph2/cg_debug.h"
#include "deep_learning/compute_graph2/cg_list.h"
#include "deep_learning/compute_graph2/cg_tensor.h"
#include "deep_learning/nn_operand/nn_operand.h"
#include "deep_learning/compute_graph2/cg_operator.h"

typedef struct nn_operand nn_operand_t;

typedef struct linear_opt {

    cg_operator_t operator;
    nn_operand_t* _Input;
    nn_operand_t* W;
    nn_operand_t* b;

} linear_opt_t;


static inline int __calculate(cg_node_t* thiz, cg_node_t* out)
{
    cg_operand_t* out_operand = (cg_operand_t*) out;
    linear_opt_t* operator    = (linear_opt_t*) thiz;

    nn_operand_t* x        = operator->x;
    nn_operand_t* W        = operator->W;
    nn_operand_t* b        = operator->b;

    CG_DEBUG("calculate @ %s operator:\n", CG_NODE_ID(operator));
    CG_DEBUG(" %s = %s X %s + %s\n", CG_NODE_ID(out_operand), CG_NODE_ID(W), CG_NODE_ID(x), CG_NODE_ID(b));
    return 0;
}

static inline int __differentiate(cg_node_t* thiz,  cg_node_t* variant, cg_node_t in_gard)
{
    linear_opt_t* operator = (linear_opt_t*) thiz;
    nn_operand_t* x        = operator->x;
    nn_operand_t* W        = operator->W;
    nn_operand_t* b        = operator->b;
    
    CG_DEBUG("differentinate @ %s operator\n", CG_NODE_ID(operator))
    if (variant == x) {
        
        CG_DEBUG("delta %s = (%s)^T X env_gradient\n", CG_NODE_ID(x), CG_NODE_ID(W));

    } else if (variant == W) {

        CG_DEBUG("delta %s = (%s)^T X env_gradient\n", CG_NODE_ID(W), CG_NODE_ID(x));

    } else if (variant == b) {

        CG_DEBUG("delta %s = 1 X env_gradient\n", CG_NODE_ID(b));

    } else {

        CG_DEBUG("Error! object to been different is no belong %s\n", CG_NODE_ID(operator));

    }
    return 0;
}

static inline linear_opt_t* linear_opt_create(const char* id, nn_operand_t* _Input, nn_operand_t* W, nn_operand_t* b) 
{
    linear_opt_t* linear_opt = (linear_opt_t*) malloc (sizeof(linear_opt_t));

    cg_operator_init(linear_opt, id, __calculate, __differentiate, NULL);

    *linear_opt = (linear_opt_t) {
        ._Input  = _Input,
        .W       = W,
        .b       = b
    };
    return linear_opt;
}

#endif