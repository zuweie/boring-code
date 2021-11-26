/*
 * @Author: your name
 * @Date: 2021-11-15 16:45:20
 * @LastEditTime: 2021-11-26 16:42:47
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
    int __marco_l = (l); \
    vfloat_t* ptr = UA_data_ptr(&(pmodel)->layer_size); \
    int e_count  = (int)ptr[__marco_l]; \
    e_count; \
})

// // 某一层 wb 参数分组的数量。
#define Nw_vcount(pmodel, l) \
({ \
    int __marco_l = (l); \
    vfloat_t* ptr = UA_data_ptr(&(pmodel)->layer_size); \
    int v_count = (int)ptr[__marco_l+1]; \
    v_count; \
})
// // 某一层 wb 参数单位数量
#define Nw_icount(pmodel, l) \
({ \
    int __marco_l = (l); \
    vfloat_t* ptr = UA_data_ptr(&(pmodel)->layer_size); \
    int item_count = (int)ptr[__marco_l] + 1; \
    item_count;\
})
// 某一层的 wb 参数的总数量
#define Nw_count(pmodel, l) \
({ \
    int __marco_l = (l); \
    vfloat_t* ptr = UA_data_ptr(&(pmodel)->layer_size); \
    int total_num = ((int)ptr[__marco_l] + 1) * ((int)ptr[__marco_l+1]); \
    total_num;\
})
// // 计算 wb 数组的首地址
// #define Nw_cal_wbase_ptr(pmodel) \
// ({  \
//     char* base = (char*)(pmodel)->_w; \
//     int l_count = Nl_count(pmodel); \
//     char* dest = base + (l_count-1) * sizeof(char*); \
//     int v_count = 0; \
//     for (l=0; l<l_count-1; ++l) { \
//         v_count += Nw_vcount(pmodel, l); \
//     } \
//     dest += v_count * sizeof(char*); \
//     dest; \
// })

// // 计算某一层的某一组 wb 参数的首地址。
// #define Nw_cal_vptr(pmodel, l, v) \
// ({ \
//     int __marco_l = (l); \
//     int __marco_v = (v); \
//     int w_num = 0; \
//     /* 计算并非当前层数的 w 的数量 */ \
//     for (int il=0; il<__marco_l; ++il) { \
//         w_num += Nw_count(pmodel, il); \
//     } \
//     w_num += Nw_icount(pmodel, __marco_l) * (__marco_v);\
//     char* base = (char*)Nw_cal_wbase_ptr(pmodel); \
//     base = (vfloat_t*)(base + w_num*sizeof(vfloat_t));\
//     base;\
// })

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