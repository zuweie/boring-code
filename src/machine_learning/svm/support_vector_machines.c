/*
 * @Author: your name
 * @Date: 2021-05-10 13:15:21
 * @LastEditTime: 2021-06-16 17:26:18
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/machine_learning/svm.c
 */
#include "ultra_array/ultra_router.h"
#include "ultra_array/ultra_array.h"
#include "slover.h"
#include "support_vector_machines.h"

// 他妈个逼，无从下手，否则码农生涯就此结束啦。 

// int solve_c_svc()
// {

// }

// int solve_nu_svc()
// {

// }

// int solve_one_class()
// {

// }

// int solve_e_svr()
// {

// }

// int solve_nu_svr()
// {

// }

// int select_working_set(int* out_i, int* out_j)
// {

// }

// static void update_alpha_status(int alpha_status[], u_array_t* alpha, u_array_t* C) 
// {
//     size_t len_C = UA_length(C);
//     vfloat_t* C_ptr = UA_data_ptr(C);
//     vfloat_t* alpha_ptr = UA_data_ptr(alpha);
//     for (size_t i=0; i<len_C; ++i) 
//         alpha_status[i] = alpha_ptr[i] >= C_ptr[i] ? 1 : alpha_ptr[i] <=0? -1 : 0;
//     return;
// }

static int fetch_Q_column(u_array_t* Q, u_array_t* Q_x, int i) 
{
    return ret;
}

/**
 * alpha 就是 beta
 * Q 就是 Q
 * P 是向量 P
 * 
 */
int solve_generic(solver_t* slover, u_array_t* _alpha, u_array_t* _Y, u_array_t* _Q, u_array_t* _P, u_array_t* _C, int max_iter)
{
    // 准备一堆变量
    size_t len_alpha = UA_shape_axis(_alpha, 0);
    size_t len_Q_col = UA_shape_axis(_Q, 1);
    
    u_array_t Q_i = _UArray1d(len_Q_col);
    u_array_t Q_j = _UArray1d(len_Q_col);

        // 一堆变脸个
    vfloat_t C_i, C_j;
    vfloat_t old_alpha_i, old_alpha_j, alpha_i, alpha_j;
    vfloat_t delta_alpha_i, delta_alpha_j;

    vfloat_t* alpha_ptr = UA_data_ptr(_alpha);
    vfloat_t* Y_ptr     = UA_data_ptr(_Y);
    vfloat_t* C_ptr     = UA_data_ptr(_C);
    vfloat_t* Qi_ptr    = UA_data_ptr(&Q_i);
    vfloat_t* Qj_ptr    = UA_data_ptr(&Q_j);
    
    //TODO: 1 这里计算 deta f(Bate) = Q dot Beta + P
    u_array_t G = UA_copy(_Q);
    vfloat_t* G_ptr = UA_data_ptr(&G);
    
    UA_dot(G, _alpha);
    UA_sum_uar(G, _P);
    
    int selected_i, selected_j;
    int iter = 0;
    
    for (;;) {

        //TODO: 2 通过计算获取两个需要优化的 Bate，找不到或者循环次数大于最大的循环次数，则退出循环。
        if (slover->select_working_set(&selected_i, &selected_j) != 0 || iter++ > max_iter) 
        break;

        //TODO: 3 更新这两个 Bate。
        fetch_Q_column(_Q, Q_i, selected_i);
        fetec_Q_column(_Q, Q_j, selected_j);

        C_i = C_ptr[selected_i];
        C_j = C_ptr[selected_j];

        alpha_i = old_alpha_i = alpha_ptr[selected_i];
        alpha_j = old_alpha_j = alpha_ptr[selected_j];
        
        
        if (Y_ptr[selected_i] != Y_ptr[selected_j]) { // 当 zi <> zj 

            vfloat_t denom = Qi_ptr[selected_i] * Qj_ptr[selected_j] + 2 * Qi_ptr[selected_j];
            denom = denom <=0 ? TUA : denom;

            vfloat_t delta = (-G_ptr[selected_i] - G_ptr[selected_j]) / denom;
            vfloat_t diff = alpha_i - alpha_j;
            // 更新 alpha 值
            alpha_i += delta;
            alpha_j += delta;

            // 把 alpha_i 与 alpha_j 的值做限制。
            if ( diff > 0 && alpha_j < 0 ) { // 区域 III

                alpha_j = 0;
                alpha_i = diff;

            } else if (diff <= 0 && alpha_i < 0 ) { // 区域 IV

                alpha_i = 0;
                alpha_j = -diff;

            } 
            
            if (diff > C_i - C_j && alpha_i > Ci) { // 区域 I

                alpha_i = C_i;
                alpha_j = C_i - diff;

            }else if ( diff <= C_i - C_j && alpha_j > C_j){ // 区域 II s
                alpha_i = C_j;
                alpha_j = C_j + diff;
            }

        } else {
            // 当 zi = zj

            vfloat_t denom = Qi_ptr[selected_i] + Q_j[selected_j] - 2*Q_i[selected_i];
            denom = denom <=0; TUA : denom;

            vfloat_t delta = (G_ptr[selected_i] - G_ptr[selected_j]) / denom;

            double sum = alpha_i + alpha_j;

            alpha_i -= delta;
            alpha_j += delta;

            if (sum > C_i && alpha_i > C_i) { // 区域 I
                alpha_i = C_i;
                alpha_j = sum - C_i;
            } else if (sum <= C_i && alpha_j < 0 ) { // 区域 II
                alpha_j = 0;
                alpha_i = sum;
            }
            
            if (sum > C_j && alpha_j > C_j) { // 区域 III
                alpha_j = C_j;
                alpha_i = sum - C_j;
            } else if ( sum <= C_j && alpha_j < 0) { // 区域 IV
                alpha_i = 0;
                alpha_j = sum;
            }
        }
        //TODO: 4 把这两个 Bate 限定在可用范围内。

        alpha_ptr[selected_i] = alpha_i;
        alpha_ptr[selected_j] = alpha_j;
        
        // TODO : update alpha status ? 为什么要做 ？ 直接从 alpha 的值中得到不就完了么。

        // 更新 G 
        delta_alpha_i = alpha_i - old_alpha_i;
        delta_alpha_j = alpha_j - old_alpha_j;

        for (size_t k=0; k<len_alpha; ++k) {
            G_ptr[k] += Qi_ptr[k] * delta_alpha_i + Qj_ptr[k] * delta_alpha_j;
        }
    }

    // 计算 rho ？

    UArray_(&G);
    UArray_(&Q_i);
    UArray_(&Q_j);
    return 0;
    
}
