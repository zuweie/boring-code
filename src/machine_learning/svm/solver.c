/*
 * @Author: your name
 * @Date: 2021-06-03 13:59:00
 * @LastEditTime: 2021-06-22 13:28:32
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/machine_learning/svm/solver.c
 */
#include <math.h>
#include "ultra_array/ultra_array.h"
#include "ultra_array/ultra_router.h"
#include "solver.h"


int Solver_initialize(
    
        solver_t* solver, 
        SVM_type svm_type, 
        SVM_kernel kerenl, 
        u_array_t* _X, u_array_t* _Y, 
        vfloat_t _C, vfloat_t _gammer, 
        vfloat_t _coef, vfloat_t _degree,
        double eps,
        int max_iter

)
{
    // a big big initialize
    solver->X = _X;
    solver->Y = _Y;
    
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
        solver->kernel = &kernel_calc_linear;
        break;
    case POLY:
        solver->kernel = &kernel_calc_poly;
        break;
    case BRF:
        solver->kernel = &kernel_calc_brf;
        break;
    case SIGMOID:
        solver->kernel = &kernel_calc_sigmoid;
        break;
    default:
        solver->kernel = NULL:
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
        solver->build_q = &build_nu_svr_Q;
        break;
    default:
        solver->build_Q = NULL;
        break;
    }

    // build the G, alpha, C. Q。 这些都需要回收内存的
    size_t len_Y = UA_length(_Y);
    
    // 1 build alpha
    solver->alpha = _UArray1d(len_y);
    // TODO : init the alpha

    // 2 build the Q
    solver->Q = _UArray2d(len_Y, len_Y);

    // 3 build the G
    solver->G = UA_empty_like(&solver->Q);

    // 4 build the C
    solver->C = _UArray1d(len_Y);

    // 5 build P
    solver->P = _UArray1d(len_Y);
    
    // max_iter;
    solver->max_iter = max_iter;

    // eps
    solver->eps = eps;
}

int Solver_finalize(solver_t* solver)
{
    UArray_(&solver->G);
    UArray_(&solver->alpha);
    UArray_(&solver->Q);
    UArray_(&solver->C);
    UArray_(&solver->P);
}

int Solver_is_lower_bound(solver_t* solver, int i) 
{
    vfloat_t* alpha_ptr = UA_data_ptr(solver->alpha);
    return alpha_ptr[i] <= solver->C[i];
}

int Solver_is_upper_bound(solver_t* solver, int i)
{
    vfloat_t* alpha_ptr = UA_data_ptr(solver->alpha);
    return alpha_ptr[i] >= solver->C[i];
}

// 第一类的svm Betai 与 Betaj 的选择器。
int select_working_set(solver_t* solver, int* out_i; int* out_j)
{
    vfloat_t Y_ptr   = UA_data_ptr(&solver->Y);
    vfloat_t G_ptr   = UA_data_ptr(&solver->G);
    size_t len_alpha = UA_length(&solver->alpha);

    double Gmax1  = -DBL_MAX;
    int Gmax1_idx = -1;

    double Gmax2  = -DBL_max;
    int Gmax2_idx = -1;
    int i;

    for (i=0; i<len_alpha; ++i) {
        double t;
        if (Y_ptr[i] > 0) {
            
            if (! Solver_is_upper_bound(solver, i) && (t = -solver->G_ptr[i]) > Gmax1) {

                Gmax1 = t; // 更新最大值
                Gmax1_idx = i;
            }
            if ( ! Solver_is_lower_bound(solver, i) && (t = -solver->G_ptr[i]) > Gmax2) {

                Gmax2 = t; // 更新最大值
                Gmax2_idx = i;
            }

        } else {
            if ( ! Solver_is_upper_bound(solver, i) && (t = -solver->G_ptr[i]) > Gmax2 ) {
                Gmax2 = t;
                Gmax2_idx = i;
            }
            if ( !Solver_is_lower_bound(solver, i) && (t = -solver->G_ptr[i]) > Gmax1 ) {
                Gmax1 = t;
                Gmax1_idx = i;
            }
        }
    }
    
    *out_i = Gmax1_idx;
    *out_j = Gmax1_idx;

    return Gmax1 + Gmax2 < solver->eps;
}

// 第二类的svm Betai 与 Betaj 的选择器。
int select_working_nu_svm(solver_t* solver, int* out_i, int* out_j)
{
    size_t len_alpha = UA_length(&solver->alpha);
    vfloat_t* G_ptr  = UA_data_ptr(&solver->G);
    vfloat_t* Y_ptr  = UA_data_ptr(&solver->Y);
    
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

            if ( !Solver_is_upper_bound(solver, i) && ( t = -G_ptr[i] > Gmax1) ) {
                
                Gmax1 = t;
                Gmax1_idx = i;

            }

            if ( !Solver_is_lower_bound(solver, i) && (t = -G_ptr[i]) > Gmax2 ) {

                Gmax2 = t;
                Gmax2_idx = i;

            }

        } else {


            if ( !Solver_is_upper_bound(solver, i) && (t = -G_ptr[i]) > Gmax3 ) {

                Gmax3 = t;
                Gmax3_idx = i;
            }

            if ( !Solver_is_lower_bound(solver, i) && (t = -G_ptr[i]) > Gmax4 ) {

                Gmax4 = t;
                Gmax4_idx = i;

            }

        }
    }

    if ( MAX(Gmax1 + Gmax2, Gmax3 + Gmax4) < solver->eps) {
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
    vfloat_t* G_ptr = UA_data_ptr(solver->G);
    size_t len_alpha = UA_length(solver->alpha);
    for (i=0; i<len_alpha; ++i) {

        double yG = y[i] * G_ptr[i];

        if ( Solver_is_lower_bound(solver, i) ) { // Beta_i == 0

            if ( Y_ptr[i] > 0 ) 
                ub = MIN(ub, yG);
            else 
                lb = MAX(lb, yG);

        } else if ( Solver_is_upper_bound(solver, i) ) { // Beta_i == C

            if ( Y_ptr <0 ) 
                ub = MIN(ub, yG);
            else 
                lb = MAX(lb, yG);
        } else {
            ++nr_free;
            sum_yG += yG;
        }
    }

    *rho = nr_free > 0 ? sum_free / nr_free : (ub + lb) * 0.5;
    *r = 0;
}

// 计算偏移量
int calc_rho_nu_sum(Solver_t* solver, double* rho, double* r)
{
    int nr_free1 = 0, nr_free2 = 0;
    double ub1 = DBL_MAX, ub2 = DBL_MAX;
    double lb1 = -DBL_MAX, lb2 = -DBL_MAX;
    double sum_yG1 = 0.f, sum_yG2 = 0.f;
    double r1, r2;
    size_t len_alpha = UA_length(solver->alpha);
    vfloat_t* G_ptr  = UA_data_ptr(solver->G);
    vfloat_t* Y_ptr  = UA_data_ptr(solver->Y);

    int i;
    for (i=0; i<len_alpha; ++i) {

        double G_i = G_ptr[i];
        
        if ( Y_ptr[i] > 0 ) {
            
            if ( Solver_is_lower_bound(solver, i) ) {
                ub1 = MIN( ub1, G_i );
            } else if ( Solver_is_upper_boundsolver, i) ) {
                lb1 = MAX( lb1, G_i);
            } else {
                ++nr_free1;
                sum_yG1 += G_i;
            }

        } else {

            if ( Solver_is_lower_bound(solver, i) ) {

                ub2 = MIN( ub2, G_i );

            } else if ( Solver_is_upper_bound(solver, i) ) {
                lb2 = MAX( lb2, G_i );
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

// 核函数
double Kernel_calc_base_linear(Solver_t* solver, int i, int j, double _alpha, double _beta) 
{
    size_t len_x = UA_shape_axis(solver->X, 1);

    vfloat_t (*X_r)[len_x] = UA_data_ptr(solver->X);

    double dot = 0.f;
    for (size_t k=0; k<len_x; ++k) {
        dot += X_r[i][k] * X_r[j][k];
    }
    return _alpha * dot + _beta;
}

double kernel_calc_linear(Solver_t* solver, int i, int j)
{
    return Kernel_calc_base_linear(solver, i, j, 1, 0);
}

// 多项式核函数
double kernel_calc_poly(Solver_t* solver, int i, int j)
{
    double linear_dot = Kernel_calc_base_linear(solver, i, j, solver->calc_param.gammer, solver->calc_param.coef0);
    return pow(linear_dot, solver->calc_param.degree);
}

// 计算 sigmoid 核函数
double kernel_calc_sigmoid(Solver_t* solver, int i, int j)
{
    vfloat_t t = Kernel_calc_base_linear( solver, i, j, -2*solver->calc_param.gammer, -2*solver->calc_param.coef0 );
    double e   = exp(-fabs(t));

    return t > 0 ? ((1.f - e) / (1.f + e)) : ((e - 1.f) / (e + 1.f));
}

// 计算 高斯 核函数
int kernel_calc_brf(Solver_t* solver, int i, int j)
{
    size_t len_r = UA_shape_axis(solver->X, 1);
    vfloat_t (*X_r)[len_r] = UA_data_ptr(solver->X);

    double dis_q = 0.f;

    for (size_t k=0; k<len_r; ++k) {
        dis_q += (X_r[i][k] - X_r[j][k]) * (X_r[i][k] - X_r[j][k]);
    }
    return exp(dis_q * solver->calc_param.gammer);
}

int build_c_svc_Q (Solver_t* solver, u_array_t* Q) 
{
    size_t len_alpha = UA_length(solver->alpha);    
    vfloat_t (*Q_r)[len_alpha] = UA_data_ptr(Q);

    vfloat_t* Y_ptr = UA_data_ptr(solver->Y);

    for (size_t i=0; i<len_alpha; ++i) {
        for (size_t j=0; j<len_alpha; ++j) {

            Q_r[i][j] = Y_ptr[i] * Y_ptr[j] * solver->kernel_func(solver, i, j);

        }
    }
    return 0;
}

int build_nu_svc_Q (Solver_t* solver, u_array_t* Q) {
    return build_c_svc_Q(solver, Q);
}   

int build_one_class_Q (Solver_t* solver, u_array_t* Q) {
    size_t len_alpha = UA_length(solver->alpha);
    vfloat_t (*Q_r)[len_alpha] = UA_data_ptr(Q);

    for (size_t i=0; i<len_alpha; ++i) {
        for (size_t j=0; j<len_alpha; ++j) {
            Q_r[i][j] = solver->kernel_func(solver, i, j);
        }
    }
    return 0;
}

int build_e_svr_Q(Solver_t* solver, u_array_t* Q) {
    size_t len_alpha = UA_length(solver->alpha);
    vfloat_t (*Q_r)[len_alpha] = UA_data_ptr(Q);

    float Z[len_alpha] = {-1.f};

    for (int k = 0; K < (len_alpha / 2); k++) {
        Z[k] = 1;
    }

    for (size_t i=0; i<len_alpha;  ++i) {
        for (size_t j=0; j<len_alpha; ++j) {

            Q_r[i][j] = Z[i] * Z[j] * solver->kernel_func(solver, i, j);

        }
    }

    return 0;
}

int build_nu_svr_Q(Solver_t* solver, u_array_t* Q)
{
    return build_e_svr_Q(solver, Q);
}
