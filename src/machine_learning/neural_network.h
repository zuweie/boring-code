/*
 * @Author: your name
 * @Date: 2021-11-15 16:45:20
 * @LastEditTime: 2021-11-25 11:28:23
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /boring-code/src/machine_learning/neural_network.h
 */
#ifndef __NEURAL_NETWORK_H__
#define __NEURAL_NETWORK_H__
#include "vtype/vfloat_type.h"
#include "ultra_array/ultra_array.h"

// 激活函数类型枚举
#define Wl_sz(pmodel, Wli) \
({ \
    vfloat_t* l_ptr = UA_data_ptr(&(pmodel)->layer_size); \
    int wlsz = ((int)l_ptr[Wli] + 1) * (int)l_ptr[Wli+1]; \
    wlsz; \
})
#define Wl_count(pmodel) (UA_shape_axis(&(pmodel)->layer_size, 0)-1)

#define Cl_sz(pmodel, Cli) \
({ \
    vfloat_t* l_ptr = UA_data_ptr(&(pmodel)->layer_size); \
    int clsz = (int)l_ptr[Cli]; \
    clsz; \
})

#define Cl_count(pmodel) (UA_shape_axis(&(pmodel)->layer_size, 0))

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
    vfloat_t** _w;
    vfloat_t** _cell;
} ann_mpl_model_t;

ann_mpl_model_t* ann_mpl_training(u_array_t* layer_size, u_array_t* X, u_array_t* Y, ann_mpl_param_t* params);
int ann_mpl_predict(ann_mpl_model_t* model, u_array_t* sample, u_array_t* prediction);

int ann_mpl_model_initialize(ann_mpl_model_t* model, u_array_t* layer_size, active_func_t active);
int ann_mpl_model_finalize(ann_mpl_model_t* model);
int ann_mpl_model_save(ann_mpl_model_t* model, const char* path);
ann_mpl_model_t* ann_mpl_model_load(const char* path);
#endif