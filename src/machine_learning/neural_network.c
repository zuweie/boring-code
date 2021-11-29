/*
 * @Author: your name
 * @Date: 2021-11-15 16:45:08
 * @LastEditTime: 2021-11-29 16:12:22
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /boring-code/src/machine_learning/neural_network.c
 */
#include <stdlib.h>
#include <math.h>
#include <string.h>
#include "matrix/matrix.h"
#include "neural_network.h"

// 输入 u 的激活函数
static double __ann_mpl_active_u(ann_mpl_model_t* model, double u) 
{
    
}
// u 的激活函数对 u 的导数
static double __ann_mpl_dactive_du(ann_mpl_model_t* model, double u) 
{

}
static char* __nw_cal_wbase_ptr(ann_mpl_model_t* model) 
{  
    char* base = (char*)(model)->_w_mat; 
    int l_count = Nl_count(model); 
    char* dest = base + (l_count) * sizeof(char*); 
    int k_count = 0; 
    for (int l=1; l<l_count; ++l) { 
        k_count += Wm_k(model, l); 
    } 
    dest += k_count * sizeof(char*); 
    return dest; 
}

static char* __nw_cal_vptr(ann_mpl_model_t* model, int l, int k) 
{ 
    int w_num = 0; 
    /* 计算并非当前层数的 w 的数量 */ 
    for (int il=1; il<l; ++il) { 
        w_num += Nw_count(model, il); 
    }
    w_num += Wm_h(model, l) * (k);
    char* base = (char*)__nw_cal_wbase_ptr(model); 
    base = (char*)(base + w_num*sizeof(vfloat_t));
    return base;
}

ann_mpl_model_t* ann_mpl_training(u_array_t* layer_size, u_array_t* X, u_array_t* Y, ann_mpl_param_t* params)
{
    ann_mpl_model_t* model = ann_mpl_model_create(layer_size, params->active);

    // TODO 1: forward propagation to calculate the E
    // TODO 2: backward propagation to update w mat
    
}

int ann_mpl_forward_propagation(ann_mpl_model_t* model, u_array_t* input, u_array_t* output)
{
    size_t len_input = UA_length(input);
    int l,k,h,l_count = Nl_count(model);
    u_array_t w_mat = _UArray1d(1);
    u_array_t x_mat = UA_copy(input);
    vfloat_t* u0_ptr = model->_u0[0];
    UA_export(input, u0_ptr);

    for (l=1; l<l_count; ++l) {
        k = Wm_k(model, l);
        h = Wm_h(model, l);
        vfloat_t* wk_ptr = Wk_ptr(model, l, 0);
        UA_reshape_dots(&w_mat, 2, k, h);
        UA_load(&w_mat, wk_ptr);

        int x_r = UA_shape_axis(&x_mat, 0);
        int x_c = UA_shape_axis(&x_mat, 1);
        UA_reshape_dots(&x_mat, 2, x_r, x_c+1);
        vfloat_t* x_mat_ptr = UA_data_ptr(&x_mat);
        x_mat_ptr[x_c+1] = 1.f;
        
        UA_dot(&w_mat, &x_mat);
        u0_ptr = model->_u0[l];
        UA_export(&w_mat, u0_ptr);

        // 每个 u0 做激活。作为下一个的输入进入下一个层计算。
        vfloat_t* w_mat_ptr = UA_data_ptr(&w_mat);
        for (size_t i=0; i<UA_length(&w_mat); ++i) {
            w_mat_ptr[i] = __ann_mpl_active(model, w_mat_ptr[i]);
        }
        UA_absorb(&x_mat, &w_mat);
    }
    
    // 
    UA_absorb(output, &x_mat);
    return 0;
}

int ann_mpl_backward_propagation(ann_mpl_model_t* model, u_array_t* y, u_array_t* t)
{
    // 一切重最后一层开始。然后做 w mat 矩阵的 update
    int l,k,h,l_count = Nl_count(model);
    u_array_t grad
    for (l=l_count-1; l>=0; --l) 
    {
        
    }

    
}

int ann_mpl_predict(ann_mpl_model_t* model, u_array_t* sample, u_array_t* prediction)
{

}

ann_mpl_model_t* ann_mpl_model_create(u_array_t* _layer_size, active_func_t _active)
{
    ann_mpl_model_t* model = (ann_mpl_model_t*) malloc (sizeof(ann_mpl_model_t));
    
    model->active = _active;
    model->layer_size = UA_copy(_layer_size);
    UA_display(&model->layer_size);
    
    int l, k, wb;
    int l_count = Nl_count(model);
    int wm_len = 0;
    int wm_k_num = 0;
    int u0_num = 0;
    for (l=0; l<l_count; ++l) {
        if (l !=0 ) {
            wm_len   += Nw_count(model, l);
            wm_k_num += Wm_k(model, l);
        }
        u0_num += Ne_count(model, l);
    }
    
    int sz_wb   = sizeof(char*) * (l_count) + sizeof(char*) * wm_k_num + sizeof(vfloat_t) * wm_len;
    model->_w_mat    = (char**) malloc(sz_wb);

    // wb 内存块切糕。这次十分难切。
    char* layer_ptr_base = (char*)model->_w_mat;
    char* wv_ptr_base = layer_ptr_base + sizeof(char*) * (l_count);
    size_t k_offset = 0;
    // 第一个层级指针置0;
    *((char**)layer_ptr_base) = NULL;
    for (l=1; l<l_count; ++l) {
        *((char**)(layer_ptr_base + l * sizeof(char*))) = (char*)(wv_ptr_base + k_offset);
        int k_count = Wm_k(model, l);
        for (k=0;k<k_count; ++k) {
            *((char**)(wv_ptr_base + k_offset + k * sizeof(char*))) =  __nw_cal_vptr(model, l, k);
        }
        k_offset += k_count * sizeof(char*);
    }

    int sz_u0 = sizeof(char*) * (l_count) + sizeof(vfloat_t) * u0_num;
    model->_u0 = (char**) malloc(sz_u0);
    layer_ptr_base = (char*) model->_u0;
    char* u0_ptr_base = layer_ptr_base + sizeof(char*) * l_count;
    size_t u0_offset = 0;
    for (l=0; l<l_count; ++l) {
        model->_u0[l] = u0_ptr_base + u0_offset;
        u0_offset += Ne_count(model, l) * sizeof(vfloat_t);
    }

    // 实际算法中其实不需要保存 cell 的内存，所以这段代码暂时不要了。
    // cell 内存切糕。比 wb 简单一点。
    // int sz_ce   = sizeof(char*) * l_count + sizeof(vfloat_t) * cell_num;
    // model->_cell = (char**) malloc (sz_ce);
    // int sz_cell  = sizeof(char*) * l_count + sizeof(vfloat_t) * cell_num;
    // model->_cell = (char**) malloc (sz_cell);
    // layer_ptr_base = (char*)model->_cell;
    // char* cv_ptr_base = layer_ptr_base + sizeof(char*) * l_count;

    // v_offset = 0;
    // for (l=0; l<l_count; ++l) {
    //     *((char**)(layer_ptr_base + l * sizeof(char*))) = (char*)(cv_ptr_base + v_offset);
    //     v_offset += Ne_count(model, l) * sizeof(vfloat_t);
    // }

    // 最后将 w_mat 矩阵初始化。
    double vh, vk, svh;
    int h;
    for (l=1; l<l_count; ++l) {

        int n2 = Ne_count(model, l);
        int n1 = Ne_count(model, l-1);
        double G = 0.7 * pow(n1, 1.f/ (n2 -1));
            
        for (k=0; k<Wm_k(model, l); ++k) {

            vfloat_t* wk_ptr = Wk_ptr(model, l, k);
                
            for (h=0; h<Wm_h(model, l); ++h) {
                vh = ann_mpl_rand1(-1, 1);
                wk_ptr[h] = vh;
                svh += fabs(vh);
            }
            // 减掉最后一个。
            svh = svh - vh;

            for (h=0; h<Wm_h(model, l)-1; ++h) {
                wk_ptr[h] = wk_ptr[h] / svh;
            }

            vk = ann_mpl_rand1(-1, 1);
            wk_ptr[h] = ((float)(2*k) / (float)n2 -1) * vk * G;
        }
    }

    return model;
}

int ann_mpl_model_finalize(ann_mpl_model_t* model)
{
    free(model->_w_mat);
    free(model->_u0);
    UArray_(&model->layer_size);
    free(model);
    return 0;
}

double ann_mpl_rand1(int from, int to) 
{
    int r = rand() % ((to+1) * 10000 + 1) ;
    double v = (double) r / (double) 10000 + from;
    return v;
}