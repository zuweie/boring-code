/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-03-14 11:35:43
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-08-21 22:01:44
 * @FilePath: /boring-code/src/deep_learning/nn/nn.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __NN_H__
#define __NN_H__

#include "deep_learning/compute_graph2/cg_allocator.h"

typedef struct cg_tensor       cg_tensor_t;
typedef struct nn              nn_t;
typedef struct nn_fit_package  nn_fit_package_t;

struct nn {
    cg_allocator_t  alloc;

    nn_operand_t*   _Output;
    nn_operand_t*   _Input;
    nn_operand_t*   _Loss;
    
    cg_tensor_t*   train_datas;
    cg_tensor_t*   labels;

    cg_list_t*      operands;
    cg_list_t*      operators;
    int             znode_count;
};

struct nn_fit_package {

    int    batch;
    int    max_iter;
    int    err_stable;

    float  epsilon;
    float  alpha;

    // prepaer train branch
    int(*prepare)(nn_t* nn);
    // end train loop indication
    int(*term)   (nn_t* nn);
    // processing
    int(*processing) (nn_t* nn);
}


int nn_init(nn_t* nn);
int nn_reset(nn_t* nn);

int nn_fit(nn_t* nn, nn_fit_package_t* package);
int nn_predict(nn_t*, nn_operand_t* _Input);

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