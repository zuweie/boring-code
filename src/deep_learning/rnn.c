/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-11-29 15:56:28
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-12-12 23:54:16
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
 * @brief 简单的同步 RNN 
 * 整体模型如下：
 * 
 *  x_t \in R^{K*1}, h_t \in R^{N*1}, y_t \in R{L*1} \
 * W_{hy} \in R^{L*N}, W_{xh} \in R^{N*K}, W_{hh} \in R^{N*N}
 *  
 * h_t = f( W_{xh} \codt X_{t} + W{hh} \cdot h_{t-1} )
 * y_t = g( W_{hy} \cdot h_t ) 
 * 
 * f(x) 为 tanh 激活函数
 * g(x) 为 softmax 激活函数
 * 
 * 为了简化不要，所有的线性运算皆不要偏置量。
 * 
 * @param data 训练数据
 * @param label 标签
 * @param rnn_params 训练参数
 * @param epochs 循环次数
 * @return int 
 */

/**
 * @brief 
 * 
 * @param seq_data 序列式的训练数据
 * @param seq_label 序列式的训练标签
 * @param rnn_params 训练参数
 * @param epochs 训练循环参数
 * @param out_W 关于输入的权重参数
 * @param out_U 关于隐藏状态的权重参数
 * @param out_V 关于输出的
 * @return int 
 */
int rnn_sync_train(matrix2_t* seq_data, matrix2_t* seq_label, rnn_param_t* rnn_params, matrix2_t* out_W, matrix2_t* out_U, matrix2_t* out_V)
{

    // TODO：1、做向前传播计算
    // TODO：2、做双通道向后传播计算

    int seq_length = seq_data->rows;
    
    // X length
    int K = seq_data->cols;
    // hidden status length
    int N = rnn_params->hidden_layer_cells_numbers;
    // ouput length
    int L = seq_label->cols;
    
    matrix2_t* _W_xh = Mat2_create(N, K);
    matrix2_t* _W_hh = Mat2_create(N, N);
    matrix2_t* _W_hy = Mat2_create(L, N);

    matrix2_t* _W_xh_T = Mat2_create(1,1);
    matrix2_t* _W_hh_T = Mat2_create(1,1);
    matrix2_t* _W_hy_T = Mat2_create(1,1);

    matrix2_t* _pre_h = Mat2_create(N,1);
    matrix2_t* _h     = Mat2_create(1,1);
    
    matrix2_t* _Xi    = Mat2_create(1,1);
    matrix2_t* _Yi    = Mat2_create(1,1);
    matrix2_t* _input = Mat2_create(1,1);

    matrix2_t* _delta_y = Matrix2_create(L,1);
    matrix2_t* _delta_h = Matrix2_create(N,1);
    
    Mat2_fill_random(_W_xh, 0, 1.f);
    Mat2_fill_random(_W_hh, 0, 1.f);
    Mat2_fill_random(_W_hy, 0, 1.f);
    // 
    Mat2_fill(_pre_H, 0.f);
    Mat2_fill(_delta_h, 0.f);

    vfloat_t (*u_buff)[N] = (vfloat_t*) malloc( seq_length * N * sizeof (vfloat_t));
    vfloat_t (*h_buff)[N] = (vfloat_t*) malloc( seq_length * N * sizeof (vfloat_t));
    vfloat_t (*v_buff)[L] = (vfloat_t*) malloc( seq_length * L * sizeof (vfloat_t));
    vfloat_t (*y_buff)[L] = (vfloat_t*) malloc( seq_length * L * sizeof (vfloat_t));

    double error = FLT_MAX;
    double last_error = 0.f;
    int iter = 0;

    while ( (error - last_error) > rnn_params->term_epsilon && iter++ < rnn_params->max_iter) {

        // 向前传播。
        for (int i=0; i<seq_length; ++i) {
        
            Mat2_slice_row_to(_Xi, seq_data, i);
            Mat2_T(_Xi);

            Mat2_cpy(_h, _W_xh);
            Mat2_dot(_h, _Xi);
        
            Mat2_cpy(_input, _W_hh);
            Mat2_dot(_input, _pre_h);

            Mat2_add(_h, _input);
            
            Mat2_export(_h, u_buff[i]);

            //rnn_params->act.active(_h, rnn_params->act.params);
            tanh(_h);

            Mat2_export(_h, h_buff[i]);
            
            Mat2_cpy(_pre_h, _h);

            Mat2_cpy(_input, _W_hy);

            Mat2_dot(_input, _h);

            Mat2_export(_input, v_buff[i]);

            //rnn_params->act.active(_var);
            softmax(_input);

            Mat2_export(_input, y_buff[i]);
            
        }

        // bptt 向后传播
        
        for (int j=seq_data-1; j>=0; --j) {
            
            Mat2_slice_row_to(_Yi, seq_label, j)
            
            

        }

    }

    // TODO: clearn the shit
    
    *out_W = _W;
    *out_U = _U;
    *out_V = _V;

    return 0;
}

int rnn_sync_predict(matrix2_t* _Input, matrix2_t** Wbs, int Wbs_length, matrix2_t* U, matrix2_t* V)
{

}