/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-09-17 22:13:03
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-11-29 00:03:52
 * @FilePath: /boring-code/src/deep_learning/nn_library/nn_operator_embedding.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __NN_OPERATOR_EMBEDDING_H__
#define __NN_OPERATOR_EMBEDDING_H__

#include "deep_learning/compute_graph2/cg_operator.h"
#include "deep_learning/compute_graph2/cg_debug.h"
#include "nn_operand.h"

typedef struct embedding_opt{
    cg_operator_t operator;
    nn_operand_t* x;
} embedding_opt_t;

static inline int __embedding_cal (cg_operator_t* thiz, cg_operand_t* out) 
{
    CG_DEBUG("calculate @ %s\n", CG_NODE_ID(thiz));
    CG_DEBUG("%s = embedding_cal(%s)\n", CG_NODE_ID(out), ((embedding_opt_t*)thiz)->x);
    return 0;
}

static inline int __embedding_diff (cg_operator_t* thiz, cg_operand_t* variant, cg_operand_t* in_gards) 
{
    CG_DEBUG("diff @ %s\n", CG_NODE_ID(thiz));
    CG_DEBUG("delta %s = delta %s X env_griden(%s)\n", CG_NODE_ID(variant), CG_NODE_ID(in_gards));
    return 0;
}

static inline embedding_opt_t* embedding_opt_create( const char* id, nn_operand_t* variant)
{
    embedding_opt_t* operator = (embedding_opt_t*) malloc (sizeof(embedding_opt_t));
    cg_operator_init(operator, id, __embedding_cal, __embedding_diff, NULL);
    operator->x = variant;
    return operator;
}

#endif