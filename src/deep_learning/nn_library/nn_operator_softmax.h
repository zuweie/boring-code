/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-03-15 18:30:21
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-08-30 13:22:11
 * @FilePath: /boring-code/src/deep_learning/nn_operator/softmax_opt.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __NN_OPERATOR_SOFTMAX_H__
#define __NN_OPERATOR_SOFTMAX_H__

#include "deep_learning/compute_graph2/cg_debug.h"
#include "deep_learning/compute_graph2/cg_operator.h"
#include "nn_operand.h"

typedef struct softmax_opt
{
    cg_operator_t operator;
    nn_operand_t* x;

} softmax_opt_t;

static inline int __softmax_calc(cg_operator_t* thiz, cg_operand_t* out) 
{
    softmax_opt_t* operator    = (cg_operator_t*) thiz;
    //nn_operand_t*  out_opreand = (cg_operand_t*) out;
    nn_operand_t*  x           = operator->x;

    CG_DEBUG("calculate @ %s: \n",  CG_NODE_ID(operator));
    CG_DEBUG("%s = softmax(%s) \n", CG_NODE_ID(out), CG_NODE_ID(x));
    return 0;
}

static inline int __softmax_diff(cg_operator_t* thiz, cg_operand_t* variant, cg_operand_t* in_gard)
{
    softmax_opt_t* operator = (cg_operator_t*) thiz;

    CG_DEBUG("differentiate @ %s: \n", CG_NODE_ID(operator));
    if (variant == operator->x) {
        CG_DEBUG("delta %s = delta softmax(%s) X env_gradient", CG_NODE_ID(operator->x), CG_NODE_ID(operator->x));
    } else {
        CG_DEBUG("ERROR! %s is not belong %s\n", CG_NODE_ID(variant), CG_NODE_ID(operator));
        return -1;
    }
    return 0;
}

static inline softmax_opt_t* softmax_opt_create(const char* id, cg_operand_t* variant)
{
    softmax_opt_t* softmax_opt = (softmax_opt_t*) malloc (sizeof(softmax_opt_t));
    softmax_opt->x             = variant;
    cg_operator_init(softmax_opt, id, __softmax_calc, __softmax_diff, NULL);
    return softmax_opt;
}

#endif