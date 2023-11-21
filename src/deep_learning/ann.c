/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-11-20 09:28:39
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-11-21 15:04:21
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
    matrix2_t* _Yi  = Mat2_create(ann_params->bathc, label->cols);
    // z 是 W dot x + b 未经过激活函数的输出
    matrix2_t* _z;
    // u 是经过 active (x) 后的输出
    matrix2_t* _u  = Mat2_create(1,1);
    // delta W 矩阵, L 对 W 的导数
    matrix2_t* delta_W = Mat2_create(1,1);
    // delta b 矩阵，L 对 b 的导数
    matrix2_t* delta_b = Mat2_create(1,1);
    // delta z
    matrix2_t* delta_z;

    
    // TODO : 1 申请并且使用 0～1 初始化权重矩阵。
    // 权重矩阵的数量为：隐藏层 + 输入 + 输出 - 1；
    int wbs_number = hidden_layers_number + 1;
    matrix2_t** Wbs = (matrix2_t**) malloc( wbs_number * sizeof(matrix2_t*) );
    
    // 以及申请未激活后的 z 矩阵数组，用于未激励激活后的输出的 z
    matrix2_t** Zs = (matrix2_t**) malloc (wbs_number * sizeof(matrix2_t*));

    int last_input_number = data->cols;
    int wb_index;

    for(wb_index=0; wb_index<hidden_layers_number; wb_index++) {

        Wbs[wb_index] = Mat2_create(hidden_layer_cell_numbers[wb_index], (last_input_number+1));
        Mat_fill_random(Wbs[wb_index], 0, 1);

        // 这随便就行了
        Zs[wb_index]  = Mat2_create(1,1);

        last_input_number = hidden_layer_cell_numbers[wb_index];

    }
    // 最后一个矩阵的行数要有输出层的列数决定，也就是 label 的 cols 
    Wbs[wb_index] = Mat2_create(label->cols, last_input_number+1);
    Mat_fill_random(Wbs[wb_index], 0, 1);
    Zs[wb_index] = Mat2_create(1,1);
    // end 1

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
    while (fabs(error-last_error) < ann_params->epsilon 
    && iter++ <= ann_params->max_iter) {

        last_error = error;

        // TOOD: 4 做向前传播
        for(int i=0; i<ann_params->batch; ++i) {

            int data_index = batch_data_index[i];

            // 输入层从 data 开始获取。
            Mat2_slice_row_to(_u, data, data_index);
            Mat2_T(_u);
            
            for (int i=0; i<wbs_number; ++i) {
                // 这里 padding 1 是为了 把 [x1, x2, x3, ... xn] => [x1, x2, x3, ..., xn, 1] 
                Mat2_padding_bottom(_u, 1, 1.f);
                // W dot X + b
                _z = Zs[i];
                Mat2_cpy(_z, Wbs[i]);
                Mat2_dot(_z, _u);
                Mat2_cpy(_u, _z);
                // 通过激活函数。成功成为了下一个神经层的输入。
                ann_params->active(_u, ann_params->active_params);
                // 保存每一层的净输出。
            }

            
            // 循环退出后，得到 _u 和 _z 是保留向后传播使用。

            // 统计这个 batch 的误差。
            Mat2_slice_row_to(_Yi, label, data_index);
            error += vec_diff(_u, _Yi);

            // TODO: 5 向后传播算法。
            delta_z = _u;

            Mat2_sub(delta_z, _Yi);

            _z = Zs[wbs_number-1];

            ann_params->d_active(_z, ann_params->d_active_params);

            Mat2_hadamard_product(delta_z, _z);
        
            for (int i=wbs_number-1; i<=0; --i) {
                
                
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
