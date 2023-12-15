/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-11-29 15:56:28
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-12-15 16:08:49
 * @FilePath: /boring-code/src/deep_learning/rnn.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>
#include <float.h>
#include <math.h>
#include <string.h>
#include "rnn.h"
//#include "active.h"

// for debug
static __printf_buff(const* title, int r, int c, vfloat_t (*buff)[c])
{
    printf(" \n------- <%s> ------ %d x %d \n", title, r, c);
    for (int i=0; i<r; ++i) {
        for (int j=0; j<c; ++j) {
            printf("  %lf, ", buff[i][j]);
        }
        printf("\n");
    }
    printf(" ------------------ \n");
}
// for debug

static double __output_compare(matrix2_t* out_put, matrix2_t* label) 
{
    int m1_size = out_put->rows * out_put->cols;
    int m2_size = label->rows * label->cols;

    if (m1_size == m2_size) {

        int out_cls = -1;
        int label_cls = -2;

        double out_max = -FLT_MAX;
        double label_max = -FLT_MAX;

        for (int i=0; i<m1_size; ++i) {
            if (out_put->pool[i] > out_max) {
                out_cls = i;
                out_max = out_put->pool[i];
            }
            if (label->pool[i] > label_max) {
                label_cls = i;
                label_max = label->pool[i];
            }
        }
        return ! (out_cls == label_cls);
    }
    return 1.f;
    
}

static double __error (matrix2_t* out_put, matrix2_t* label)
{
    int m1_size = out_put->rows * out_put->cols;
    int m2_size = label->rows * label->cols;

    if (m1_size == m2_size) {
        double err = 0.f;

        for (int i=0; i<m1_size; ++i) {
            err += label->pool[i] * log(out_put->pool[i]);
        }
        return err;
    }
    return -1.f;

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
int rnn_train(matrix2_t* seq_data, matrix2_t* seq_label, rnn_param_t* rnn_params, matrix2_t** out_Wb_xh, matrix2_t** out_W_hh, matrix2_t** out_W_hy, void (*progress)(char*, unsigned long, unsigned long, double))
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
    
    // 添加一个偏置量在 xh 中来
    matrix2_t* _Wb_xh = Mat2_create(N, K+1);

    matrix2_t* _W_hh = Mat2_create(N, N);
    matrix2_t* _W_hy = Mat2_create(L, N);

    matrix2_t* _W_hh_T = Mat2_create(1,1);
    matrix2_t* _W_hy_T = Mat2_create(1,1);

    matrix2_t* _h     = Mat2_create(1,1);
    matrix2_t* _Xi    = Mat2_create(1,1);

    // 运算使用的。
    matrix2_t* _item1 = Mat2_create(1,1);
    matrix2_t* _item2 = Mat2_create(1,1);

    matrix2_t* _delta_y = Mat2_create(L,1);
    matrix2_t* _delta_h = Mat2_create(N,1);
    
    Mat2_fill_random(_Wb_xh, 0, 1.f);
    Mat2_fill_random(_W_hh, 0, 1.f);
    Mat2_fill_random(_W_hy, 0, 1.f);
    
    // Mat2_fill(_W_xh, 0.f);
    // Mat2_fill(_W_hh, 0.f);
    // Mat2_fill(_W_hy, 0.f);
    // 

    vfloat_t (*u_buff)[N] = malloc( seq_length * N * sizeof (vfloat_t));
    vfloat_t (*h_buff)[N] = malloc( (seq_length + 1) * N * sizeof (vfloat_t));
    vfloat_t (*v_buff)[L] = malloc( seq_length * L * sizeof (vfloat_t));
    vfloat_t (*y_buff)[L] = malloc( seq_length * L * sizeof (vfloat_t));

    vfloat_t (*delta_h_buff)[N] = malloc( (seq_length + 1) * N * sizeof (vfloat_t));
    vfloat_t (*delta_y_buff)[L] = malloc( seq_length * L * sizeof (vfloat_t));

    // 初始化 h_buffer[0] 与 delta_h_buff[seq_length] 为零

    
    memset(h_buff[0], 0, N * sizeof(vfloat_t));
    memset(delta_h_buff[seq_length], 0, N * sizeof(vfloat_t));

    double error = FLT_MAX;
    int iter = 0;

    while ( iter++ < rnn_params->max_iter && fabs(error) > rnn_params->term_epsilon ) {

        if (progress) progress("rnn training...", iter, rnn_params->max_iter, fabs(error));

        error = 0.f;

        // for debug
        // printf("\n wxh: \n");
        // MAT2_INSPECT(_Wb_xh);
        // printf("\n whh: \n");
        // MAT2_INSPECT(_W_hh);
        // printf("\n why: \n");
        // MAT2_INSPECT(_W_hy);
        // for debug

        // 向前传播。
        for (int i=0, l=1; i<seq_length; ++i, ++l) {
        
            Mat2_slice_row_to(_Xi, seq_data, i);

            Mat2_T(_Xi);

            // for debug
            // MAT2_INSPECT(_Xi);
            // for debug

            Mat2_padding_bottom(_Xi, 1, 1.0f);
            
            Mat2_cpy(_item1, _Wb_xh);

            Mat2_dot(_item1, _Xi);
        
            Mat2_cpy(_item2, _W_hh);

            // load h_{t-1}
            Mat2_load_on_shape(_h, h_buff[l-1], N, 1);

            Mat2_dot(_item2, _h);

            //for debug 
            // printf("\n item1: \n");
            // MAT2_INSPECT(_item1);
            // printf("\n\n item2: \n");
            // MAT2_INSPECT(_item2);
            // printf("\n\n _h: \n");
            // MAT2_INSPECT(_h);
            //for debug

            Mat2_add(_item1, _item2);
            
            // 一系列操作后成为 u_{t}
            Mat2_export(_item1, u_buff[i]);

            // 经过激活后成为 h_{t}
            //tanh1(_item1, NULL);

            // for debug 
            // MAT2_INSPECT(_item1);
            // for debug

            rnn_params->hidden_act.active(_item1, rnn_params->hidden_act.params);

            // for debug
            // MAT2_INSPECT(_item1);
            // for debug

            Mat2_export(_item1, h_buff[l]);
            
            Mat2_cpy(_item2, _W_hy);

            // 两个点乘后成为 v_{t}
            Mat2_dot(_item2, _item1);

            Mat2_export(_item2, v_buff[i]);

            // 经过 softmax 后成为输出的 Y。
            //softmax(_item1, NULL);
            rnn_params->output_act.active(_item2, rnn_params->hidden_act.params);

            // 把输出保存起来。
            Mat2_export(_item2, y_buff[i]);

            // 此处本想用于收敛停止训练。后来发现这个是收敛了还是不行的。
            Mat2_slice_row_to(_item1, seq_label, i);
            error += __error(_item2, _item1);

            // for debug
            // Mat2_T(_item2);
            // MAT2_INSPECT(_item2);
            // MAT2_INSPECT(_item1);

            // printf("\n seq: %d -------- \n", i);
            // for debug
        }

        // for debug print buff
        // __printf_buff("u_buff", seq_length, N, u_buff);
        // __printf_buff("h_buff", seq_length+1, N, h_buff);
        // __printf_buff("v_buff", seq_length, L, v_buff);
        // __printf_buff("y_buff", seq_length, L, y_buff);
        // printf("\n error : %f \n", error);
        // for debug

        // bptt 向后传播
        
        for (int j=seq_length-1; j>=0; --j) {
            
            // 1 计算 delta_y
            Mat2_slice_row_to(_item1, seq_label, j);

            Mat2_T(_item1);

            //Mat2_import(_item2, y_buff[j]);
            Mat2_load_on_shape(_item2, y_buff[j], L, 1);

            Mat2_sub(_item1, _item2);

            // for debug
            // MAT2_INSPECT(_item1);
            // for debug

            Mat2_cpy(_delta_y, _item1);
        
            Mat2_load_on_shape(_item1, v_buff[j], L, 1);

            // g'(vt)
            rnn_params->output_d_act.d_active(_item1, rnn_params->output_d_act.params);

            // for debug
            // printf("\n\n g'(vt)\n");
            // MAT2_INSPECT(_item1);
            // for debug 

            Mat2_hadamard_product(_delta_y, _item1);
            
            // for debug
            // printf("\n\n hadamard product \n");
            // MAT2_INSPECT(_delta_y);
            // for debug

            Mat2_export(_delta_y, delta_y_buff[j]);

            /* --------------------------------------- */
            // 2 计算 delta_h
            Mat2_cpy(_item1, _W_hh);
            
            Mat2_T(_item1);

            Mat2_cpy(_item2, _W_hy);

            Mat2_T(_item2);

            // 加载 delta_h_{t+1}

            Mat2_load_on_shape(_delta_h, delta_h_buff[j+1], N, 1);

            Mat2_dot(_item1, _delta_h);

            Mat2_cpy(_item2, _W_hy);

            Mat2_T(_item2);

            Mat2_dot(_item2, _delta_y);

            Mat2_sub(_item1, _item2);

            Mat2_load_on_shape(_item2, u_buff[j], N, 1);

            //d_tanh1(_item2, NULL);
            rnn_params->hidden_d_act.d_active(_item2, rnn_params->hidden_d_act.params);

            Mat2_hadamard_product(_item1, _item2);

            Mat2_export(_item1, delta_h_buff[j]);
            
        }

        // for debug 

        // __printf_buff("delta_h_buff", seq_length + 1, N, delta_h_buff);
        // __printf_buff("delta_y_buff", seq_length, L, delta_y_buff);

        // for debug 

        // update model

        for (int k=0, l=1; k<seq_length; ++k, ++l) {

            // 更新 W_hy
            Mat2_load_on_shape(_item1, delta_y_buff[k], L, 1);

            Mat2_load_on_shape(_item2, h_buff[k], N, 1);

            Mat2_T(_item2);

            // for debug

            // MAT2_INSPECT(_item1);
            // MAT2_INSPECT(_item2);

            // for debug
            

            Mat2_dot(_item1, _item2);
            
            Mat2_scalar_multiply(_item1, rnn_params->learning_rate);
            
            // for debug 
            // MAT2_INSPECT(_item1);
            // for debug

            Mat2_sub(_W_hy, _item1);

            /* ------------------------------------------------------------- */
            // 更新 W_hh

            Mat2_load_on_shape(_item1, delta_h_buff[k], N, 1);

            Mat2_load_on_shape(_item2, h_buff[l-1], N, 1);

            Mat2_T(_item2);

            Mat2_dot(_item1, _item2);

            Mat2_scalar_multiply(_item1, rnn_params->learning_rate);

            // for debug 
            // MAT2_INSPECT(_item1);
            // for debug

            Mat2_sub(_W_hh, _item1);

            
            /* ------------------------------------------------------------------- */

            // 更新 W_hx
            Mat2_load_on_shape(_item1, delta_h_buff[k], N, 1);
            
            Mat2_slice_row_to(_item2, seq_data, k);

            Mat2_dot(_item1, _item2);

            Mat2_load_on_shape(_item2, delta_h_buff[k], N, 1);

            Mat2_merge_cols(_item1, _item2);
            
            Mat2_scalar_multiply(_item1, rnn_params->learning_rate);



            // for debug 
            // MAT2_INSPECT(_item1);
            // for debug

            Mat2_sub(_Wb_xh, _item1);

            // for debug
            // printf("\n ------- \n");
            // for debug
        }

        // for debug
        // printf(" \n <iter: %d> ---------------------------------------------------- \n", iter);
        // for debug

    }

    // TODO: clearn the shit
    
    Mat2_destroy(_h);
    Mat2_destroy(_Xi);   

    // 运算使用的。
    Mat2_destroy(_item1); 
    Mat2_destroy(_item2);

    Mat2_destroy(_delta_y);
    Mat2_destroy(_delta_h);

    free(u_buff);
    free(h_buff);
    free(v_buff);
    free(y_buff);

    free(delta_h_buff);
    free(delta_y_buff);

    *out_Wb_xh = _Wb_xh;
    *out_W_hh = _W_hh;
    *out_W_hy = _W_hy;

    return 0;
}

int rnn_predict(matrix2_t* seq_inputs, matrix2_t* Wb_xh, matrix2_t* W_hh, matrix2_t* W_hy, rnn_active_t* hidden_act, rnn_active_t* output_act, matrix2_t** seq_outpts)
{
    int seq_length = seq_inputs->rows;
    int seq_cols   = W_hy->rows;

    matrix2_t* _outputs = Mat2_create(seq_length, seq_cols);
    MAT2_POOL_PTR(_outputs, _outputs_ptr);

    matrix2_t* _h0      = Mat2_create(W_hh->cols, 1);
    matrix2_t* _Xi      = Mat2_create(1,1);
    matrix2_t* _item1   = Mat2_create(1,1);
    matrix2_t* _item2   = Mat2_create(1,1);

    Mat2_fill(_h0, 0.f);

    for (int i=0; i<seq_length; ++i) {

        Mat2_slice_row_to(_Xi, seq_inputs, i);

        //MAT2_INSPECT(_Xi);

        Mat2_T(_Xi);
        
        Mat2_padding_bottom(_Xi, 1, 1.f);

        Mat2_cpy(_item1, Wb_xh);

        Mat2_dot(_item1, _Xi);
        
        Mat2_cpy(_item2, W_hh);

        Mat2_dot(_item2, _h0);

        Mat2_add(_item1, _item2);

        hidden_act->active(_item1, hidden_act->params);

        Mat2_cpy(_h0, _item1);

        Mat2_cpy(_item1, W_hy);

        Mat2_dot(_item1, _h0);

        //MAT2_INSPECT(_h0);

        //softmax(_item1, NULL);
        output_act->active(_item1, output_act->params);

        Mat2_export(_item1, _outputs_ptr[i]);

        //printf("\n ----------------- \n");

    }

    Mat2_destroy(_h0);
    Mat2_destroy(_Xi);
    Mat2_destroy(_item1);
    Mat2_destroy(_item2);
    
    *seq_outpts = _outputs;
    return 0;
    
}