/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-03-14 11:35:43
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-08-30 09:33:32
 * @FilePath: /boring-code/src/deep_learning/nn/nn.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __DNN_H__
#define __DNN_H__

#include "deep_learning/compute_graph2/cg_allocator.h"

typedef struct cg_list       cg_list_t;
typedef struct cg_tensor     cg_tensor_t;
typedef struct nn_operand    nn_operand_t;
typedef struct dnn_optimizer dnn_optimizer_t;

typedef struct dnn {
    // 非常简单的内存配置器
    cg_allocator_t  alloc;
    nn_operand_t*   _Output;
    nn_operand_t*   _Input;
    nn_operand_t*   _Loss;

    cg_list_t*      nodes_list;
    // 组网用
    cg_list_t*      build_stack;
    int             node_count;
} dnn_t;

int dnn_init(dnn_t* nn);
int dnn_reset(dnn_t* nn);

int dnn_fit(dnn_optimizer_t* optimizer);

cg_tensor_t* dnn_predict(dnn_t*, cg_tensor_t* feature);

/* build neual net work */
// 线性运算
int dnn_linear(dnn_t* nn, int in_dimens, int out_dimens);
// relu 激活
int dnn_relu(dnn_t* nn);
// mse loos
int dnn_mse(dnn_t* nn);
// softmax output + crxenotrpy loss
int dnn_softcrx(dnn_t* nn);
// softmax output
int dnn_softmax(dnn_t* nn);

#endif