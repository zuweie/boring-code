/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-03-14 11:35:43
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-08-23 00:13:25
 * @FilePath: /boring-code/src/deep_learning/nn/nn.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __DNN_H__
#define __DNN_H__

#include "deep_learning/compute_graph2/cg_allocator.h"

typedef struct cg_tensor     cg_tensor_t;
typedef struct dnn           dnn_t;
typedef struct dnn_optimizer dnn_optimizer_t;

struct dnn {
    cg_allocator_t  alloc;

    nn_operand_t*   _Output;
    nn_operand_t*   _Input;
    nn_operand_t*   _Loss;

    cg_list_t*     recycle_list;
    
    // 组网用
    cg_list_t*      build_stack;
    int             node_count;
};

struct dnn_optimizer {

    int    batch;
    int    max_iter;
    int    err_stable;

    float  epsilon;
    float  alpha;
    
    cg_tensor_t* lables;
    cg_tensor_t* train_datas;

    // prepaer train branch
    int(*prepare)(dnn_t* nn, dnn_optimizer_t*);
    // end train loop indication
    int(*term)   (dnn_t* nn);
    // processing
    int(*processing) (dnn_t* nn);
}


int dnn_init(dnn_t* nn);
int dnn_reset(dnn_t* nn);

int dnn_fit(dnn_t* nn, dnn_optimizer_t* package);
cg_tensor_t* dnn_predict(dnn_t*, cg_tensor_t* feature);

/* build neual net work */
// 线性运算
linear_opt_t*  nn_linear(nn_t* nn, int in_dimens, int* out_dimens);
// relu 激活
relu_opt_t*    nn_relu(nn_t* nn);
// softmax
softmax_opt_t* nn_softmax(nn_t* nn);
// most square error
mse_opt_t*     nn_mse_loss(nn_t* nn);
// corss entropy
crossentropy_opt_t* nn_crossentropy(nn_t* nn);

#endif