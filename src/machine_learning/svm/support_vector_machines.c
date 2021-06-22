/*
 * @Author: your name
 * @Date: 2021-05-10 13:15:21
 * @LastEditTime: 2021-06-22 15:24:48
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/machine_learning/svm.c
 */
#include "ultra_array/ultra_router.h"
#include "ultra_array/ultra_array.h"
#include "support_vector_machines.h"

// 他妈个逼，无从下手，否则码农生涯就此结束啦。 

// svc 的 svm 支持向量机的实现
// X 为数据
// Y 为标志量
// M 为输出的 model




/**
 * 
 */
int solve_generic(solver_t* slover, svm_model_t* model)
{
    // 准备一堆变量
    size_t len_alpha = UA_length(&solver->alpha);
    size_t len_Qc    = UA_shape_axis(&solver->Q, 1);
    
        // 一堆变脸个
    vfloat_t C_i, C_j;
    vfloat_t old_alpha_i, old_alpha_j, alpha_i, alpha_j;
    vfloat_t delta_alpha_i, delta_alpha_j;

    vfloat_t* alpha_ptr = UA_data_ptr(&solver->alpha);
    vfloat_t* Y_ptr     = UA_data_ptr(&solver->Y);
    vfloat_t* C_ptr     = UA_data_ptr(&solver->C);

    vfloat_t (*Qc_ptr)[len_Qc] = UA_data_ptr(&solver->Q);
    
    //TODO: 1 这里计算 deta f(Bate) = Q dot Beta + P
    vfloat_t* G_ptr = UA_data_ptr(&solver->G);
    
    UA_dot(&solver->G, &solver->alpha);
    UA_sum_uar(&solver->G, &solver->P);
    
    int selected_i, selected_j;
    int iter = 0;
    
    for (;;) {

        //TODO: 2 通过计算获取两个需要优化的 Bate，找不到或者循环次数大于最大的循环次数，则退出循环。
        if (slover->select_working_set(solver,&selected_i, &selected_j) != 0 || iter++ > solver->max_iter) 
        break;

        //TODO: 3 更新这两个 Bate。
        vfloat_t* Qi_ptr = Qc_ptr[selected_i];
        vfloat_t* Qj_ptr = Qc_ptr[selected_j];
        

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
            
            if (diff > C_i - C_j && alpha_i > C_i) { // 区域 I

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

    // TODO：计算 rho
    // TODO: 计算 f(Beta)

    return 0;
    
}

// 开始计算

int solve_c_svc( \
        u_array_t* X, u_array_t* Y, \
        SVM_type svm_type,  \
        SVM_kernel svm_kernel, \
        vfloat_t _C, vfloat_t _gammer, \ 
        vfloat_t _coef, vfloat_t _degree, \
        double eps, \
        int max_iter)
{
    // 初始化所有的
    solver_t solver;
    svm_model_t model;
    //1 初始化运行的空间
    Solver_initialize(&solver, svm_type, svm_kernel, X, Y, _C, _gammer, _coef, _degree, eps, max_iter);

    //2 初始化 csvc 的参数。
    UA_ones(&solver->alpha, 0);
    
    
    solve_generic(&solver, &model);
    Solver_finalize(&solver);
}
