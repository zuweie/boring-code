/*
 * @Author: your name
 * @Date: 2021-11-15 16:45:08
 * @LastEditTime: 2021-11-26 17:31:24
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /boring-code/src/machine_learning/neural_network.c
 */
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include "neural_network.h"

static double
__ann_mpl_rand1(int from, int to) 
{
    srand((unsigned int) time(NULL));
    int r = rand() % ((to+1) * 10 + 1) ;
    double v = (double) r / (double) 10 - from;
    return v;
}

// 某一层 wb 参数分组的数量。
// static int __nw_vcount(ann_mpl_model_t* model, int l)
// { 
//     int __marco_l = (l); 
//     vfloat_t* ptr = UA_data_ptr(&(model)->layer_size); 
//     int v_count = (int)ptr[__marco_l+1]; 
//     return v_count; 
// }
// 某一层 wb 参数单位数量
// static int __nw_icount(ann_mpl_model_t* model, int l) 
// { 
//     int __marco_l = (l); 
//     vfloat_t* ptr = UA_data_ptr(&(model)->layer_size); 
//     int item_count = (int)ptr[__marco_l] + 1; 
//     return item_count;
// }

// 某一层的 wb 参数的总数量
// static int __nw_count(ann_mpl_model_t* model, int l) 
// {
//     int __marco_l = (l); 
//     int total_num = __nw_icount(model, __marco_l) * __nw_vcount(model, __marco_l); 
//     return total_num;
// }

static char* __nw_cal_wbase_ptr(ann_mpl_model_t* model) 
{  
    char* base = (char*)(model)->_w; 
    int l_count = Nl_count(model); 
    char* dest = base + (l_count-1) * sizeof(char*); 
    int v_count = 0; 
    for (int l=0; l<l_count-1; ++l) { 
        v_count += Nw_vcount(model, l); 
    } 
    dest += v_count * sizeof(char*); 
    return dest; 
}

static char* __nw_cal_vptr(ann_mpl_model_t* model, int l, int v) 
{ 
    int __marco_l = (l); 
    int __marco_v = (v); 
    int w_num = 0; 
    /* 计算并非当前层数的 w 的数量 */ 
    for (int il=0; il<__marco_l; ++il) { 
        w_num += Nw_count(model, il); 
    } 
    int cl_icount = Nw_icount(model, __marco_l);
    w_num += cl_icount * (__marco_v);
    char* base = (char*)__nw_cal_wbase_ptr(model); 
    base = (char*)(base + w_num*sizeof(vfloat_t));
    return base;
}
// static int 
// __ann_mpl_model_initialize(ann_mpl_model_t* model)
// {
//     int r;
//     double v;
//     for (int i=0; i<Wl_count(model); ++i) {
//         // 初始化 w
//         double sum_v = 0.f;
//         for (int j=0; j<Wl_sz(model, j)-1; ++j) {
//             model->_w[i][j] = __ann_mpl_rand1(-1, 1);
//             sum_v += fabs(model->_w[i][j]);
//         }
        
//     }
// }

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
    if (len_Lr <= 2 || len_Lc != 0) return -1;

    ann_mpl_model_t* model = (ann_mpl_model_t*) malloc (sizeof(ann_mpl_model_t));
    
    model->active = _active;
    model->layer_size = UA_copy(_layer_size);
    UA_display(&model->layer_size);
    
    int l, v, wb;
    int l_count = Nl_count(model);
    int wb_num = 0;
    int wb_v_mum = 0;
    int cell_num = 0;

    for (l=0; l<l_count; ++l) {
        if (l != l_count -1) {
            wb_num += Nw_count(model, l);
            wb_v_mum += Nw_vcount(model, l);
        }
        cell_num += Ne_count(model, l);
    }
    
    int sz_wb   = sizeof(char*) * (l_count-1) + sizeof(char*) * wb_v_mum + sizeof(vfloat_t) * wb_num;
    model->_w    = (char**) malloc(sz_wb);

    // 内存块切糕。这次十分难切。哈哈哈哈。
    int iwbv = 0;
    char* layer_ptr_base = (char*)model->_w;
    char* wv_ptr_base = layer_ptr_base + sizeof(char*) * (l_count - 1);
    size_t v_offset = 0;
    for (l=0; l<l_count-1; ++l) {
        *((char**)(layer_ptr_base + l * sizeof(char*))) = (char*)(wv_ptr_base + v_offset);
        int v_count = Nw_vcount(model, l);
        for (v=0;v<v_count; ++v) {
            *((char**)(wv_ptr_base + v_offset + v * sizeof(char*))) =  __nw_cal_vptr(model, l, v);
        }
        v_offset += v_count * sizeof(char*);
    }

    return model;
}

int ann_mpl_model_finalize(ann_mpl_model_t* model)
{
    free(model->_w);
    free(model->_cell);
    UArray_(&model->layer_size);
    free(model);
    return 0;
}