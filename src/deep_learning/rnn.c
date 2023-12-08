/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-11-29 15:56:28
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-12-08 16:23:42
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
 *  W_{hy} \in R^{L*N}, W_{xh} \in R^{N*K}, W_{hh} \in R^{N*N}
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
int rnn_sync_train(matrix2_t* seq_data, matrix2_t* seq_label, rnn_param_t* rnn_params, int epochs, )
{


    
}

int rnn_sync_predict(matrix2_t* _Input, matrix2_t** Wbs, int Wbs_length, matrix2_t* U, matrix2_t* V)
{

}