/*
 * @Author: your name
 * @Date: 2021-11-15 16:45:08
 * @LastEditTime: 2021-11-25 11:44:28
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /boring-code/src/machine_learning/neural_network.c
 */
#include <stdlib.h>
#include "neural_network.h"

// 根据层数生情


ann_mpl_model_t* ann_mpl_training(u_array_t* layer_size, u_array_t* X, u_array_t* Y, ann_mpl_param_t* params)
{

    
}

int ann_mpl_predict(ann_mpl_model_t* model, u_array_t* sample, u_array_t* prediction)
{

}

ann_mpl_model_t* ann_mpl_model_create(u_array_t* _layer_size, active_func_t _active)
{

    int len_Lr = UA_shape_axis(_layer_size, 0);
    int len_Lc = UA_shape_axis(_layer_size, 1);
    if (len_Lr <= 2 && len_Lc != 1) return -1;

    ann_mpl_model_t* model = (ann_mpl_model_t*) malloc (sizeof(ann_mpl_model_t));
    
    model->active = _active;
    model->layer_size = UA_copy(_layer_size);

    vfloat_t* l_ptr = UA_data_ptr(&model->layer_size);
    // 开始统计总共需要多少的 w 于 bais。
    // 开始统计有多少个 cell。
    int i, j, k;
    int wb_num = 0;
    int cell_num = 0;
    for (i=0; i<len_Lr; ++i) {
        if (i > 0 ) 
            wb_num += ((int)l_ptr[i-1] + 1) * (int)l_ptr[i];
        cell_num += (int)l_ptr[i];
    }
    
    int sz_wb = sizeof(vfloat_t*) * (len_Lr-1) + sizeof(vfloat_t) * wb_num;
    int sz_cell = sizeof(vfloat_t*) * (len_Lr) + sizeof (vfloat_t) * cell_num;

    char* w_base    =  malloc(sz_wb);
    char* 
    char* cell_base = malloc(sz_cell);

    //开始内存分配了
    size_t offset = 0;
    for (j=0; j<Wl_count(model); ++j) {
        // 这里错了
        model->_w[j] = (vfloat_t*)(w_base + offset);
        offset += Wl_sz(model, j) * sizeof(vfloat_t);
    }
    offset = 0;
    for (k=0; k<Cl_count(model); ++k) {
        // 这里错了
        model->_cell[k] = (vfloat_t*)(cell_base + offset);
        offset += Cl_sz(model, k) * sizeof(vfloat_t);
    }

    // 
    return 0;
}

int ann_mpl_model_finalize(ann_mpl_model_t* model)
{
    free(model->_w);
    free(model->_cell);
    UArray_(&model->layer_size);
    free(model);
    return 0;
}