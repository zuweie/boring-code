/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-06-11 11:12:02
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-06-14 08:22:33
 * @FilePath: /boring-code/src/deep_learning/cg_ann/cg_ann.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __CG_ANN_H__
#define __CG_ANN_H__
#include "deep_learning/compute_graph2/cg_znode_base.h"
#include "deep_learning/compute_graph2/cg_base.h"

typedef enum {
    e_mse = 1,
    e_cross_entroy
} loss_type_t;

typedef struct cg_ann {
    cg_base_t  cg_base;
    int   hidden_layer_size;
    int*  hidden_layer;
    int   batch_size;
    int   max_epoch;
    int   step;
    int   x_dimens;
    int   y_dimens;
    loss_type_t loss_type;
    float learning_rate;
    float epsilon;
    cg_list_t* znode_list;
    // 记下几个关键的节点，使得操作起来比较方便
    cg_znode_t* x_node;
    cg_znode_t* y_node;
    cg_znode_t* y_hat_node;
    cg_znode_t* loss_node;
    cg_allocator_t alloc;
}cg_ann_t;

int cg_ann_init(cg_ann_t* ann, int hl_size, int* hl, int batch_size, int max_iter, int x_dimens, int y_dimens, loss_type_t loss_type, float learning_rate, float espilon);
int cg_ann_reset(cg_ann_t* ann);
int cg_ann_build(cg_ann_t* ann);
int cg_ann_train(cg_ann_t* ann, cg_tensor_t* X_data, cg_tensor_t* Y_lable);
int cg_ann_predict(cg_ann_t* ann, cg_tensor_t* input, cg_tensor_t* predict);

#endif