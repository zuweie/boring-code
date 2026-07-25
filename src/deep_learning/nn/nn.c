/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-03-14 11:35:50
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-07-25 23:17:50
 * @FilePath: /boring-code/src/deep_learning/nn/nn.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdlib.h>
#include <stdio.h>
#include "deep_learning/compute_graph2/cg_allocator.h"
#include "deep_learning/compute_graph2/cg_list.h"
#include "deep_learning/compute_graph2/cg_tensor.h"
#include "deep_learning/compute_graph2/cg_operand.h"
#include "deep_learning/compute_graph2/cg_operator.h"
#include "deep_learning/compute_graph2/cg_graph.h"
#include "nn.h"


static char* __gen_id(nn_t* nn, const char* prefix) 
{
    snprintf(nn->temp_id, 64, "%s_%d", prefix, nn->zonde_count);
    return nn->temp_id;
}
static cg_operand_t* __create_operand(nn_t* nn, const char* id, int in_dims, int out_dims)
{
    cg_operand_t* operand = (cg_operand_t*) malloc (sizeof(cg_operand_t));
    cg_operand_init(operand, id);
    operand->x  = cg_tensor_create(nn->alloc, 2, in_dims, out_dims);
    operand->Gx = cg_tensor_create(nn->alloc, 2, in_dims, out_dims);
    // 集中起来统一销毁
    cg_list_push(nn, operand);
    return operand;
}

static int __recycle_operand(cg_operand_t* operand)
{
    cg_tensor_recycle(operand->x);
    cg_tensor_recycle(operand->Gx);

    cg_operand_reset(operand);
    free(operand);
    return 0;
}

static cg_operator_t* __create_operator(nn_t* nn, const char* id)
{
    return NULL;
}

static int __recycle_operator(cg_operator_t* operator) 
{
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
        .operators    = cg_list_create(),
        .build_stack  = cg_list_create()
    };
    cg_allocator_init(&nn->alloc);
    return 0;
}


int nn_reset(nn_t* nn)
{
    cg_allocator_reset(&nn->alloc);
    cg_list_recycle(nn->build_stack, NULL);
    cg_list_recycle(nn->operands, __recycle_operand);
    cg_list_recycle(nn->operators, NULL);

    free(nn->alloc);
    return 0;
}

int nn_linear(nn_t* nn, int in_dimens, int out_dimens)
{
    // check build stack;
    
    cg_operand_t* x =  cg_list_is_empty(nn->build_stack) ? __create_operand(nn, __gen_id(nn, "x")) : cg_list_pop(nn->build_stack);
    cg_operand_t* W = __create_operand(nn, __gen_id(nn, "W"));
    cg_operand_t* b = __create_operand(nn, __gen_id(nn, "b"));
    
    return 0;
}

int nn_relu(nn_t* nn)
{

}
int nn_mse_loss(nn_t* nn)
{

}
int nn_crossentropy(nn_t* nn)
{

}
int nn_softmax(nn_t* nn)
{

}