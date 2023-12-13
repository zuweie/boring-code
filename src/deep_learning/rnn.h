/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-11-29 15:56:34
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-12-13 15:37:50
 * @FilePath: /boring-code/src/deep_learning/rnn.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __RNN_H__
#define __RNN_H__
#include "matrix2/matrix2.h"
#include "vtype/vfloat_type.h"

typedef struct {

    int (*active)(matrix2_t* vec, double* params);
    double* params;

} rnn_active_t;

typedef struct {

    int (*d_active)(matrix2_t* vec, double* params);
    double* params;

} rnn_d_active_t;

typedef struct {

    // 训练最大次数
    int max_iter;
    // 训练结束精度
    double term_epsilon;
    // 梯度下降率
    double learning_rate;
    // 隐藏层神经元数量
    int hidden_layer_cells_numbers;

} rnn_param_t;

typedef struct {

    matrix2_t* _W_xh;
    matrix2_t* _W_hy;
    matrix2_t* _W_hh;

} rnn_sync_model_t;

int rnn_sync_train(matrix2_t* seq_data, matrix2_t* seq_label, rnn_param_t* rnn_params, matrix2_t* out_W_xh, matrix2_t* out_W_hh, matrix2_t* out_W_hy, void (*progress)(char*, unsigned long, unsigned long, double));
int rnn_sync_predict(matrix2_t* seq_inputs, matrix2_t* w_xh, matrix2_t* w_hh, matrix2_t* w_hy, matrix2_t** seq_outpts);

#endif