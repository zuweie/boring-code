/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-11-20 09:28:45
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-11-21 16:38:57
 * @FilePath: /boring-code/src/deep_learning/ann.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __ANN_H__
#define __ANN_H__

#include "vtype/vfloat_type.h"
#include "matrix2/matrix2.h"


typedef struct {
    // 结束的精度
    double trim_epsilon;
    // 最大迭代次数
    int max_iter;
    // 每次训练的量
    int batch;
    // 梯度下降的速率。
    double learning_rate;
    // 激活函数
    int (*active)(matrix2_t*, double* );
    // 激活函数的导数
    int (*d_active)(matrix2_t*, double* );
    // 激活函数的参数
    double* active_params;
    // 激活函数导数的参数
    double* d_active_params;

} ann_param_t;

int ann_train(matrix2_t* data, matrix2_t* label, int* hidden_layers, int hidden_layers_number, ann_param_t* ann_params, matrix2_t** out_Wbs, void(*progress)(unsigned long, unsigned long));
int ann_predict(matrix2_t* _Input, matrix2_t* Wbs, int hidden_layers_number, vfloat_t* predict);
int ann_recycle(matrix2_t* Wbs);


#endif