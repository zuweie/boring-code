/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-11-20 09:28:39
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-11-20 15:57:50
 * @FilePath: /boring-code/src/deep_learning/ann.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdlib.h>
#include <float.h>
#include "ann.h"


int ann_train(matrix2_t* data, matrix2_t* label, int* hidden_layers, int hidden_layers_number, ann_param_t* ann_params, matrix2_t** out_Wbs)
{
    // 这是计算得到的 y^.
    matrix2_t* _Y  = Mat2_create(label->rows, label->cols);
    
    matrix2_t* _Xi = Mat2_create(1,1);
    // z 是 W dot x + b 未经过激活函数的输出
    matrix2_t* _z  = Mat2_create(1,1);
    // u 是经过 active (x) 后的输出
    matrix2_t* _u  = Mat2_create(1,1);
    //matrix2_t* wb  = Mat2_create(1,1);

    // 权重和偏置矩阵的数量是，总层数 - 1；
    // 总层数是，输入和输出层加隐藏层

    // TODO : 1 申请并且使用 0～1 初始化权重矩阵。
    int wbs_number = hidden_layer_numbser + 1;
    matrix2_t** Wbs = (matrix2_t**) malloc( wbs_number * sizeof(matrix2_t*) );
    int last_input_number = data->cols;
    int wb_index;

    for(wb_index=0; i<hidden_layers_number; wb_index++) {

        Wbs[wb_index] = Mat2_create(hidden_layers[wb_index], (last_input_number+1));
        
        Mat_fill_random(Wbs[wb_index], 0, 1);

        last_input_number = hidden_layers[wb_index];

    }
    // 
    Wbs[wb_index] = Mat2_create(label->cols, last_input_number+1);
    Mat_fill_random(wbs[wb_index], 0, 1);

    

    double error = FLT_MAX;
    double last_error = 0.f;
    while ( (error - last_error)< epsilon ) {

        // TOOD: 2 做向前传播
        for(int i=0; i<data->rows; ++i) {

            Mat2_slice_row_to(_u data, i);

            // [x1, x2, ..., xn, 1]
            Mat2_padding_right(_u, 1, 1.f);

            Mat2_T(_z);
            
            for (int i=0; i<wbs_number; ++i) {
                
                // W dot X + b
                Mat2_cpy(_z, wbs[i]);
                Mat2_dot(_z, _u);

                Mat2_cpy(_u, _z);

                // 通过激活函数。
                ann_params->active(_u, ann_params->active_params);
                
            }

            
            

            

        }

        // TODO: 3 做向后传播


        
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
