/*
 * @Author: your name
 * @Date: 2021-11-15 16:45:08
 * @LastEditTime: 2021-12-09 17:00:56
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /boring-code/src/machine_learning/neural_network.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <float.h>
#include <math.h>
#include <string.h>
#include <time.h>
#include "matrix/matrix.h"
#include "neural_network.h"

#define INSPECT_U_MAT() \
({ \
    int ll_count = Nl_count(model);\
    int ll, ee, ee_count;\
    for(ll=0; ll<ll_count; ++ll)\
    { \
        vfloat_t* u_ptr = u_mat[ll]; \
        printf("u-layer %d: ", ll); \
        if (u_ptr) { \
            ee_count = Ne_count(model, ll); \
            for (ee=0; ee<ee_count; ++ee) { \
                printf("%f ", u_ptr[ee]); \
            } \
        } else { \ 
            printf("NULL"); \
        } \
        printf("\n"); \
    } \
})

#define INSPECT_Y_MAT() \
({ \
    int ll_count = Nl_count(model);\
    int ll, ee, ee_count;\
    for(ll=0; ll<ll_count; ++ll){ \
        vfloat_t* y_ptr = y_mat[ll]; \
        printf("y-layer %d: ", ll); \
        if (y_ptr) { \
            ee_count = Ne_count(model, ll); \
            for (ee=0; ee<ee_count; ++ee) { \
                printf("%f ", y_ptr[ee]); \
            } \
        } else { \ 
            printf("NULL"); \
        } \
        printf("\n");\
    } \
})

#define INSPECT_W_MAT() \
({ \    
    int ll_count = Nl_count(model);\
    int ll, ee, ee_count;\
    for(ll=0; ll<ll_count; ++ll)\
    { \
        if (model->_w_mat[ll]) { \
            printf("w-layer %d: \n", ll); \
            int kk_count = Wm_k(model, ll); \
            int hh_count = Wm_h(model, ll); \
            for (int kk=0; kk<kk_count; ++kk) { \
                vfloat_t* wvv_ptr = Wk_ptr(model, ll, kk); \
                for (int hh=0; hh<hh_count; ++hh) { \
                    printf("%f ", wvv_ptr[hh]); \
                } \
                printf("\n");\
            } \
        } else { \ 
            printf("w-layer %d: NULL", ll); \
        } \
        printf("\n");\
    } \
})

#define INSPECT_DELTA_MAT() \
({ \
    int ll_count = Nl_count(model);\
    int ll, ee, ee_count;\
    for(ll=0; ll<ll_count; ++ll){ \
        vfloat_t* delta_ptr = delta_mat[ll]; \
        printf("delta-layer %d: ", ll); \
        if (delta_ptr) { \
            ee_count = Ne_count(model, ll); \
            for (ee=0; ee<ee_count; ++ee) { \
                printf("%f ", delta_ptr[ee]); \
            } \
        } else { \ 
            printf("NULL"); \
        } \
        printf("\n");\
    } \
})

// static double __do_active_u(ann_mpl_param_t* params, double u)
// {
//     double alpha = params->alpha;
//     double beta  = params->beta;
    
//     double v = beta * ((1 - exp(-alpha * u)) / (1 + exp(-alpha * u)));
//     return v;
// }

// static double __do_dactive_du(ann_mpl_param_t* params, double u) 
// {
//     double alpha = params->alpha;
//     double beta  = params->beta;
//     double v1 = exp(-alpha * u);
//     double v2 = 1+exp(-alpha*u);
//     double v = 2 * alpha * beta * exp(-alpha * u) / ((1+exp(-alpha*u)) * (1+exp(-alpha*u)));
//     return v;
// }
// 输入 u 的激活函数
static void __active_u(ann_mpl_model_t* model, matrix_t* u) 
{
    double alpha = model->params.alpha;
    double beta  = model->params.beta;

    int len = u->rows * u->cols;
    for (int i=0; i<len; ++i) {

        u->pool[i] = beta * ((1 - exp(-alpha * u->pool[i])) / (1 + exp(-alpha * u->pool[i])));
    }
}
// u 的激活函数对 u 的导数
static void __dactive_du(ann_mpl_model_t* model, matrix_t* u) 
{
    double alpha = model->params.alpha;
    double beta  = model->params.beta;

    int len = u->rows * u->cols;

    for (int i=0; i<len; ++i) {
        u->pool[i] = \
            2 * alpha * beta * exp(-alpha * u->pool[i]) / ((1+exp(-alpha*u->pool[i])) * (1+exp(-alpha*u->pool[i])));
    }
}

static double __calculate_e(vfloat_t* y, vfloat_t* t, int size)
{
    double e = 0.f;
    for (int i=0; i<size; ++i) {
        e += (t[i] - y[i]) * (t[i] - y[i]);
    }
    return e * 0.5;
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
    vfloat_t* layer_size_ptr = UA_data_ptr(layer_size);

    
    if ((int)layer_size_ptr[0] != UA_shape_axis(X, 1)
    || (int)layer_size_ptr[UA_length(layer_size)-1] != UA_shape_axis(Y, 1)) 
        return NULL;

    // 开始各种骚操作。
    ann_mpl_model_t* model = ann_mpl_model_create(layer_size, params);
    //INSPECT_W_MAT();
    // TODO 1: forward propagation to calculate the E
    // TODO 2: backward propagation to update w mat
    
    // 初始化一些需要用到的变量。
    double Pre_E = DBL_MAX * 0.5, E = 0.f;
    matrix_t u1_mat = Mat_create(1, 1);
    matrix_t y1_mat = Mat_create(1, 1);
    matrix_t delta1_mat = Mat_create(1, 1);
    
    matrix_t du1_mat = Mat_create(1, 1);
    matrix_t dEdw1_mat = Mat_create(1, 1);
    matrix_t w1_mat = Mat_create(1, 1);
    matrix_t t1_mat = Mat_create(1, 1);

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
    int delta_num = 0;
    for (int l=0; l<l_count; ++l)
    {
        y_num += Ne_count(model, l);
        // 第一层的 u 不要。
        if (l > 0) u_num += Ne_count(model, l);
        // 第一层的 delta 不要
        if (l > 0) delta_num += Ne_count(model, l);
    } 
    // 
    char** u_mat;
    char** y_mat;
    char** delta_mat;

    u_mat = (char**) malloc( sizeof(char*) * l_count + sizeof(vfloat_t) * u_num);
    y_mat = (char**) malloc( sizeof(char*) * l_count + sizeof(vfloat_t) * y_num);
    delta_mat = (char**) malloc( sizeof(char*) * l_count + sizeof(vfloat_t) * delta_num);
    
    char* u_data_base = (char*)u_mat + sizeof(char*) * l_count;
    char* y_data_base = (char*)y_mat + sizeof(char*) * l_count;
    char* delta_mat_base = (char*) delta_mat + sizeof(char*) * l_count;

    size_t u_base_offset = 0;
    size_t y_base_offset = 0;
    size_t delta_base_offset = 0;

    for (l=0;l<l_count; ++l) {

        if (l==0) {
            u_mat[l] = NULL;
            delta_mat[l] = NULL;
        } else {
            u_mat[l] = u_data_base + u_base_offset;
            u_base_offset += sizeof(vfloat_t) * Ne_count(model, l);

            delta_mat[l] = delta_mat_base + delta_base_offset;
            delta_base_offset += sizeof(vfloat_t) * Ne_count(model, l);
        }

        y_mat[l] = y_data_base + y_base_offset;
        y_base_offset += sizeof(vfloat_t) * Ne_count(model, l);
    }
    
    
    for (int step=0; step<params->max_iter; ++step) {
        // 
        if (step % sample_count == 0 ) {
            // 走完一圈，检测那个误差是否已经收敛了。收敛了就 break，跳出循环。
            // 否则将样本的顺序打乱，在来一遍。
            //printf("step %d, Pre_e %f, E %f , fabs(Pre_E - E) %f\n", step, Pre_E, E, fabs(Pre_E - E));
            if (fabs(Pre_E - E) < params->epsilon)
            {
                //printf("o fuck is come out!!!\n");
                break;
            }
            Pre_E = E;
            srand((unsigned int)time(NULL));
            for (int i = 0; i<sample_count; ++i) {
                int j = rand() % sample_count;
                int k = rand() % sample_count;
                int tmp = sample_index[j];
                sample_index[j] = sample_index[k];
                sample_index[k] = tmp;
            }
        }

        idx = sample_index[step%sample_count];

        memcpy(y_mat[0], sample_ptr[idx], per_sample_count*sizeof(vfloat_t));
        

        //printf("inspect W_mat \n");
        //INSPECT_W_MAT();

        // 走! 出发！向前传播！
        for(l=1; l<l_count; ++l) {
            
            k = Wm_k(model, l);
            h = Wm_h(model, l);
            vfloat_t* wptr = Wk_ptr(model, l, 0);

            Mat_reload(&y1_mat, Ne_count(model, l-1), 1, y_mat[l-1]);
            Mat_reshape(&y1_mat, y1_mat.rows+1, 1);
            Mat_put(&y1_mat, y1_mat.rows-1, 0, 1.f);

            Mat_reload(&w1_mat, k, h, wptr);

            // dot 后的结果就是 ul，将 ul 存入 u_ptr 中。
            Mat_dot(&w1_mat, &y1_mat);
            Mat_save(&w1_mat, u_mat[l]);

            // 然后做激活函数，然后再将结果放入 y_mat[l] 中。
            __active_u(model, &w1_mat);
            Mat_save(&w1_mat, y_mat[l]);
        }

        E += __calculate_e(y_mat[l-1], response_ptr[idx], Ne_count(model, l_count-1));
        //printf("step %d\n", step);
        // INSPECT_U_MAT();
        // printf("\n");
        // INSPECT_Y_MAT();
        // printf("\n");
        // 走向后传播。
        for (l=l_count-1; l>0; l--) {

            if(l == l_count -1) {
                // 当 l 是输出层的时候。
                //printf(" last layer \n\n");
                k = Ne_count(model, l);
                Mat_reload(&du1_mat, k, 1, u_mat[l]);
                //printf("inspect last layer befort dactive du\n");
                //Mat_inspect(&du1_mat);
                __dactive_du(model, &du1_mat);
                //printf("inspect last layer dactve_du\n");
                //Mat_inspect(&du1_mat);
                //printf("inspect last layer y1_mat\n");
                Mat_reload(&y1_mat, k, 1, y_mat[l]);
                //Mat_inspect(&y1_mat);
                //printf("inspect last layer t1_mat\n");
                Mat_reload(&t1_mat, k, 1, response_ptr[idx]);
                //Mat_inspect(&t1_mat);
                
                Mat_op_mat(&y1_mat, &t1_mat, op_sub);
                //printf("inpsect &y1_mat sub &t1_mat\n");
                //Mat_inspect(&y1_mat);
                Mat_op_mat(&y1_mat, &du1_mat,op_multi);
                Mat_save(&y1_mat, delta_mat[l]);
                //printf("the last layer of delta_mat\n ");
                //Mat_inspect(&y1_mat);
            } else {
                //printf("not the last layer \n\n");
                // 当 l 是中间层的时候，那就非常鸡吧复杂了。
                int cur_layer_k = Ne_count(model, l);
                int next_layer_wk = Wm_k(model, l+1);
                int next_layer_wh = Wm_h(model, l+1);
                
                Mat_reload(&du1_mat, cur_layer_k, 1, u_mat[l]);
                //printf("befort dactive_du\n");
                //Mat_inspect(&du1_mat);
                __dactive_du(model, &du1_mat);
                //printf("after dactive_du\n");
                //Mat_inspect(&du1_mat);
                // 这里要提出上一层的参数矩阵
                Mat_reload(&w1_mat, next_layer_wk, next_layer_wh, Wk_ptr(model, l+1, 0));
                // 将其装置
                Mat_transpose(&w1_mat);
                //printf("inspect w1_mat\n");
                //Mat_inspect(&w1_mat);
                // 最后一行不要，那是因为最后一行时 bais， 也就是偏置量 b，不要于上一层的 delta 相乘。
                Mat_reshape(&w1_mat, w1_mat.rows-1, w1_mat.cols);

                Mat_reload(&delta1_mat, next_layer_wk, 1, delta_mat[l+1]);
                //printf("inspect delta1_mat\n");
                //Mat_inspect(&delta1_mat);

                //printf("inspect w1 multi delta1\n");
                Mat_op_mat(&w1_mat, &delta1_mat, op_multi);
                //Mat_inspect(&w1_mat);

                //printf("inspect &w1_mat deflate add \n");
                Mat_deflate(&w1_mat, dimen_col, op_add);
                //Mat_inspect(&w1_mat);
                //printf("inspect &w1_mat multi &du1_mat\n");
                Mat_op_mat(&w1_mat, &du1_mat, op_multi );
                //Mat_inspect(&w1_mat);
                Mat_save(&w1_mat, delta_mat[l]);
            }

            // 算完 delta_mat,然后 update 那个 w 矩阵了
            
            // int k_delta = Ne_count(model, l);
            // int h_y_1   = Ne_count(model, l-1);
            // vfloat_t* delta_ptr = delta_mat[l];
            // vfloat_t* y_1_ptr   = y_mat[l-1];

            // Mat_reshape(&dEdw1_mat, k_delta, h_y_1);
            // Mat_eptr(&dEdw1_mat, dEdw_ptr);
            // for (int i=0; i<k_delta; ++i) {
            //     for (int j=0; j<h_y_1; ++j) {
            //         dEdw_ptr[i][j] = delta_ptr[i] * y_1_ptr[j];
            //     }
            // }
            // Mat_op_numberic(&dEdw1_mat, -0.01, op_multi);
            // Mat_reload(&w1_mat, k_delta, h_y_1, Wk_ptr(model, l, 0));
            // Mat_op_mat(&w1_mat, &dEdw1_mat, op_add);
            // Mat_save(&w1_mat, Wk_ptr(model, l, 0));
        }
        // printf("\n");
        // INSPECT_DELTA_MAT();
        // printf("\n");
        //printf("update W with dW \n");
        for (l=l_count-1; l>0; --l) {
            h = Ne_count(model, l-1);
            k = Ne_count(model, l);
            //printf("inspect y_mat[l-1]\n");
            Mat_reload(&y1_mat, h, 1, y_mat[l-1]);
            Mat_reshape(&y1_mat, y1_mat.rows + 1, 1);
            Mat_put(&y1_mat, y1_mat.rows-1, 0, 1.f);

            //Mat_inspect(&y1_mat);

            //printf("inspect delta_mat[l]\n");
            Mat_reload(&delta1_mat, k, 1, delta_mat[l]);
            //Mat_inspect(&delta1_mat);

            Mat_reshape(&dEdw1_mat, k, h+1);
            Mat_eptr(&dEdw1_mat, dEdw_ptr);

            for (int i=0; i<k; ++i) {
                for (int j=0; j<h+1;++j) {
                    dEdw_ptr[i][j] = delta1_mat.pool[i] * y1_mat.pool[j];
                }
            }
            //printf("inspect dEdw1_mat\n");
            //Mat_inspect(&dEdw1_mat);

            Mat_op_numberic(&dEdw1_mat, -params->down_scale, op_multi);
            //printf("inspect after multi &dEdw1 -0.01\n");
            //Mat_inspect(&dEdw1_mat);

            //printf("inspect before update &w1_mat \n");
            int wk = Wm_k(model, l);
            int wh = Wm_h(model, l);
            Mat_reload(&w1_mat, wk, wh, Wk_ptr(model, l, 0));
            //Mat_inspect(&w1_mat);
            //printf("inspect after update &w1_mat \n");
            Mat_op_mat(&w1_mat, &dEdw1_mat, op_add);
            //Mat_inspect(&w1_mat);
            Mat_save(&w1_mat, Wk_ptr(model, l, 0));
        }
    }


    // end
    free(u_mat);
    free(y_mat);
    free(delta_mat);

    Mat_destroy(&u1_mat);
    Mat_destroy(&y1_mat);
    Mat_destroy(&delta1_mat);
    
    Mat_destroy(&du1_mat); 
    Mat_destroy(&dEdw1_mat); 
    Mat_destroy(&w1_mat);
    Mat_destroy(&t1_mat);
    return model;
}

int ann_mpl_predict(ann_mpl_model_t* model, u_array_t* sample, u_array_t* prediction)
{
    int l, k, h, l_count = Nl_count(model);
    matrix_t w1_mat = Mat_create(1, 1);
    matrix_t y1_mat = Mat_create(1, 1);
    Mat_reload(&y1_mat, UA_length(sample), 1, UA_data_ptr(sample));

    for (l=1; l<l_count; ++l) {
        int k = Wm_k(model, l);
        int h = Wm_h(model, l);
        Mat_reload(&w1_mat, k, h, Wk_ptr(model, l, 0));
        Mat_reshape(&y1_mat, y1_mat.rows+1, 1);
        Mat_put(&y1_mat, y1_mat.rows, 0, 1.f);
        Mat_dot(&w1_mat, &y1_mat);

        __active_u(model, &w1_mat);

        Mat_reshape(&y1_mat, Ne_count(model, l), 1);

        Mat_save(&w1_mat, y1_mat.pool);
    }
    Mat_save(&y1_mat, UA_data_ptr(prediction));
    return 0;
}


ann_mpl_model_t* ann_mpl_model_create(u_array_t* _layer_size, ann_mpl_param_t* params)
{
    ann_mpl_model_t* model = (ann_mpl_model_t*) malloc (sizeof(ann_mpl_model_t));
    
    model->params = *params;
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

    // int sz_u0 = sizeof(char*) * (l_count) + sizeof(vfloat_t) * u0_num;
    // model->_u0 = (char**) malloc(sz_u0);
    // layer_ptr_base = (char*) model->_u0;
    // char* u0_ptr_base = layer_ptr_base + sizeof(char*) * l_count;
    // size_t u0_offset = 0;
    // for (l=0; l<l_count; ++l) {
    //     model->_u0[l] = u0_ptr_base + u0_offset;
    //     u0_offset += Ne_count(model, l) * sizeof(vfloat_t);
    // }

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
            svh = 0.f;
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