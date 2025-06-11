/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-06-11 11:11:57
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-06-11 16:23:49
 * @FilePath: /boring-code/src/deep_learning/cg_ann/cg_ann.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "deep_learning/compute_graph2/cg_znode.h"
#include "cg_ann.h"


int cg_ann_init(
    cg_ann_t* ann, 
    int hl_size, 
    int* hl, 
    int batch_size, 
    int max_iter, 
    int x_dimens, 
    int y_dimens, 
    loss_type_t loss_type, 
    float learning_rate, 
    float espilon)
{
    cg_init(&ann->compute_graph);
    ann->batch_size   = batch_size;
    ann->hidden_layer = hl;
    ann->hidden_layer_size = hl_size;
    ann->x_dimens = x_dimens;
    ann->y_dimens = y_dimens;
    ann->loss_type = loss_type;
    ann->learning_rate     = learning_rate;
    ann->max_iter = max_iter;
    ann->epsilon  = espilon;
    ann->step     = 0;
    return 0;
}

/**
 * @brief build compute graph
 * 
 * @param ann 
 * @param x_dimens 每条 x 数据的维度，例如 人（身高，体重，年龄，喜欢看动作片...)
 * @param y_dimens 每条 y 数据的维度，例如 男人（1， 0，0）、女人 （0，1，0）、人妖（0，0，1）
 * @param loss_type loss function 有 mse、corss entroy
 * @return int 
 */
int cg_ann_build(cg_ann_t* ann)
{
    // push a start tag to start build compute flow
    cg_flow_push(ann, cg_create_beg());
    cg_flow_push(ann, cg_create_zonde(ann, "x", e_x));
    cg_flow_push(ann, cg_create_end());
    return 0;
}

int cg_ann_train(cg_ann_t* ann);
int cg_ann_predict(cg_ann_t* ann, cg_tensor_t* input, cg_tensor_t* predict);