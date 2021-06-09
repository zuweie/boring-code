/*
 * @Author: your name
 * @Date: 2021-06-03 13:59:00
 * @LastEditTime: 2021-06-10 07:07:10
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/machine_learning/svm/solver.c
 */
#include <math.h>
#include "solver.h"
#include "ultra_array/ultra_array.h"

int Solver_constructor(solver_t* solver, size_t l, int (*select_working_set)(int, int), int (*calculate_rho)(vfloat_t, vfloat_t), int (*kernel_func)(vfloat_t, vfloat_t))
{
    solver->C = NULL;
    solver->calculate_rho = calculate_rho;
    solver->kernel_func = kernel_func;
    solver->select_working_set = select_working_set;
    return 0;
}

int Solver_desctructor(solver_t* solver)
{
    if (solver->C) free(solver->C);
    if (solver->alpha_status) (solver->alpha_status);
    return 0;
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
    vfloat_t Y_ptr   = UA_data_ptr(solver->Y);
    vfloat_t G_ptr   = UA_data_ptr(solver->G);
    size_t len_alpha = UA_length(solver->alpha);

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
    size_t len_alpha = UA_length(solver->alpha);
    vfloat_t* G_ptr  = UA_data_ptr(solver->G);
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


}

// 核函数
int kernel_calc_linear();

// 多项式核函数
int kernel_calc_poly();

// 计算 sigmoid 核函数
int kernel_calc_sigmoid();

// 计算 高斯 核函数
int kernel_calc_rbf();