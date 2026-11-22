/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-09-17 22:15:14
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-11-22 20:15:58
 * @FilePath: /boring-code/src/deep_learning/nn_library/nn_operator_multi_head_attention.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __NN_OPERATOR_MULTI_HEAD_ATTENTION_H__
#define __NN_OPERATOR_NULTI_HEAD_ATTENTION_H__

#include "deep_learning/compute_graph2/cg_debug.h"
#include "deep_learning/compute_graph2/cg_operator.h"
#include "nn_operand.h"

typedef struct multi_head_opt{

    cg_operand_t operator;
    nn_operand_t* x;
    nn_operand_t* Q;
    nn_operand_t* K;
    nn_operand_t* V;
    int head_number;

} mulit_head_opt_t;


static inline int __multi_head_cal (cg_operator_t* thiz, cg_operand_t* out) 
{
    CG_DEBUG("calculate @ %s\n", CG_NODE_ID(thiz));
    return 0;
}

static inline int __multi_head_diff (cg_operator_t* thiz, cg_operand_t* variant, cg_operand_t* in_gards) 
{
    CG_DEBUG("diff @ %s\n", CG_NODE_ID(thiz));
    return 0;
}

static inline int multi_head_opt_t* multi_head_create(const char* id, nn_operand_t* variant, nn_operand_t* Q, nn_operand_t* K, nn_operand_t* V, int head_number)
{
    mulit_head_opt_t* operator = (multi_head_opt_t*) malloc (sizeof(multi_head_opt_t));
    cg_operator_init(operator, id, __multi_head_cal, __multi_head_diff, NULL);
    operator->x = variant;
    operator->Q = Q;
    operator->K = K;
    operator->V = V;
    operator->head_number = head_number
    return operator;
}


#endif