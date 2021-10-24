/*
 * @Author: your name
 * @Date: 2021-06-03 13:43:31
 * @LastEditTime: 2021-10-24 09:40:41
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/machine_learning/svm/solver.h
 */


#ifndef __SVM_SOLVER_H__
#define __SVM_SOLVER_H__
#include "vtype/vfloat_type.h"
#include "ultra_array/ultra_array.h"

#define SVM_MAX(x, y) (((x) > (y)) ? (x) : (y))
#define SVM_MIN(x, y) (((x) < (y)) ? (x) : (y))

typedef enum { C_SVC, NU_SVC, ONE_CLASS, EPSILON_SVR, NU_SVR } SVM_type; /* SVM type */
typedef enum { LINEAR, POLY, RBF, SIGMOID } SVM_kernel; /* kernel function type */



typedef struct _kernel_param {
    double gammer; // 多项式
    double coef0;  // 多项式
    double degree; // 多项式的
} kernel_param_t;

typedef struct _solver solver_t;
struct _solver {
    
    int (*select_working_set)(solver_t* solver, int* out_i, int* out_j);
    int (*calc_rho)(solver_t* solver, double*, double*);
    double (*kernel)(solver_t* solver, int i, int j);
    int (*build_Q)(solver_t* solver);


    u_array_t* Y;
    u_array_t* X;
    u_array_t* C;

    u_array_t alpha;
    u_array_t G; 
    u_array_t Q;
    u_array_t P;
    
    int max_iter; // select working set 最大的尝试次数。
    double eps;   // select working set 精度要求。
    double rho;
    double r;
    kernel_param_t kernel_param;
};


int solver_initialize(     \
        solver_t* solver,  \ 
        SVM_type svm_type, \
        SVM_kernel kerenl, \
        double _gammer,  \
        double _coef,    \
        double _degree,  \
        double eps,      \
        int max_iter     \
);

int solver_finalize(solver_t* solver);

int solver_set_calculating_dataset(solver_t* solver, u_array_t* X, u_array_t* Y, u_array_t* _C);

int solver_is_lower_bound(solver_t* solver, int i);
int solver_is_upper_bound(solver_t* solver, int j);

// 第一类的svm Betai 与 Betaj 的选择器。
int select_working_set(solver_t* solver, int* out_i, int* out_j);

// 第二类的svm Betai 与 Betaj 的选择器。
int select_working_nu_svm(solver_t* solver, int* out_i, int* out_j);

// 计算偏移量
int calc_rho(solver_t* solver, double* rho, double* r);

// 计算偏移量
int calc_rho_nu_sum(solver_t* solver, double* rho, double* r);

// 核函数
double calc_linear(solver_t* solver, int i, int j);

// 多项式核函数
double calc_poly(solver_t* solver, int i, int j);

// 计算 sigmoid 核函数
double calc_sigmoid(solver_t* solver, int i, int j);

// 计算 高斯 核函数
double calc_rbf(solver_t* solver, int i, int j);

// 生成 c_svc Q 矩阵 
int build_c_svc_Q(solver_t* solver);
int build_nu_svc_Q(solver_t* solver);
int build_one_class_Q(solver_t* solver);
int build_e_svr_Q(solver_t* solver);
int build_nu_svr_Q(solver_t* solver);

#endif