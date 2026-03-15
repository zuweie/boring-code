/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-03-14 11:35:43
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-03-15 11:26:58
 * @FilePath: /boring-code/src/deep_learning/nn/nn.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __NN_H__
#define __NN_H__

#include "deep_learning/compute_graph2/cg_allocator.h"

typedef struct cg_allocator cg_allocator_t;
typedef struct cg_tensor    cg_tensor_t;

typedef struct nn {

    int    input_dimens;
    int    output_dimens;

    int    batch;
    int    max_iter;
    int    err_stable;
    int    znode_count;

    float  epsilon;
    float  alpha;

    cg_allocator_t alloc;

    cg_operand_t*   labels;
    cg_operand_t*   output;
    cg_operand_t*   input;
    cg_operand_t*   loss;

    cg_list_t*      operands;
    cg_list_t*      operators;

    // use to build compute graph 
    cg_list_t*      build_stack;
    cg_list_t*      import_operands;
    cg_list_t*      export_operands;
    char            temp_id[64];

} nn_t;

int nn_int(nn_t* nn, int batch, int max_iter, int err_stable, float epsilon, float alpah);
int nn_reset(nn_t* nn);
int nn_linear(nn_t* int in_dimens, int out_dimens);
int nn_relu(nn_t* nn);
int nn_mse_loss(nn_t* nn);
int nn_crossentropy(nn_t* nn);
int nn_softmax(nn_t* nn);

int nn_forward(nn_t* nn);
int nn_backward(nn_t* nn);
int nn_train(nn_t* nn);

#endif