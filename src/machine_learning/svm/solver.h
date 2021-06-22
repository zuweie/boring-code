/*
 * @Author: your name
 * @Date: 2021-06-03 13:43:31
 * @LastEditTime: 2021-06-22 14:47:34
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/machine_learning/svm/solver.h
 */

#ifndef _SVM_SOLVER_H_
#define _SVM_SOLVER_H_
#include "vtype/vfloat_type.h"

#define SVM_MAX(x, y) (((x) > (y)) ? (x) : (y))
#define SVM_MIN(x, y) SVM_MAX(y, x)

typedef enum { C_SVC, NU_SVC, ONE_CLASS, EPSILON_SVR, NU_SVR } SVM_type; /* SVM type */
typedef enum { LINEAR, POLY, BRF, SIGMOID } SVM_kernel; /* kernel function type */

typedef struct _u_array u_array_t;
typedef struct _solver solver_t;

struct _solver {
    int (*select_working_set)(int* out_i, int* out_j, int*);
    int (*calc_rho)(vfloat_t, vfloat_t);
    double (*kernel)(vfloat_t*, vfloat_t*);
    int (*build_Q)(solver_t *, u_array_t*);


    u_array_t* Y;
    u_array_t* X;

    u_array_t alpha;
    u_array_t G; 
    u_array_t C;
    u_array_t Q;
    u_array_t P;
    
    int max_iter; // select working set 最大的尝试次数。
    double eps;

    struct kennel_param {
        double gammer; // 多项式
        double coef0;  // 多项式
        double degree; // 多项式的
    } kernel_param;
    
};

int Solver_initialize(     \
        solver_t* solver,  \ 
        SVM_type svm_type, \
        SVM_kernel kerenl, \
        u_array_t* _X, u_array_t* _Y,     \ 
        vfloat_t _C, vfloat_t _gammer,    \
        vfloat_t _coef, vfloat_t _degree, \
        double eps,  \
        int max_iter \
);
int Solver_finalize(solver_t* solver);

// 第一类的svm Betai 与 Betaj 的选择器。
int select_working_set(solver_t* solver);

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
int kernel_calc_brf();

// 生成 c_svc Q 矩阵 
build_c_svc_Q(solver_t* solver, u_array_t* Q);
build_nu_svc_Q(solver_t* solver, u_array_t* Q);
build_one_class_Q(solver_t* solver, u_array_t* Q);
build_e_svr_Q(solver_t* solver, u_array_t* Q);
build_nu_svr_Q(solver_t* solver, u_array_t* Q);

#endif