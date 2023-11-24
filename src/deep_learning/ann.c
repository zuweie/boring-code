/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-11-20 09:28:39
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-11-24 16:52:58
 * @FilePath: /boring-code/src/deep_learning/ann.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <float.h>
#include <string.h>
#include "ann.h"

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


int ann_train(matrix2_t* data, matrix2_t* label, int* hidden_layer_cell_numbers, int hidden_layers_length, ann_param_t* ann_params, matrix2_t** out_Wbs, int* out_Wbs_length, void (*progress)(char*, unsigned long, unsigned long))
{
    // 这是计算得到的 y^.
    matrix2_t* _Yi     = Mat2_create(1,1);
    // input 是每层神经网络的输入。
    matrix2_t* _input   = Mat2_create(1,1);
    // delta LW 矩阵, L 对 W 的导数
    matrix2_t* delta_lW = Mat2_create(1,1);
    // delta Lz 矩阵，L 对 z 的导数
    matrix2_t* delta_lz = Mat2_create(1,1);
    // delta Az 矩阵，A 激活函数对 z 的导数

    matrix2_t* next_layer_delta_lz = Mat2_create(1,1);

    matrix2_t* next_layer_W = Mat2_create(1,1);

    matrix2_t* _W            = Mat2_create(1,1);

    
    // TODO : 1 组建并且使用 0～1 初始化权重矩阵网络。
    // 权重矩阵的数量为：隐藏层 + 输入 + 输出 - 1；
    int cell_layer_length = hidden_layers_length + 1;
    int input_layer_length = cell_layer_length + 1;

    matrix2_t** _Wbs = (matrix2_t**) malloc( cell_layer_length * sizeof(matrix2_t*) );
    
    // 用于记录所有权重矩阵的输入,包括原始输入，前一层激活后的输出等于这一层的输入
    matrix2_t** _Us = (matrix2_t**) malloc ( input_layer_length * sizeof(matrix2_t*) );

    // 用于记录所有的输入点乘后的未激活的输出
    matrix2_t** _Zs = (matrix2_t**) malloc ( cell_layer_length * sizeof (matrix2_t*) );

    int last_input_number = data->cols;

    _Us[0] = Mat2_create(1,1);

    for(int i=0,j=1; i<cell_layer_length; ++i, ++j) {

        // 若果是最后一个的便是输出 Y 的列数
        _Wbs[i] = Mat2_create( i != cell_layer_length-1 ? hidden_layer_cell_numbers[i] : label->cols, (last_input_number+1));
        Mat2_fill_random(_Wbs[i], 0, 1);

        _Zs[i] = Mat2_create(1,1);
        // 这随便就行了
        _Us[j] = Mat2_create(1,1);

        if (i != cell_layer_length - 1) last_input_number = hidden_layer_cell_numbers[i];

    }

    // TODO: 2 batch 的大小随机选取一些数据
    
    srand(time(0));
    int batch_data_index[ann_params->batch];
    for (int i=0; i<ann_params->batch; ++i) {
        batch_data_index[i] = rand() % (data->rows + 1);
    }
    // end 2 

    // TODO: 3 start training

    double error = FLT_MAX;
    double last_error = 0.f;
    int iter = 0;
    while (fabs(error-last_error) > ann_params->trim_epsilon && iter++ <= ann_params->max_iter) {

        if (progress) 
            progress("ann training...", iter, ann_params->max_iter);
            
        last_error = error;

        
        for(int batch_index=0; batch_index<ann_params->batch; ++batch_index) {

            // TOOD: 4 向前传播
            int data_index = batch_data_index[batch_index];

            // 输入层从 data 开始获取原始的输入 Xi
            Mat2_slice_row_to(_Yi, label, data_index);
            Mat2_T(_Yi);
            Mat2_slice_row_to(_input, data, data_index);
            Mat2_T(_input);

            // _Us 第一个便是原始的 X 输入。
            Mat2_cpy(_Us[0], _input);
            
            // _Us 从第二个输入开始计算。
            for (int i=0,j=1; i<cell_layer_length; ++i, ++j) {
                // 这里 padding 1 是为了 把 [x1, x2, x3, ... xn] => [x1, x2, x3, ..., xn, 1] 
                Mat2_padding_bottom(_input, 1, 1.f);
                // W dot X + b
                //_z = _Us[j];
                Mat2_cpy(_Us[j], _Wbs[i]);
                Mat2_dot(_Us[j], _input);

                Mat2_cpy(_Zs[i], _Us[j]);
                // 通过激活函数后，激活的结果保留在 _Us 中

                ann_params->act.active(_Us[j], ann_params->act.active_params);

                // 把激活结果复制给 _input 为下一层运算做准备
                Mat2_cpy(_input, _Us[j]);
            }


            // 统计这个 batch 的误差。
            
            error += vec_diff(_Us[input_layer_length-1], _Yi);

            // TODO: 5 向后传播算法。

            // 1 计算最后一层的 delta_lz
            // 1，1 计算输出层的 f`(u)


            Mat2_cpy(next_layer_delta_lz, _Us[input_layer_length-1]);
            Mat2_sub(next_layer_delta_lz, _Yi);

            Mat2_2I(next_layer_W, next_layer_delta_lz->rows);
            
            // 1.2 核心函数
            for (int i=cell_layer_length-1, j=input_layer_length-1; i>=0; --i, --j) {

                // 计算 f`(z) 也就是激活函数对输入的导数
                
                matrix2_t* _z = _Zs[i];
                ann_params->d_act.d_active(_z, ann_params->d_act.d_active_params);
                
                Mat2_T(next_layer_W);
                Mat2_cpy(delta_lz, next_layer_W);
                Mat2_dot(delta_lz, next_layer_delta_lz);
                Mat2_hadamard_product(delta_lz, _z);

                Mat2_cpy(delta_lW, delta_lz);
                Mat2_T(_Us[j-1]);
                Mat2_dot(delta_lW, _Us[j-1]);

                Mat2_slice_cols_to(_W, _Wbs[i], 0, _Wbs[i]->cols-1);
                Mat2_scalar_multiply(_W, ann_params->lambda);

                Mat2_add(delta_lW, _W);
                Mat2_scalar_multiply(delta_lW, ann_params->learning_rate);

                //Mat2_cpy(delta_lb, delta_lz);
                
                Mat2_merge_cols(delta_lW, delta_lz);

                Mat2_add(_Wbs[i], delta_lW);

                // 
                Mat2_slice_cols_to(next_layer_W, _Wbs[i], 0, _Wbs[i]->cols-1);
                
                Mat2_cpy(next_layer_delta_lz, delta_lz);
                
                // node

            }
        }
    }

    // clean up the shit
    Mat2_destroy(_Yi); 
    Mat2_destroy(_input);   
    Mat2_destroy(delta_lW);
    Mat2_destroy(delta_lz); 
    Mat2_destroy(next_layer_delta_lz); 
    Mat2_destroy(next_layer_W);
    Mat2_destroy(_W);        

    //
    Mat2_destroy(_Us[0]);
    for (int i=0,j=1; i<cell_layer_length; ++i, ++j) {
        Mat2_destroy(_Us[j]);
        Mat2_destroy(_Zs[i]);
    }

    free(_Zs);
    free(_Us);

    *out_Wbs = _Wbs;
    *out_Wbs_length = cell_layer_length;
    return 0;
}


int ann_predict(matrix2_t* _Input, matrix2_t** Wbs, int Wbs_length, ann_active_t* act, matrix2_t* predict)
{
    // 确保 _Input 是竖向量。
    if (_Input->rows < _Input->cols) 
        Mat2_T(_Input);

    for (int i=0; i<Wbs_length; ++i) {
        
        Mat2_padding_bottom(_Input, 1, 1.f);

        Mat2_cpy(predict, Wbs[i]);

        Mat2_dot(predict, _Input);

        act->active(predict, act->active_params);
        
        Mat2_cpy(_Input, predict);
    }
    
    return 0;
}

int ann_recycle(matrix2_t** Wbs, int Wbs_length)
{
    for (int i=0; i<Wbs_length; ++i) {
        Mat2_destroy(Wbs[i]);
    }
    free(Wbs);
    return 0;
}
