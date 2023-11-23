/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-11-20 09:28:39
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-11-23 15:42:23
 * @FilePath: /boring-code/src/deep_learning/ann.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include <float.h>
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


int ann_train(matrix2_t* data, matrix2_t* label, int* hidden_layer_cell_numbers, int hidden_layers_number, ann_param_t* ann_params, matrix2_t** out_Wbs, void (*progress)(unsigned long, unsigned long))
{
    // 这是计算得到的 y^.
    matrix2_t* _Yi     = Mat2_create(1,1);
    // z 是 W dot x + b 未经过激活函数的净输出
    matrix2_t* _z;
    // input 是每层神经网络的输入。
    matrix2_t* _input   = Mat2_create(1,1);
    // delta LW 矩阵, L 对 W 的导数
    matrix2_t* delta_LW = Mat2_create(1,1);
    // delta Lb 矩阵，L 对 b 的导数
    matrix2_t* delta_Lb = Mat2_create(1,1);
    // delta Lz 矩阵，L 对 z 的导数
    matrix2_t* delta_Lz = Mat2_create(1,1);
    // delta Az 矩阵，A 激活函数对 z 的导数
    matrix_t* delta_Az  = Mat2_create(1,1);

    
    // TODO : 1 组建并且使用 0～1 初始化权重矩阵网络。
    // 权重矩阵的数量为：隐藏层 + 输入 + 输出 - 1；
    int cell_layer_number = hidden_layers_number + 1;

    matrix2_t** _Wbs = (matrix2_t**) malloc( cell_layer_number * sizeof(matrix2_t*) );
    
    // 以及申请未激活后的 z 矩阵数组，用于未激励激活后的输出的 z
    matrix2_t** _Zs = (matrix2_t**) malloc (cell_layer_number * sizeof(matrix2_t*) );

    matrix2_t** _Us = (matrix2_t**) malloc (cell_layer_number * sizeof(matrix2_t*) );

    int last_input_number = data->cols;
    int cell_layer_index;

    for(cell_layer_index=0; cell_layer_index<cell_layer_number; cell_layer_index++) {

        // 若果是最后一个的便是输出 Y 的列数
        int wbs_rows = cell_layer_index != cell_layer_number ? hidden_layer_cell_numbers[cell_layer_index] : label->cols;

        _Wbs[cell_layer_index] = Mat2_create( wbs_rows, (last_input_number+1));
        Mat_fill_random(Wbs[cell_layer_index], 0, 1);

        // 这随便就行了
        _Zs[cell_layer_index] = Mat2_create(1,1);
        _Us[cell_layer_index] = Mat2_create(1,1);

        last_input_number = hidden_layer_cell_numbers[cell_layer_index];

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
    while (fabs(error-last_error) < ann_params->epsilon && iter++ <= ann_params->max_iter) {

        last_error = error;

        // TOOD: 4 做向前传播
        for(int i=0; i<ann_params->batch; ++i) {

            int data_index = batch_data_index[i];

            // 输入层从 data 开始获取原始的输入 Xi
            Mat2_slice_row_to(_input, data, data_index);
            Mat2_slice_row_to(_Yi, label, data_index);

            Mat2_T(_input);
            
            for (int i=0; i<cell_layer_number; ++i) {
                // 这里 padding 1 是为了 把 [x1, x2, x3, ... xn] => [x1, x2, x3, ..., xn, 1] 
                Mat2_padding_bottom(_input, 1, 1.f);
                // W dot X + b
                _z = Zs[i];
                Mat2_cpy(_z, Wbs[i]);
                Mat2_dot(_z, _input);

                // 把净输出的结果保存到 _Us 中
                Mat2_cpy(_Us[i], _z);
                // 通过激活函数后，激活的结果保留在 _Us 中
                ann_params->active(_Us[i], ann_params->active_params);

                // 把激活结果复制给 _input 为下一层运算做准备
                Mat2_cpy(_input, Us[i]);
            }


            // 统计这个 batch 的误差。
            
            error += vec_diff(_Us[cell_layer_number-1], _Yi);

            // TODO: 5 向后传播算法。

            // 计算倒数第一层的误差值 delta_z
            
            Mat2_cpy(delta_Lz, _Us[cell_layer_number-1]);
            Mat2_cpy(delta_Az, _Zs[cell_layer_number-1]);

            Mat2_sub(delta_Lz, _Yi);
            ann_params->d_active(delta_Az, ann_params->d_active_params);
            Mat2_hadamard_product(delta_Lz, delta_Az);

            for (int j=cell_layer_number-1; j>=0; ++j) {
                
                Mat2_cpy(delta_LW, delta_Lz);
                if ( j== 0) {
                    // 来到最后一个的时候，_input 便是输入原始 x
                    Mat2_slice_row_to(_input, data, data_index);
                    //Mat2_T(_input);
                } else {
                    Mat2_cpy(_input, _Us[j-1]);
                    Mat2_T(_input);
                }

                Mat2_dot(delta_LW, _input);

                // TOOD: update delta
                
                

            }

        }

        //if (fabs(error - last_error) < ann_params->epsilon) break;
        //last_error = error; 
        
    }


    *out_Wbs = Wbs;
    return 0;
}
int ann_predict(matrix2_t* _Input, matrix2_t* Wbs, int hidden_layers_number, vfloat_t* predict)
{
    return 0;
}

int ann_recycle(matrix2_t* Wbs)
{
    return 0;
}
