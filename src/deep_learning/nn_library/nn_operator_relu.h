/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-03-15 15:56:08
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-08-22 22:43:46
 * @FilePath: /boring-code/src/deep_learning/nn_operator/relu_opt.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __NN_OPERATOR_RELU_H__
#define __NN_OPERATOR_RELU_H__

#include "deep_learning/compute_graph2/cg_debug.h"
#include "deep_learning/compute_graph2/cg_operator.h"
#include "deep_learning/nn_operand/nn_operand.h"

typedef struct relu_opt
{
    cg_opreator_t operator;
    nn_operand_t* _Input;

} relu_opt_t;

static inline int __calculate(cg_node_t* thiz, cg_node_t* out) 
{
    relu_opt_t* operator       = (cg_operator_t*) thiz;
    nn_operand_t*  out_operand = (cg_operand_t*) out;
    //nn_operand_t*  _Input      = operator->_Input;

    CG_DEBUG("calculate @ %s: \n", CG_NODE_ID(operator));
    CG_DEBUG("%s = relu(%s) \n", CG_NODE_ID(out_operand), CG_NODE_ID(operator->_Input));
    return 0;
}

static inline int __differentiate(cg_node_t* thiz, cg_node_t* variant, cg_ref_t env_gradient)
{
    relu_opt_t* operator = (cg_operator_t*) thiz;

    CG_DEBUG("differentiate @ %s: \n", CG_NODE_ID(operator));
    if (variant == operator->variant) {
        CG_DEBUG("delta %s = delta relu (%s) ⊙ env_gradient\n", CG_NODE_ID(operator->variant), CG_NODE_ID(operator->variant));

    } else {
        CG_DEBUG("ERROR! %s is not belong %s \n", CG_NODE_ID(variant), CG_NODE_ID(operator));
        return -1;
    }
    return 0;
}

static inline relu_opt_t* relu_opt_create(const char* id, nn_operand_t* variant)
{
    relu_opt_t* relu_opt = (relu_opt_t*) malloc (sizeof(relu_opt_t));
    relu_opt->variant    = variant;
    cg_operator_init(relu_opt, id, __calculate, __differentiate, NULL);
    return relu_opt;
}


#endif