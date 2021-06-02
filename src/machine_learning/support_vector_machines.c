/*
 * @Author: your name
 * @Date: 2021-05-10 13:15:21
 * @LastEditTime: 2021-06-03 01:15:02
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/machine_learning/svm.c
 */
#include "ultra_array/ultra_array.h"
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
    
}

/**
 * alpha 就是 beta
 * Q 就是 Q
 * P 是向量 P
 * 
 */
int solve_generic(u_array_t* _alpha, u_array_t* _Q, u_array_t* _P, u_array_t* _C, int max_iter)
{
    // 构建惩罚参数数组C。
    size_t len_alpha = UA_shape_axis(_alpha, 0);
    vfloat_t* b
    int alpha_staus[len_alpha];
    
    u_array_t G = UA_copy(_Q);

    // 1 这里计算 deta f(Bate) = Q dot Beta + P
    UA_dot(G, _alpha);
    UA_sum_uar(G, _P);
    
    int out_i, out_j;
    int iter = 0;
    
    u_array_t Q_i;
    u_array_t Q_j;

    for (;;) {

        // 2 通过计算获取两个需要优化的 Bate，找不到则退出循环。
        if (select_working_set(&out_i, &out_j) != 0 || iter++ > max_iter) 
        break;

        // 3 更新这两个 Bate。
        vfloat_t C_i = _C[out_i];
        vfloat_t C_j = _C[out_j];
        
        // 4 把这两个 Bate 限定在可用范围内。

    }
    
    
}
