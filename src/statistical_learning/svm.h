#ifndef __SVM_H__
#define __SVM_H__

#include "matrix2.h"

typedef enum {

    c_svc,
    nu_svc,
    one_class,
    epsilon_svr,
    nu_svr
}svm_type_t;

typedef struct {

    double p1;
    double p2;
    double p3;

} k_params_t;

typedef struct {

    // 两个类的惩罚参数，正常来说两个类的惩罚参数应该是一样的。
    // C[0] 为 -1 类，C[1] 为 1 类。
    double C[2];

    // Beta 的导数计算终止条件。
    double eps;

    // 最大的迭代次数。
    int max_iter;

} svm_params_t;

typedef vfloat_t (*Kernel_func)(vfloat_t*, vfloat_t*, int, double, double, double);
typedef int (*Selct_working_set_func)(matrix2_t*, matrix2_t*, matrix2_t*, svm_params_t*, int*, int* );
typedef int (*Calculate_rho)(matrix2_t*, matrix2_t*, double*, double*);
int svm_train(matrix2_t* train_data, matrix2_t* train_label, svm_type_t svm_type, double esp, Kernel_func K, k_params_t* k_params, matrix2_t* alpha);
int svm_predict(matrix2_t* _Input, matrix2_t* alpha, vfloat_t* predict);

#endif