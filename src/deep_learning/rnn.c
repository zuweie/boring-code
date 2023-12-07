/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-11-29 15:56:28
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-12-07 16:36:28
 * @FilePath: /boring-code/src/deep_learning/rnn.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <float.h>
#include "rnn.h"

static double vec_diff(matrix2_t* m1, matrix2_t* m2) 
{
    int m1_size = m1->rows * m1->cols;
    int m2_size = m2->rows * m2->cols;
    double diff = 0.f;
    if (m1_size == m2_size) {
        for (int i = 0; i<m1_size; ++i){
            diff += sqrt(((m1->pool[i] - m2->pool[i]) * (m1->pool[i] - m2->pool[i])));
        }
        return diff;
    }
    return -1;
}

/**
 * @brief 同步 RNN
 * 
 * @param data 训练数据
 * @param label 标签
 * @param rnn_params 训练参数
 * @param epochs 循环次数
 * @param out_Wbs 输出隐藏层参数
 * @param out_Wbs_length 隐藏层参数的数量
 * @param out_U 输出 U 权重参数
 * @param out_V 输出 V 权重参数
 * @return int 
 */
int rnn_sync_train(matrix2_t* seq_data, matrix2_t* seq_label, rnn_param_t* rnn_params, int epochs, matrix2_t** out_Wbs, int* out_Wbs_length, matrix2_t** out_Us, matrix2_t** out_V)
{

    int seq_length = seq_data->rows;
    // TOOD: 2 申请及初始化 U、 V、_Wbs 矩阵。
    //matrix2_t* _Xi   = Mat2_create(1,1);
    matrix2_t* _Yi   = Mat2_create(1,1);
    matrix2_t* _Y    = Mat2_create(1,1);

    matrix2_t* _V    = Mat2_create(seq_label->cols,seq_data->cols);
    matrix2_t* _U    = Mat2_create(1,1);

    matrix2_t* _Wb   = Mat2_create(1,1);
    matrix2_t* _input  = Mat2_create(1,1);

    matrix2_t** _Wbs = (matrix2_t**) malloc (rnn_params->hidden_layer_number * sizeof(matrix2_t*));
    matrix2_t** _Hts = (matrix2_t**) malloc (rnn_params->hidden_layer_number * sizeof(matrix2_t*));
    matrix2_t** _Us  = (matrix2_t**) malloc (rnn_params->hidden_layer_number * sizeof(matrix2_t*));

    // 初始化上述的一堆矩阵
    Mat2_fill_random(_V, 0, 1.f);
    Mat2_fill_random(_U, 0, 1.f);

    for (int i=0, j=1; i<rnn_params->hidden_layer_numbers; ++i) {
        _Wbs[i] = Mat2_create(seq_data->cols, seq_data->cols+1);
        Mat2_fill_random(_Wbs[i], 0, 1.f);

        _Hts[i] = Mat2_create(seq_data->cols, 1);
        Mat2_fill(_Hts[i], 0.f);

        _Us[i]  = Mat2_create(seq_data->cols, seq_data->cols);
        Mat2_fill_random(_Us[i], 0, 1.f); 
    }
    // 只要第一个为零矩阵即可。


    // TODO: 3 设计好向前传播，以及向后传播的训练。
    double error = FLT_MAX; 
    double last_error = 0.f;

    while (epochs-- > 0 ) {

        // TODO: 3.1 向前传播然后记录误差
        last_error = error;
        error = 0.f;

        for (int i=0; i<seq_length; ++i) {

            Mat2_slice_row_to(_input, seq_data, i);
            Mat2_T(_input);

            Mat2_slice_row_to(_Yi, seq_label, i);
            Mat2_T(_Yi);

            //Mat2_cpy(_input, _Xi);

            for (int j=0; j<rnn_params->hidden_layer_number; ++j) {
                
                Mat2_cpy(_Wb, _Wbs[j]);

                Mat2_cpy(_U, _Us[j]);

                Mat2_paddind_bottom(_input, 1, 1.f);

                Mat2_dot(_Wb, _input);

                Mat2_dot(_U, _Hts[j]);

                Mat2_add(_U, _Wb);

                Mat2_cpy(_input, _U);
                
                rnn_params->act.active(_input, rnn_params->act.params);
                
                // 此时 _Hts[j] 在 seq 的作用已经完结，讲它值保存为已经激活的 _input，作为下一个seq Ht 使用。
                Mat2_cpy(_Hts[j], _input);

            }

            Mat2_cpy(_Y, _V);
            Mat2_dot(_Y, _input);

            error += vec_diff(_Yi, _Y);
    
        }

        if ((error-last_error) < rnn_params->term_epsilon) break;

        // TODO: 3.2 若误差较大，则向后传播，否则退出训练
        // 需要求 deltaV deltaU deltaWb。这个才是他妈的超级难得东东。 

        for (int i=0; i<seq_length; ++i) {

            for (int j=0; i<rnn_params->hidden_layer_number; ++j) {

                

            }

        }

    }

    *out_Wbs = _Wbs;
    *out_U   = _U;
    *out_V   = _V;
    *out_Wbs_length = rnn_params->hidden_layer_numbers;

    return 0;
    
}

int rnn_sync_predict(matrix2_t* _Input, matrix2_t** Wbs, int Wbs_length, matrix2_t* U, matrix2_t* V)
{

}