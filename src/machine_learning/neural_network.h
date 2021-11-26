/*
 * @Author: your name
 * @Date: 2021-11-15 16:45:20
 * @LastEditTime: 2021-11-26 20:22:19
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /boring-code/src/machine_learning/neural_network.h
 */
#ifndef __NEURAL_NETWORK_H__
#define __NEURAL_NETWORK_H__
#include <stdio.h>
#include "vtype/vfloat_type.h"
#include "ultra_array/ultra_array.h"

// 神经网络的层数
#define Nl_count(pmodel) (UA_shape_axis(&(pmodel)->layer_size, 0))

// 某一层的神经元数量
#define Ne_count(pmodel, l) \
({ \
    int __marco_##l = (l); \
    vfloat_t* ptr = UA_data_ptr(&(pmodel)->layer_size); \
    int e_count  = (int)ptr[__marco_##l]; \
    e_count; \
})

// // 某一层 wb 参数分组的数量。
#define Nw_vector_count(pmodel, l) \
({ \
    int __marco_##l = (l); \
    vfloat_t* ptr = UA_data_ptr(&(pmodel)->layer_size); \
    int v_count = (int)ptr[__marco_##l+1]; \
    v_count; \
})


// // 某一层 wb 参数单位数量
#define Nw_item_count(pmodel, l) \
({ \
    int __marco_##l = (l); \
    vfloat_t* ptr = UA_data_ptr(&(pmodel)->layer_size); \
    int item_count = (int)ptr[__marco_##l] + 1; \
    item_count;\
})
    
// 某一层的 wb 参数的总数量 
#define Nw_count(pmodel, l) \
({ \
    int __marco_##l = (l); \
    vfloat_t* ptr = UA_data_ptr(&(pmodel)->layer_size); \
    int total_num = ((int)ptr[__marco_##l] + 1) * ((int)ptr[__marco_##l+1]); \
    total_num;\
})


#define Wv_ptr(pmodel, l, v) ((vfloat_t*)(((char**)(pmodel)->_w[(l)])[(v)]))

// 激活函数枚举类型。
typedef enum {
    IDENTITY = 1,
    GAUSSIAN,
    SIGMOID_SYM
} active_func_t;

typedef struct {

    active_func_t active;
    int max_iter;
    float epsilon;
    // 备用参数。
    float param1;
    float param2;
} ann_mpl_param_t;

typedef struct {
    active_func_t active;
    u_array_t layer_size;
    char** _w;
    void* _cell;
} ann_mpl_model_t;

ann_mpl_model_t* ann_mpl_training(u_array_t* layer_size, u_array_t* X, u_array_t* Y, ann_mpl_param_t* params);
int ann_mpl_predict(ann_mpl_model_t* model, u_array_t* sample, u_array_t* prediction);

ann_mpl_model_t* ann_mpl_model_create(u_array_t* layer_size, active_func_t active);
int ann_mpl_model_finalize(ann_mpl_model_t* model);
int ann_mpl_model_save(ann_mpl_model_t* model, const char* path);
ann_mpl_model_t* ann_mpl_model_load(const char* path);
#endif