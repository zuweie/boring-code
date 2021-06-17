/*
 * @Author: your name
 * @Date: 2021-06-03 13:43:31
 * @LastEditTime: 2021-06-16 17:35:07
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/machine_learning/svm/solver.h
 */

#ifndef _SVM_SOLVER_H_
#define _SVM_SOLVER_H_
#include "vtype/vfloat_type.h"

#define MAX(x, y) (((x) > (y)) ? (x) : (y))
#define MIN(x, y) MAX(y, x)

typedef struct _u_array u_array_t;
typedef struct _solver {
    
    int (*select_working_set)(int* out_i, int* out_j, int*);
    int (*calc_rho)(vfloat_t, vfloat_t);
    int (*kernel_func)(vfloat_t*, vfloat_t*);
    u_array_t* alpha;
    u_array_t* G;
    u_array_t* Y;
    u_array_t* X;
    int* C;

    struct kennel_calc_param {
        double gammer; // 多项式
        double coef0;  // 多项式
        double degree; // 多项式的
    } calc_param;
    
} solver_t;

int Solver_constructor();
int Solver_desctructor();

// 第一类的svm Betai 与 Betaj 的选择器。
int select_working_set();

// 第二类的svm Betai 与 Betaj 的选择器。
int select_working_nu_svm();

// 计算偏移量
int calc_rho(vfloat_t*, vfloat_t*);

// 计算偏移量
int calc_rho_nu_sum(vfloat_t*, vfloat_t*);

// 核函数
int kernel_calc_linear();

// 多项式核函数
int kernel_calc_poly();

// 计算 sigmoid 核函数
int kernel_calc_sigmoid();

// 计算 高斯 核函数
int kernel_calc_rbf();


#endif