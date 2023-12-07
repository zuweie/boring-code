/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-11-29 15:56:34
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-12-07 13:15:33
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
    double learn_rate;
    // 隐藏层的数量
    int hidden_layer_number;
    // 激活函数
    rnn_active_t act;
    // 激活函数倒数
    rnn_d_active_t d_act;

} rnn_param_t;

int rnn_sync_train(matrix2_t* seq_data, matrix2_t* seq_label, rnn_param_t* rnn_params, int epochs, matrix2_t** out_Wbs, int* out_Wbs_length, matrix2_t** out_U matrix2_t** out_V)
int rnn_sync_predict(matrix2_t* _Input, matrix2_t** Wbs, int Wbs_length, matrix2_t* U, matrix2_t* V);

#endif