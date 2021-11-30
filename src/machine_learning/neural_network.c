/*
 * @Author: your name
 * @Date: 2021-11-15 16:45:08
 * @LastEditTime: 2021-11-30 16:01:30
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /boring-code/src/machine_learning/neural_network.c
 */
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <string.h>
#include "matrix/matrix.h"
#include "neural_network.h"

// 输入 u 的激活函数
static double __active_u(ann_mpl_model_t* model, vfloat_t* y, vfloat_t* u, int size) 
{
    
}
// u 的激活函数对 u 的导数
static double __dactive_du(ann_mpl_model_t* model, double u) 
{

}

static double __calculate_e(vfloat_t* y, vfloat_t* t, int size)
{
    double e = 0.f;
    for (int i=0; i<size; ++i) {
        e += (t[i] - y[i]) * (t[i] - y[i]) 
    }
    return e * 0.5;
}


static void __calculate_delta_1(vfloat_t* delta, vfloat_t* y, vfloat_t* t, vfloat_t* u, int size) 
{
    for (int i=0; i<size; ++i) {
        delta[i] = (y[i] - t[i]) * __dactive_du(u[i]);
    }
    return;
}

static void __calculate_delta_2(vfloat_t* o_delta, vfloat_t* i_delta, vfloat_t* u, vfloat_t* w, int size) 
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

    // 检测输入 X 的列数于输入层的 列数是否一致，不一致返回null
    vfloat_t* layer_size_pr = UA_data_ptr(layer_size);

    if ((int)layer_size_pr[0] != UA_shape_axis(X, 1)
    || (int)layer_size_ptr[layer_size->axis_n-1] != UA_shape_axis(Y, 1)) 
        return NULL;

    // 开始各种骚操作。
    ann_mpl_model_t* model = ann_mpl_model_create(layer_size, params->active);

    // TODO 1: forward propagation to calculate the E
    // TODO 2: backward propagation to update w mat
    
    // 初始化一些需要用到的变量。
    double Pre_E = DBL_MAX * 0.5, E = 0.f;
    matrix_t u0_mat;
    matrix_t y0_mat;
    matrix_t w_mat = Mat_create(1, 1);
    matrix_t delta_mat1 = Mat_create(1, 1);
    matrix_t delta_mat2 = Mat_create(1, 1);
    matrix_t dw_mat = Mat_create(1, 1);

    int sample_count     = UA_shape_axis(X, 0);
    int per_sample_count = UA_shape_axis(X, 1);
    int per_response_count = UA_shape_axis(Y, 1);

    int sample_index[sample_count];
    vfloat_t (*sample_ptr)[per_sample_count] = UA_data_ptr(X);
    vfloat_t (*response_ptr)[per_response_count] = UA_data_ptr(Y);

    int idx;
    for (int i=0; i<sample_count; ++i) {
        sample_index[i] = i;
    }

    int l, k, h, l_count = Nl_count(model);
    int y_num = 0;
    int u_num = 0;
    for (int l=0; l<l_count; ++l)
    {
        y_num += Ne_count(model, l);
        // 第一层的 u 不要。
        if (l > 0) u_num += Ne_count(model, l);
    } 
    // 
    char** u_mat, y_mat;
    u_mat = (char**) malloc( sizeof(char*) * l_count + sizeof(vfloat_t) * u_num);
    y_mat = (char**) malloc( sizeof(char*) * l_count + sizeof(vfloat_t) * y_num);

    char* u_data_base = (char*)u_mat + sizeof(char*) * l_count;
    char* y_data_base = (char*)y_mat + sizeof(char*) * l_count;

    size_t u_base_offset = 0;
    size_t y_base_offset = 0;
    
    for (l=0;l<l_count; ++l) {

        if (l=0)  
            u_mat[l] = NULL;
        else {
            u_mat[l] = u_data_base + u_base_offset;
            u_base_offset + sizeof(vfloat_t) * Ne_count(model, l);
        }

        y_mat[l] = y_data_base + y_base_offset;
        y_base_offset += sizeof(vfloat_t) * Ne_count(model, l);
    }
    

    for (int step=0; step<params->max_iter; ++step) {
        // 
        if (step % sample_count == 0 ) {
            // 走完一圈，检测那个误差是否已经收敛了。收敛了就 break，跳出循环。
            // 否则将样本的顺序打乱，在来一遍。
            if (fabs(Per_E - E) < params->epsilon)
                break;

            for (int i = 0; i<sample_count; ++i) {
                int j = rand() % sample_count;
                int k = rand() % sample_count;
                int tmp = sample_index[j];
                sample_index[j] = sample_index[k];
                sample_index[k] = tmp;
            }

        }

        int idx = sample_index[step%sample_count];

        memcpy(y_mat[0], sample_ptr[idx], per_sample_count);
        

        // 走向前传播
        for(l=1; l<l_count; ++l) {
            
            int k = Wm_k(model, l);
            int h = Wm_h(model, l);
            vfloat_t* wptr = Wk_ptr(model, l, 0);

            Mat_set(&y0_mat, h, 1, y_mat[l-1]);
            Mat_put(&y0_mat, y0_mat.rows, 0, 1.f);

            Mat_reload(w_mat, k, h, wptr);

            // dot 后的结果就是 ul，将 ul 存入 u_ptr 中。
            Mat_dot(&w_mat, &y0_mat);
            
            Mat_save(&w_mat, u_mat[l]);

            __active_u(model, y_mat[l], u_mat[l], k);

        }

        E += __calculate_e(y_mat[l], response_ptr[idx], Ne_count(model, l_count-1));


        // 走向后传播。
        for (l=l_count-1; l<0; l--) {
            if(l = l_count -1) {
                // 当 l 是输出层的时候。
                k = Ne_count(model, l);
                Mat_reshpae(&delta_mat1, 1, k);

                __calculate_delta_1(delta_mat1.pool, y_mat[l], response_ptr[idx], u_mat[l], k);
                
            } else {

            }

        }

    }


    // end
    free(u_mat);
    free(y_mat);

    Mat_destroy(&w_mat);
    Mat_destroy(&delta_mat1);
    Mat_destroy(&delta_mat2);
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