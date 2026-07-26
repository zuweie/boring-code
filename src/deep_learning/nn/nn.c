/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-03-14 11:35:50
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-07-26 14:37:25
 * @FilePath: /boring-code/src/deep_learning/nn/nn.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdlib.h>
#include <stdio.h>
#include "deep_learning/compute_graph2/cg_allocator.h"
#include "deep_learning/compute_graph2/cg_list.h"
#include "deep_learning/compute_graph2/cg_tensor.h"
#include "deep_learning/compute_graph2/cg_graph.h"
#include "deep_learning/compute_graph2/cg_tensor_shape.h"
#include "deep_learning/compute_graph2/cg_debug.h"

#include "deep_learning/nnalloc/nn_alloc.h"
#include "deep_learning/nn_operand/nn_operand.h"
#include "deep_learning/nn_operator/linear_opt.h"
#include "deep_learning/nn_operator/relu_opt.h"
#include "deep_learning/nn_operator/mse_opt.h"
#include "deep_learning/nn_operator/crossentropy_opt.h"
#include "deep_learning/nn_operator/softmax_opt.h"
#include "nn.h"


static char* __gen_id(nn_t* nn, const char* prefix) 
{
    snprintf(nn->temp_id, 64, "%s@%d", prefix, nn->zonde_count);
    return nn->temp_id;
}

static int __recycle_operand(nn_operand_t* operand)
{
    cg_operand_reset(operand);
    free(operand);
    return 0;
}


static int __recycle_operator(cg_operator_t* operator) 
{
    cg_operator_reset(operator);
    free(operator);
    return 0;
}

int nn_init(nn_t* nn, int batch, int max_iter, int err_stable, float epsilon, float alpah)
{
    *nn = (nn_t) {
        .batch    = batch,
        .max_iter = max_iter,
        .epsilon  = epsilon,
        .alpha    = alpah,
        .err_stable  = err_stable,
        .znode_count = 0,

        .operands     = cg_list_create(),
        .operators    = cg_list_create()
    };
    cg_allocator_init(&nn->alloc);
    Set_alloc(&nn->alloc);
    
    return 0;
}


int nn_reset(nn_t* nn)
{
    cg_allocator_reset(&nn->alloc);
    Set_alloc(NULL);
    
    cg_list_recycle(nn->operands, __recycle_operand);
    cg_list_recycle(nn->operators, __recycle_operator);

    free(nn->alloc);
    return 0;
}

nn_operand_t* nn_linear(nn_t* nn, nn_operand_t* _Input, int out_dimens)
{
    int in_dimens = SHAPE_DIMENS(_Input->x->shape, 0);

    nn_operand_t* W = nn_operand_create(__gen_id(nn, "W"), out_dimens, in_dimens);
    nn_operand_t* b = nn_operand_create(__gen_id(nn, "b"), out_dimens, 1);
    nn_operand_t* z = nn_operand_create(__gen_id(nn, "_Z"), out_dimens, 1);
    linear_opt_t* linear_opt = linear_opt_create(__gen_id(nn, "linear_opt"),  _Input, W, b);

    // 做物理连接。
    cg_graph_link(_Input, linear_opt);
    cg_graph_link(W, linear_opt);
    cg_graph_link(b, linear_opt);
    cg_graph_link(linear_opt, z);

    // 将生成的节点收集起来，统一销毁。
    cg_list_push(nn->operands, W);
    cg_list_push(nn->operands, b);
    cg_list_push(nn->operands, z);
    cg_list_push(nn->operators, linear_opt);

    return z;
}

nn_operand_t* nn_relu(nn_t* nn, nn_operand_t* _Input)
{
    relu_opt_t* relu_opt = relu_opt_create(__gen_id(nn, "relu_opt"), _Input);
    nn_operand_t* z      = nn_operand_create(__gen_id(nn, "_Z"), SHAPE_DIMENS(_Input->x->shape, 0), SHAPE_DIMENS(_Input->x->shape, 1));

    cg_graph_link(relu_opt, z);

    // 将生成的节点收集起来，统一销毁。
    cg_list_push(nn->operands, z);
    cg_list_push(nn->operators, relu_opt);

    return z;
}

nn_operand_t* nn_mse_loss(nn_t* nn, nn_operand_t* _Input, nn_operand_t* labels)
{
    mse_opt_t* mse_opt = mse_opt_create(__gen_id(nn, "mse_loss_opt"), _Input, labels);
    nn_operand_t* z    = nn_operand_create(__gen_id(nn, "__Mse_loss"), 1, 1);
    cg_graph_link(mse_opt, z);

    cg_list_push(nn->operands, z);
    cg_list_push(nn->operators, mse_opt);

    return z;
}

nn_operand_t* nn_crossentropy(nn_t* nn, nn_operand_t* _Input, nn_operand_t* labels)
{
    crossentropy_opt_t* cross_opt = crossentropy_opt_create(__gen_id(nn, "cross_opt"), _Input, labels);
    nn_operand_t* z               = nn_operand_create(__gen_id(nn, "__Cross_loss"), 1, 1);
    cg_graph_link(cross_opt, z);

    cg_list_push(nn->operands, z);
    cg_list_push(nn->operators, cross_opt);
    return z;
}

nn_operand_t* nn_softmax(nn_t* nn, nn_operand_t* _Input)
{
    softmax_opt_t* soft_opt = softmax_opt_create(__gen_id(nn, "softmax_opt"), _Input);
    nn_operand_t* z         = nn_operand_create(__gen_id(nn, "_Input"), SHAPE_DIMENS(_Input->x->shape, 0), SHAPE_DIMENS(_Input->x->shape, 1));
    cg_graph_link(soft_opt, z);

    cg_list_push(nn->operands, z);
    cg_list_push(nn->operators, soft_opt);
    return z;
}

int nn_forward(nn_t* nn)
{
    
}

int nn_backward(nn_t* nn)
{
    
}

int nn_train(nn_t* nn)
{

}