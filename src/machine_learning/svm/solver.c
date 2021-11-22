/*
 * @Author: your name
 * @Date: 2021-06-03 13:59:00
 * @LastEditTime: 2021-11-17 14:59:45
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/machine_learning/svm/solver.c
 */
#include <stdio.h>
#include <float.h>
#include <math.h>
#include "ultra_array/ultra_array.h"
#include "ultra_array/ultra_router.h"
#include "svm_kernel_function.h"
#include "solver.h"

int solver_initialize(     
        solver_t* solver,  
        SVM_type svm_type, 
        SVM_kernel kerenl, 
        double _gammer, 
        double _coef,   
        double _degree, 
        double eps,  
        int max_iter 
)
{
    
    // 初始化 select_working_set 与 calc_rho 函数指针。
    if (svm_type == C_SVC || svm_type == EPSILON_SVR || svm_type == ONE_CLASS) {
        // 这里是第一类
        solver->select_working_set = &select_working_set;
        solver->calc_rho = &calc_rho;
    } else {
        // 这里是第二类
        solver->select_working_set = &select_working_nu_svm;
        solver->calc_rho = &calc_rho;
    }

    // 初始化核函数参数
    solver->kernel_param.gammer = _gammer;
    solver->kernel_param.coef0  = _coef;
    solver->kernel_param.degree = _degree;

    // 初始化核函数
    switch (kerenl)
    {
    case LINEAR:
        /* code */
        solver->kernel = &calc_linear;
        break;
    case POLY:
        solver->kernel = &calc_poly;
        break;
    case RBF:
        solver->kernel = &calc_rbf;
        break;
    case SIGMOID:
        solver->kernel = &calc_sigmoid;
        break;
    default:
        solver->kernel = NULL;
        break;
    }
    

    switch (svm_type)
    {
    case C_SVC:
        solver->build_Q = &build_c_svc_Q;
        break;
    case NU_SVC:
        solver->build_Q = &build_nu_svc_Q;
        break;
    case ONE_CLASS:
        solver->build_Q = &build_one_class_Q;
        break;
    case EPSILON_SVR:
        solver->build_Q = &build_e_svr_Q;
        break;
    case NU_SVR:
        solver->build_Q = &build_nu_svr_Q;
        break;
    default:
        solver->build_Q = NULL;
        break;
    }

    // 初始化一些 uarray。
    solver->alpha = _UArray1d(1);
    solver->Q     = _UArray2d(1,1);
    solver->G     = _UArray2d(1,1);
    solver->P     = _UArray1d(1);
    
    // max_iter;
    solver->max_iter = max_iter;

    // eps
    solver->eps = eps;
}
int solver_set_calculating_dataset(solver_t* solver, u_array_t* _X, u_array_t* _Y, u_array_t* _C)
{
    // 根据 X 与 Y 更新 alpha, Q, G, C, P 这几个 u_array_t 的内存。
    // alpha

    solver->X = _X;
    solver->Y = _Y;
    solver->C = _C;

    size_t len_Y = UA_length(solver->Y);
    UA_reshape_dots(&solver->alpha, 1, len_Y);

    // Q
    UA_reshape_dots(&solver->Q, 2, len_Y, len_Y);

    // G 要复制 Q
    UA_reshape_dots(&solver->G, 2, len_Y, len_Y);

    // P
    UA_reshape_dots(&solver->P, 1, len_Y);
    
}

int solver_finalize(solver_t* solver)
{
    UArray_(&solver->G);
    UArray_(&solver->alpha);
    UArray_(&solver->Q);
    UArray_(&solver->P);
}

int solver_is_lower_bound(solver_t* solver, int i) 
{
    vfloat_t* alpha_ptr = UA_data_ptr(&solver->alpha);
    vfloat_t* C_ptr     = UA_data_ptr(solver->C);

    return alpha_ptr[i] <= 0;
}

int solver_is_upper_bound(solver_t* solver, int i)
{
    vfloat_t* alpha_ptr = UA_data_ptr(&solver->alpha);
    vfloat_t* C_ptr     = UA_data_ptr(solver->C);
    return alpha_ptr[i] >= C_ptr[i];
}

// 第一类的svm Betai 与 Betaj 的选择器。
int select_working_set(solver_t* solver, int* out_i, int* out_j)
{
    vfloat_t* Y_ptr   = UA_data_ptr(solver->Y);
    vfloat_t* G_ptr   = UA_data_ptr(&solver->G);
    size_t len_alpha = UA_length(&solver->alpha);

    double Gmax1  = -DBL_MAX;
    int Gmax1_idx = -1;

    double Gmax2  = -DBL_MAX;
    int Gmax2_idx = -1;
    int i;

    for (i=0; i<len_alpha; ++i) {
        double t;
        if (Y_ptr[i] > 0) {
            
            if (! solver_is_upper_bound(solver, i) && (t = -G_ptr[i]) > Gmax1) {

                Gmax1 = t; // 更新最大值
                Gmax1_idx = i;
            }
            if ( ! solver_is_lower_bound(solver, i) && (t = G_ptr[i]) > Gmax2) {

                Gmax2 = t; // 更新最大值
                Gmax2_idx = i;
            }

        } else {
            if ( ! solver_is_upper_bound(solver, i) && (t = -G_ptr[i]) > Gmax2 ) {
                Gmax2 = t;
                Gmax2_idx = i;
            }
            if ( !solver_is_lower_bound(solver, i) && (t = G_ptr[i]) > Gmax1 ) {
                Gmax1 = t;
                Gmax1_idx = i;
            }
        }
    }
    
    *out_i = Gmax1_idx;
    *out_j = Gmax2_idx;
    // Debug 
    //printf("(Gmax1:%lf) + (Gmax2:%lf) = %lf \n", Gmax1, Gmax2, Gmax1 + Gmax2);
    return Gmax1 + Gmax2 < solver->eps;
}

// 第二类的svm Betai 与 Betaj 的选择器。
int select_working_nu_svm(solver_t* solver, int* out_i, int* out_j)
{
    size_t len_alpha = UA_length(&solver->alpha);
    vfloat_t* G_ptr  = UA_data_ptr(&solver->G);
    vfloat_t* Y_ptr  = UA_data_ptr(solver->Y);
    
    double Gmax1     = -DBL_MAX;
    double Gmax1_idx = -1;

    double Gmax2     = -DBL_MAX;
    double Gmax2_idx = -1;

    double Gmax3     = -DBL_MAX;
    double Gmax3_idx = -1;

    double Gmax4     = -DBL_MAX;
    double Gmax4_idx = -1;

    int i;

    for ( i=0; i<len_alpha; ++i ) {

        double t;
        if (Y_ptr[i] > 0) { 

            if ( !solver_is_upper_bound(solver, i) && ( t = -G_ptr[i] > Gmax1) ) {
                
                Gmax1 = t;
                Gmax1_idx = i;

            }

            if ( !solver_is_lower_bound(solver, i) && (t = -G_ptr[i]) > Gmax2 ) {

                Gmax2 = t;
                Gmax2_idx = i;

            }

        } else {


            if ( !solver_is_upper_bound(solver, i) && (t = -G_ptr[i]) > Gmax3 ) {

                Gmax3 = t;
                Gmax3_idx = i;
            }

            if ( !solver_is_lower_bound(solver, i) && (t = -G_ptr[i]) > Gmax4 ) {

                Gmax4 = t;
                Gmax4_idx = i;

            }

        }
    }

    if ( SVM_MAX(Gmax1 + Gmax2, Gmax3 + Gmax4) < solver->eps) {
        // 条件终止了，没有可以选择的 Beta_i 与 Beta_j
        return 1;
    }

    if ( Gmax1 + Gmax2 > Gmax3 + Gmax4)
    {
        *out_i = Gmax1_idx;
        *out_j = Gmax2_idx;
    } else {
        *out_i = Gmax3_idx;
        *out_j = Gmax4_idx;
    }
    return 0;
}

// 计算偏移量
int calc_rho(solver_t* solver, double* rho, double* r){

    double ub = DBL_MAX;
    double lb = -DBL_MAX;
    double sum_yG = 0.f;
    
    int i, nr_free = 0;
    vfloat_t* Y_ptr = UA_data_ptr(solver->Y);
    vfloat_t* G_ptr = UA_data_ptr(&solver->G);
    size_t len_alpha = UA_length(&solver->alpha);

    for (i=0; i<len_alpha; ++i) {

        double yG = Y_ptr[i] * G_ptr[i];

        if ( solver_is_lower_bound(solver, i) ) { // Beta_i == 0

            if ( Y_ptr[i] > 0 ) 
                ub = SVM_MIN(ub, yG);
            else 
                lb = SVM_MAX(lb, yG);

        } else if ( solver_is_upper_bound(solver, i) ) { // Beta_i == C

            if ( Y_ptr[i] < 0 ) 
                ub = SVM_MIN(ub, yG);
            else 
                lb = SVM_MAX(lb, yG);
        } else {
            ++nr_free;
            sum_yG += yG;
        }
    }

    *rho = nr_free > 0 ? sum_yG / nr_free : (ub + lb) * 0.5;
    *r = 0;
}

// 计算偏移量
int calc_rho_nu_sum(solver_t* solver, double* rho, double* r)
{
    int nr_free1 = 0, nr_free2 = 0;
    double ub1 = DBL_MAX, ub2 = DBL_MAX;
    double lb1 = -DBL_MAX, lb2 = -DBL_MAX;
    double sum_yG1 = 0.f, sum_yG2 = 0.f;
    double r1, r2;
    size_t len_alpha = UA_length(&solver->alpha);
    vfloat_t* G_ptr  = UA_data_ptr(&solver->G);
    vfloat_t* Y_ptr  = UA_data_ptr(solver->Y);

    int i;
    for (i=0; i<len_alpha; ++i) {

        double G_i = G_ptr[i];
        
        if ( Y_ptr[i] > 0 ) {
            
            if ( solver_is_lower_bound(solver, i) ) {
                ub1 = SVM_MIN( ub1, G_i );
            } else if ( solver_is_upper_bound(solver, i) ) {
                lb1 = SVM_MAX( lb1, G_i);
            } else {
                ++nr_free1;
                sum_yG1 += G_i;
            }

        } else {

            if ( solver_is_lower_bound(solver, i) ) {

                ub2 = SVM_MIN( ub2, G_i );

            } else if ( solver_is_upper_bound(solver, i) ) {

                lb2 = SVM_MAX( lb2, G_i );

            } else {
                ++nr_free2;
                sum_yG2 += G_i;
            }

        }

    }

    r1 = nr_free1 > 0 ? sum_yG1 / nr_free1 : ( ub1 + lb1 ) * 0.5f;
    r2 = nr_free2 > 0 ? sum_yG2 / nr_free2 : ( ub2 + lb2 ) * 0.5f;

    *rho = (r1 - r2) * 0.5f;
    *r   = (r1 + r2) * 0.5f;

}

// 以下四种核函数的实现。
double calc_linear(solver_t* solver, int i, int j)
{   
    int len_Xc = UA_shape_axis(solver->X, 1);
    vfloat_t (*X_r)[len_Xc] = UA_data_ptr(solver->X);
    return kernel_function_calculate_liner(X_r[i], X_r[j], len_Xc, 1, 0);
}

// 多项式核函数
double calc_poly(solver_t* solver, int i, int j)
{
    int len_Xc = UA_shape_axis(solver->X, 1);
    vfloat_t (*X_r)[len_Xc] = UA_data_ptr(solver->X);
    return kernel_function_calculate_poly(X_r[i], X_r[j], len_Xc, solver->kernel_param.degree);
}

// 计算 sigmoid 核函数
double calc_sigmoid(solver_t* solver, int i, int j)
{
    int len_Xc = UA_shape_axis(solver->X, 1);
    vfloat_t (*X_r)[len_Xc] = UA_data_ptr(solver->X);
    return kernel_function_calculate_sigmoid(X_r[i], X_r[j], len_Xc, solver->kernel_param.gammer, solver->kernel_param.coef0);
}

// 计算 高斯 核函数
double calc_rbf(solver_t* solver, int i, int j)
{
    size_t len_Xc = UA_shape_axis(solver->X, 1);
    vfloat_t (*X_r)[len_Xc] = UA_data_ptr(solver->X);
    return kernel_function_calculate_rbf(X_r[i], X_r[j], len_Xc, solver->kernel_param.gammer);
}

int build_c_svc_Q (solver_t* solver) 
{
    size_t len_alpha = UA_length(&solver->alpha);    
    vfloat_t (*Q_r)[len_alpha] = UA_data_ptr(&solver->Q);

    vfloat_t* Y_ptr = UA_data_ptr(solver->Y);

    for (size_t i=0; i<len_alpha; ++i) {
        for (size_t j=0; j<len_alpha; ++j) {
            Q_r[i][j] = Y_ptr[i] * Y_ptr[j] * solver->kernel(solver, i, j);
        }
    }
    return 0;
}

int build_nu_svc_Q (solver_t* solver) {
    return build_c_svc_Q(solver);
}   

int build_one_class_Q (solver_t* solver) {
    size_t len_alpha = UA_length(&solver->alpha);
    vfloat_t (*Q_r)[len_alpha] = UA_data_ptr(&solver->Q);

    for (size_t i=0; i<len_alpha; ++i) {
        for (size_t j=0; j<len_alpha; ++j) {
            Q_r[i][j] = solver->kernel(solver, i, j);
        }
    }
    return 0;
}

int build_e_svr_Q(solver_t* solver) {
    size_t len_alpha = UA_length(&solver->alpha);
    vfloat_t (*Q_r)[len_alpha] = UA_data_ptr(&solver->Q);

    float Z[len_alpha];

    for (int k = 0; k < len_alpha; k++) {
        if (k < len_alpha / 2) {
            k = 1.f;
        } else {
            k = -1.f;
        }
    }

    for (size_t i=0; i<len_alpha;  ++i) {
        for (size_t j=0; j<len_alpha; ++j) {
            Q_r[i][j] = Z[i] * Z[j] * solver->kernel(solver, i, j);
        }
    }

    return 0;
}

int build_nu_svr_Q(solver_t* solver)
{
    return build_e_svr_Q(solver);
}
