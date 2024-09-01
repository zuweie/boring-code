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

static void sub_regular_term (matrix2_t* wb, double alpha, double lambda) 
{
    // 这里计算 Wb 中关于 W 的部分。W 部分需要减去 \alpha * \lambda * W
    // 这里的 Wb 最后一列是偏置 b 所以计算知道 倒数第二项。
    // W = W - \alpha * \lambda * W
    // => W = W (1 -\alpha * \lambda )
    // => W *= 1 - \alpha * \lambda
    MAT2_POOL_PTR(wb, wb_ptr);
    for (int i=0; i<wb->rows; ++i) {
        for (int j=0; j<wb->cols-1; ++j) {
            wb_ptr[i][j] *= 1 - alpha * lambda;
        }
    }
}


int ann_train(matrix2_t* data, matrix2_t* label, int* hidden_layer_cell_numbers, int hidden_layers_length, ann_param_t* ann_params, matrix2_t** out_Wbs, int* out_Wbs_length, void (*progress)(char*, unsigned long, unsigned long, double))
{
    // 这是计算得到的 y^.
    matrix2_t* _Yi     = Mat2_create(1,1);
    // delta LW 矩阵, L 对 W 的导数
    matrix2_t* delta_lW = Mat2_create(1,1);
    // delta Lz 矩阵，L 对 z 的导数
    matrix2_t* delta_lz = Mat2_create(1,1);
    // delta Az 矩阵，A 激活函数对 z 的导数

    matrix2_t* next_layer_delta_lz = Mat2_create(1,1);

    matrix2_t* next_layer_W = Mat2_create(1,1);

    matrix2_t* _W            = Mat2_create(1,1);

    // input 是每层神经网络的输入。
    matrix2_t* _input = Mat2_create(1,1);


    
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

        if (i < hidden_layers_length ) last_input_number = hidden_layer_cell_numbers[i];

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
    while (fabs(error-last_error) > ann_params->term_epsilon && iter++ <= ann_params->max_iter) {

        if (progress) 
            progress("ann training foorward ...", iter, ann_params->max_iter, fabs(error - last_error));
            
        last_error = error;
        error = 0.f;

        
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

                ann_params->hidden_act.active(_Us[j], ann_params->hidden_act.active_params);

                // 把激活结果复制给 _input 为下一层运算做准备
                Mat2_cpy(_input, _Us[j]);
            }


            // 统计这个 batch 的误差。
            
            error += vec_diff(_Us[input_layer_length-1], _Yi);

            if (progress)
                progress(" ann training backword...", iter, ann_params->max_iter, fabs(error-last_error));
            // TODO: 5 向后传播算法。

            // 1 计算最后一层的 delta_lz
            // 1，1 计算输出层的 f`(u)


            Mat2_cpy(next_layer_delta_lz, _Us[input_layer_length-1]);
            Mat2_sub(next_layer_delta_lz, _Yi);

            Mat2_2I(next_layer_W, next_layer_delta_lz->rows);
            
            // 1.2 从最后一层开始做向后传播算法。
            for (int i=cell_layer_length-1, j=input_layer_length-1; i>=0; --i, --j) {

                // 计算 f`(z) 也就是激活函数对输入的导数
                
                matrix2_t* _z = _Zs[i];
                matrix2_t* _Wb = _Wbs[i];
                matrix2_t* _x  = _Us[j-1];

                ann_params->hidden_d_act.d_active(_z, ann_params->hidden_d_act.d_active_params);
                
                Mat2_T(next_layer_W);
                Mat2_cpy(delta_lz, next_layer_W);
                Mat2_dot(delta_lz, next_layer_delta_lz);
                Mat2_hadamard_product(delta_lz, _z);

                Mat2_cpy(delta_lW, delta_lz);
                Mat2_T(_x);
                Mat2_dot(delta_lW, _x);

                // 若 lamba 不等于 0，那么要从 W 中减去正则项。
                if (ann_params->lambda != 0.f) {
                    sub_regular_term(_Wb, ann_params->learning_rate, ann_params->lambda);
                }

                // 根据公式 delta b 等于是 delta lz。
                // 这里把 delta w 和 delta delta b 合并到一起。然后就可以乘以学习系数。
                // 然后更新整个 Wb 
                Mat2_merge_cols(delta_lW, delta_lz);
                Mat2_scalar_multiply(delta_lW, ann_params->learning_rate);

                // 跟新整个 Wb
                Mat2_sub(_Wb, delta_lW);

                // 从新截取 W 到 和 delta lz 进行新一步的
                Mat2_slice_cols_to(next_layer_W, _Wb, 0, _Wb->cols-1);
                
                Mat2_cpy(next_layer_delta_lz, delta_lz);
                
                // node

            }
        }
    }

    // clean up the shit
    // 清理所有申请的内存。
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


int ann_predict(matrix2_t* _Input, matrix2_t** Wbs, int Wbs_length, ann_active_t* hidden_act, matrix2_t* predict)
{
    // 确保 _Input 是竖向量。
    if (_Input->rows < _Input->cols) 
        Mat2_T(_Input);

    for (int i=0; i<Wbs_length; ++i) {
        
        Mat2_padding_bottom(_Input, 1, 1.f);

        Mat2_cpy(predict, Wbs[i]);

        Mat2_dot(predict, _Input);

        hidden_act->active(predict, hidden_act->active_params);
        
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
