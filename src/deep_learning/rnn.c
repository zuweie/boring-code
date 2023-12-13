/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-11-29 15:56:28
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-12-13 14:47:24
 * @FilePath: /boring-code/src/deep_learning/rnn.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <float.h>
#include <string.h>
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
int rnn_sync_train(matrix2_t* seq_data, matrix2_t* seq_label, rnn_param_t* rnn_params, matrix2_t* out_W_xh, matrix2_t* out_W_hh, matrix2_t* out_W_hy, void (*progress)(char*, unsigned long, unsigned long))
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

    matrix2_t* _W_hh_T = Mat2_create(1,1);
    matrix2_t* _W_hy_T = Mat2_create(1,1);

    //matrix2_t* _pre_h = Mat2_create(N,1);
    matrix2_t* _h     = Mat2_create(1,1);
    
    matrix2_t* _Xi    = Mat2_create(1,1);

    // 运算使用的。
    matrix2_t* _item1 = Mat2_create(1,1);
    matrix2_t* _item2 = Mat2_create(1,1);

    matrix2_t* _delta_y = Matrix2_create(L,1);
    
    Mat2_fill_random(_W_xh, 0, 1.f);
    Mat2_fill_random(_W_hh, 0, 1.f);
    Mat2_fill_random(_W_hy, 0, 1.f);
    // 

    vfloat_t (*u_buff)[N] = malloc( seq_length * N * sizeof (vfloat_t));
    vfloat_t (*h_buff)[N] = malloc( (seq_length + 1) * N * sizeof (vfloat_t));
    vfloat_t (*v_buff)[L] = malloc( seq_length * L * sizeof (vfloat_t));
    vfloat_t (*y_buff)[L] = malloc( seq_length * L * sizeof (vfloat_t));

    vfloat_t (*delta_h_buff)[N] = malloc( (seq_length + 1) * N * sizeof (vfloat_t));
    vfloat_t (*delta_y_buff)[L] = malloc( seq_length * L * sizeof (vfloat_t));

    // 初始化 h_buffer[0] 与 delta_h_buff[seq_length] 为零

    
    memset(h_buff[0], 0, N * sizeof(vfloat_t));
    memset(delta_h_buffer[seq_length], 0, N * sizeof(vfloat_t));

    double error = FLT_MAX;
    double last_error = 0.f;
    int iter = 0;

    while ( iter++ < rnn_params->max_iter) {

        if (progress) progress("rnn training...", iter, rnn_params->max_iter);
        
        last_error = error;
        error = 0.f;

        // 向前传播。
        for (int i=0, l=1; i<seq_length; ++i, ++l) {
        
            Mat2_slice_row_to(_Xi, seq_data, i);

            Mat2_T(_Xi);

            Mat2_cpy(_item1, _W_xh);

            Mat2_dot(_item1, _Xi);
        
            Mat2_cpy(_item2, _W_hh);

            // load h_{t-1}
            Mat2_import(_h, h_buff[l-1]);

            Mat2_dot(_item2, _h);

            Mat2_add(_item1, _item2);
            
            // 一系列操作后成为 u_{t}
            Mat2_export(_item1, u_buff[i]);

            // 经过激活后成为 h_{t}
            tanh(_item1);

            Mat2_export(_item1, h_buff[l]);
            
            Mat2_cpy(_item2, _W_hy);

            // 两个点乘后成为 v_{t}
            Mat2_dot(_item1, _item2);

            Mat2_export(_item1, v_buff[i]);

            // 经过 softmax 后成为输出的 Y。
            softmax(_item1);

            // 把输出保存起来。
            Mat2_export(_item1, y_buff[i]);

            Mat2_slice_row_to(_item2, seq_label, i);

            error += vec_diff(_item1, _item2);
            
        }
        
        // 如果达到精度要求则不在训练。
        if ( (error - last_error) < rnn_params->term_epsilon ) break;

        // bptt 向后传播
        
        for (int j=seq_length-1; j>=0; --j) {
            
            // 1 计算 delta_y
            Mat2_slice_row_to(_item1, seq_label, j)

            Mat2_import(_item2, y_buffer[j]);

            Mat2_sub(_item1, _item2);

            Mat2_cpy(_delta_y, _item1);

            Mat2_export(_delta_y, _delta_y_buff[j]);

            // 2 计算 delta_h
            Mat2_cpy(_item1, _W_hh);
            
            Mat2_T(_item1);

            Mat2_cpy(_item2, _W_hy);

            Mat2_T(_item2);

            // 加载 delta_h_{t+1}

            Mat2_import(_delta_h, _delta_h_buff[j+1]);

            Mat2_dot(_item1, _delta_h);

            

            Mat2_cpy(_itme2, _W_hy);

            Mat2_T(_item2);

            Mat2_dot(_item2, _delta_y);

            Mat2_sub(_item1, _item2);

            Mat2_load_on_shape(_item2, u_buffer[j], N, 1);

            d_tanh(item2);

            Mat2_hadamard_product(_item1, _item2);

            Mat2_export(_item1, _delta_h_buff[j]);
            
        }

        // update model

        for (int k=0; k<seq_length; ++k) {

            // 更新 W_hy
            Mat2_load_on_shape(_item1, delta_y_buff[k], L, 1);

            Mat2_load_on_shape(_item2, h_buff[k], N, 1);

            Mat2_T(_item2);

            Mat2_dot(_item1, _item2);
            
            Mat2_scalar_multiply(_item1, rnn_params->learn_rate);
            
            Mat2_add(_W_hy, _item1);

            // 更新 W_hh

            Mat2_load_on_shape(_item1, delta_h_buff[k], N, 1);

            Mat2_load_on_shape(_item2, h_buff[k-1], N, 1);

            Mat2_T(_item2);

            Mat2_dot(_item1, _item2);

            Mat2_scalar_multiply(_item1, rnn_params->learn_rate);

            Mat2_add(_W_hh, _item1);

            // 更新 W_hx
            Mat2_load_on_shape(_item1, delta_h_buff[k], N, 1);
            
            Mat2_slice_row_to(_item2, seq_data, k);

            Mat2_T(_item2);

            Mat2_dot(_item1, _item2);

            Mat2_scalar_multiply(_item1, rnn_params->learn_rate);

            Mat2_add(_W_hx, _item1);
        }

    }

    // TODO: clearn the shit
    
    Mat2_destroy(_h);
    Mat2_destroy(_Xi);   

    // 运算使用的。
    Mat2_destroy(_item); 
    Mat2_destroy(_item2);

    Mat2_destroy(_delta_y);

    free(u_buff);
    free(h_buff);
    free(v_buff);
    free(y_buff);

    free(delta_h_buff);
    free(delta_y_buff);

    *out_W_xh = _W_xh;
    *out_W_hh = _W_hh;
    *out_W_hy = _W_hy;

    return 0;
}

int rnn_sync_predict(matrix2_t* seq_inputs, matrix2_t* w_xh, matrix2_t* w_hh, matrix2_t* w_hy, matrix2_t** seq_outpts)
{
    int seq_length = seq_inputs->rows;
    int seq_cols   = w_hy->rows;

    matrix2_t* _outputs = Mat2_create(seq_length, seq_cols);
    MAT2_POOL_PTR(_outputs, _outputs_ptr);

    matrix2_t* _h0       = Mat2_create(w_hh->cols, 1);
    matrix2_t* _Xi      = Mat2_create(1,1);
    matrix2_t* _item1   = Mat2_create(1,1);
    matrix2_t* _item2   = Mat2_create(1,1);

    Mat2_fill(_h0, 0.f);

    for (int i=0; i<seq_length; ++i) {

        Mat2_slice_row_to(_Xi, seq_inputs);

        Mat2_T(_Xi);
        
        Mat2_cpy(_item1, w_xh);

        Mat2_dot(_item1, _Xi);
        
        Mat2_cpy(_item2, w_hh);

        Mat2_dot(_item2, _h0);

        Mat2_add(_item1, _item2);

        tanh(_item1);

        Mat2_cpy(_h0, _item1);

        Mat2_cpy(_item1, w_hy);

        Mat2_dot(_item1, _h0);

        softmax(_item1);

        Mat2_export(_item1, _outputs_ptr[i]);

    }

    Mat2_destroy(_h0);
    Mat2_destroy(_Xi);
    Mat2_destroy(_item1);
    Mat2_destroy(_item2);
    
    *seq_outpts = _outputs;
    return 0;
    
}