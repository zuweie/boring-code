/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-03-14 11:35:43
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-07-26 17:36:50
 * @FilePath: /boring-code/src/deep_learning/nn/nn.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __NN_H__
#define __NN_H__

#include "deep_learning/compute_graph2/cg_allocator.h"

typedef struct cg_allocator cg_allocator_t;
typedef struct cg_tensor    cg_tensor_t;
typedef struct nn           nn_t;
typedef struct nn_train_package nn_train_package_t;

struct nn {
    cg_allocator_t  alloc;
    nn_operand_t*   labels;
    nn_operand_t*   output;
    nn_operand_t*   input;
    nn_operand_t*   loss;

    cg_list_t*      operands;
    cg_list_t*      operators;
    int             znode_count;

    nn_train_package_t* train_package;
};

struct nn_train_package {

    int    batch;
    int    max_iter;
    int    err_stable;

    float  epsilon;
    float  alpha;

    cg_tensor_t* train_datas;
    cg_tensor_t* labels;
    
    int(*prepare)(nn_t* nn);
    int(*termin) (nn_t* nn);
}


int nn_init(nn_t* nn);
int nn_reset(nn_t* nn);
int nn_set_train_package(nn_t* nn, nn_train_package_t* package);
nn_operand_t* nn_linear(nn_t* nn, int out_dimens, nn_operand_t* _Input);
nn_operand_t* nn_relu(nn_t* nn, nn_operand_t* _Input);
nn_operand_t* nn_mse_loss(nn_t* nn, nn_operand_t* _Input, nn_operand_t* labels);
nn_operand_t* nn_crossentropy(nn_t* nn, nn_operand_t* _Input, nn_operand_t* labels);
nn_operand_t* nn_softmax(nn_t* nn, nn_operand_t* _Input);

#endif