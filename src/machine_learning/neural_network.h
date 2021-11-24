/*
 * @Author: your name
 * @Date: 2021-11-15 16:45:20
 * @LastEditTime: 2021-11-24 16:44:35
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /boring-code/src/machine_learning/neural_network.h
 */
#ifndef __NEURAL_NETWORK_H__
#define __NEURAL_NETWORK_H__
#include "vtype/vfloat_type.h"
#include "ultra_array/ultra_array.h"

// 激活函数类型枚举

enum {
    err_ok = 0,
    err_input_layer = -1,
    err_output_layer = -2,
    err_layer_size = -3,
    err_input_vector = -4,
};

typedef enum {
    IDENTITY = 1,
    GAUSSIAN,
    SIGMOID_SYM
} active_func_t;

typedef struct {

    active_func_t active;
    int max_iter;
    float epsilon;
} ann_mpl_param_t;

typedef struct {
    active_func_t active;
    u_array_t layer_size;
    vfloat** weights;
    vfloat** bias;
} ann_mpl_model_t;

int ann_mpl_train(ann_mpl_model_t* model, u_array_t* layer_size, u_array_t* X, u_array_t* Y, ann_mpl_param_t* params);

int ann_mpl_predict(ann_mpl_model_t* model, u_array_t* sample, u_array_t* prediction);

int ann_mpl_model_finalize(ann_mpl_model_t* model);
#endif