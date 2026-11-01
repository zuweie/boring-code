/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-09-17 22:17:10
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-11-01 23:27:15
 * @FilePath: /boring-code/src/deep_learning/nn_library/nn_operator_position_encoding.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __NN_OPERATOR_ENCODING_H__
#define __NN_OPEARTOR_ENCODING_H__

#include "deep_learning/compute_graph2/cg_debug.h"
#include "deep_learning/compute_graph2/cg_operator.h"
#include "nn_operand.h"

typedef struct position_encoding_opt{

    cg_operand_t operator;
    nn_operand_t* x;

} position_encoding_opt_t;


static inline int __pos_encoding_cal (cg_operator_t* thiz, cg_operand_t* out) 
{
    CG_DEBUG("calculate @ %s\n", CG_NODE_ID(thiz));
    CG_DEBUG("%s = embedding_cal(%s)\n", CG_NODE_ID(out), ((embedding_opt_t*)thiz)->x);
    return 0;
}

static inline int __pos_encoding_diff (cg_operator_t* thiz, cg_operand_t* variant, cg_operand_t* in_gards) 
{
    CG_DEBUG("diff @ %s\n", CG_NODE_ID(thiz));
    CG_DEBUG("pos encoding(%s) diff do nothing \n", CG_NODE_ID(thiz));
    return 0;
}

static inline int position_encoding_opt_t* position_encoding_opt_create(const char* id, nn_operand_t* variant)
{
    position_encoding_opt_t* operator = (position_encoding_opt_t*) malloc (sizeof(position_encoding_opt_t));
    cg_operator_init(operator, id, __pos_encoding_cal, __pos_encoding_diff,  NULL);
    operator->x = variant;
    return operator;
}


#endif