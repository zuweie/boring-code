/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-06-15 16:10:10
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-09-14 14:12:14
 * @FilePath: /boring-code/src/statistical_learning/svm.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

#include <float.h>
#include "svm.h"

static int __initialze_c_svc( matrix2_t* Beta, matrix2_t* Q, matrix2_t* P, matrix2_t* G, matrix2_t* _X, matrix2_t* _Y, svm_type_t svm_type, Kernel_func K, k_params_t* k_params)
{

    int R = _Y->rows;

    // 初始化 Beta
    // reshape Beta.
    Mat2_reshape(Beta, R, 1);
    // 对于 C-SVC 算法 Beta 的初始化为 0 即可。
    Mat2_fill(Beta, 0.f);

    // 初始化 Q
    Mat2_reshape(Q, R, R);

    MAT2_POOL_PTR(Q, Q_ptr);
    MAT2_POOL_PTR(_X, X_ptr);
        
    for (int i=0; i<Q->rows; ++i) {
        for (int j=0; j<Q->cols; ++j) {
            // 因为是对称矩阵，此处直接计算一半即可，剩下一半直接拷贝
            if (j>=i)
                Q_ptr[i][j] = _Y->pool[i] * _Y->pool[j] * K(X_ptr[i], X_ptr[j], _X->cols, k_params->p1, k_params->p2, k_params->p3);
            else 
                Q_ptr[i][j] = Q_ptr[j][i];
        }
    }

    // 初始化 P
    Mat2_reshape(P, R, 1);
    Mat2_fill(P, -1.f);

    // 初始化 G，G 就 f(Beta) 对 Beta 导数梯度向量。
    // Deta f(Beta) = Q dot Beta + P ,而在上行代码中 Beta 已经被赋值为 0，则 Q dot Beta 为 0 向量，所有 Deta f(Beta) 直接等于 P
    Mat2_cpy(G, P);

    return 0;
}

/**
 * @brief 
 * 
 */
static int __initialize( matrix2_t* Beta, matrix2_t* Q, matrix2_t* P, matrix2_t* G, \
                        matrix2_t* _X, matrix2_t* _Y, svm_type_t svm_type,          \
                        Kernel_func K, k_params_t* k_params)
{
    if (svm_type == c_svc) {
        
        return __initialze_c_svc(Beta, Q, P, G, _X, _Y, svm_type, K, k_params);

    }
    // else if (...) {...} 
    return 0;

}

// 第一类的 working set 选取
/**
 * @brief 在对 Beta 导数中寻找最大的两个导数的 index，并且返回这两个导数的和是否已经低于阀值，低于阀值，整个大循环即可停止。
 * 
 * @param G 这个事 公式 3-148 对 Beta 的导数
 * @param _Y 
 * @param svm_params 
 * @param out_i 
 * @param out_j 
 * @return int 
 */
static int __working_set_1(matrix2_t* G, matrix2_t* Beta, matrix2_t* _Y, svm_params_t* svm_params, int* out_i, int* out_j)
{
    vfloat_t Gmax_i = -FLT_MAX;
    vfloat_t Gmax_j = -FLT_MAX;
    int out_max_i = -1;
    int out_max_j = -1;
    
    for (int i=0; i<G->rows; ++i) {

        // 以下算法对应书中 3-166 的公式。
        if (_Y->pool[i] > 0) {

            // 分两种情况，第一种是 y == 1。
            if (Beta->pool[i] < svm_params->C[_Y->pool[i] > 0] && -G->pool[i] > Gmax_i) {
                Gmax_i = -G->pool[i];
                out_max_i = i;
            }

            if (Beta->pool[i] > 0 && G->pool[i] > Gmax_j ) {
                Gmax_j = G->pool[i];
                out_max_j = i;
            }

        } else {

            // 分两种情况，第一种是 y == -1。
            if (Beta->pool[i] > 0 && G->pool[i] > Gmax_i) {
                Gmax_i = G->pool[i];
                out_max_i = i;
            }

            if (Beta->pool[i] < svm_params->C[_Y->pool[i] > 0] && -G->pool[i] > Gmax_j) {
                Gmax_j = -G->pool[i];
                out_max_j = i;
            }
        }

    }

    *out_i = out_max_i;
    *out_j = out_max_j;
    return Gmax_i + Gmax_j < svm_params->eps;
}

// 第二类的 working set
static int __working_set_2(matrix2_t* G, matrix2_t* Beta, matrix2_t* _Y, svm_params_t* svm_params, int* out_i, int* out_j)
{
    vfloat_t Gmax_Ip = -FLT_MAX;
    vfloat_t Gmax_Jp = -FLT_MAX;
    vfloat_t Gmax_In = -FLT_MAX;
    vfloat_t Gmax_Jn = -FLT_MAX;

    int out_Ip = -1;
    int out_Jp = -1;
    int out_In = -1;
    int out_Jn = -1;

    // 以下算法对应书中 3-167 公式
    for (int i=0; i<G->rows; ++i) {

        if (_Y->pool[i] > 0) {
            // Z == 1 的情况

            if(Beta->pool[i] < svm_params->C[_Y->pool[i] > 0] && -G->pool[i] > Gmax_Ip) {

                Gmax_Ip = -G->pool[i];
                out_Ip  = i;
            }

            if (Beta->pool[i] > 0 && G->pool[i] > Gmax_Jp) {
                Gmax_Jp = G->pool[i];
                out_Jp = i;
            }

        } else {
            // Z == -1 的情况
            
            if (Beta->pool[i] < svm_params->C[_Y->pool[i] > 0] && -G->pool[i] > Gmax_In) {
                
                Gmax_In = -G->pool[i];
                out_In  = i;
            }

            if (Beta->pool[i] > 0 && G->pool[i] > Gmax_Jn) {
                Gmax_Jn = G->pool[i];
                out_Jn = i;
            }
        }

    }

    // 公式 3-169 决定 out_i = Ip out_j = Jp 还是 out_i = In out_j = Jn
    double delta_p = -G->pool[out_Ip] + G->pool[out_Jp];
    double delta_n = -G->pool[out_In] + G->pool[out_Jn];

    if (delta_p > delta_n ) {

        *out_i = out_Ip;
        *out_j = out_Jp;

    } else {
        *out_i = out_In;
        *out_j = out_Jn;
    }

    // 公式 3-171, 返回大循环是否结束的条件。
    return (delta_p > delta_n ? delta_p : delta_n) > svm_params->eps;
}


static int solve_generic(matrix2_t* G, matrix2_t* Beta, matrix2_t* Q, matrix2_t* _Y, svm_params_t* svm_params, Selct_working_set_func S)  
{
    double old_beta_i, old_beta_j, beta_i, beta_j, delta_beta_i, delta_beta_j;
    int out_i, out_j;
    int iter = 0;
    int C_i, C_j;

    MAT2_POOL_PTR(Q, Q_ptr);
    
    // 找到要做导数两个两个 beta_i、beta_j

    while (!S(G, Beta, _Y, svm_params, &out_i, &out_j) && ++iter > svm_params->max_iter) {
        
        C_i = svm_params->C[_Y->pool[out_i] > 0];
        C_j = svm_params->C[_Y->pool[out_j] > 0];

        beta_i = old_beta_i = Beta->pool[out_i];
        beta_j = old_beta_j = Beta->pool[out_j];


        if (_Y->pool[out_i] == _Y->pool[out_j]) {
            // Zi == Zj 的情况
            double qv    = Q_ptr[out_i][out_i] - Q_ptr[out_j][out_j] - 2 * Q_ptr[out_i][out_j];
            double delta = G->pool[out_i] - G->pool[out_j] /  (fabs(qv) > 0 ? qv : FLT_EPSILON); // 这里房子 QV 出现零的情况，若出现0，则除以一个很小的数字
            double sum = beta_i + beta_j;




        } else {
            // Zi ！= Zj 的情况

        }

    }
}


/**
 * @brief SVM 训练过程，使用广义 SMO 计算支持向量。
 * 
 * @param train_data 
 * @param train_label 
 * @param svm_type 
 * @param kernel_func 
 * @param alpha 
 * @return int 
 */
int svm_train(matrix2_t* train_data, matrix2_t* train_label, svm_type_t svm_type, svm_params_t* svm_params, Kernel_func K, k_params_t* k_params, matrix2_t* alpha)
{
    // 通用的计算公式：f(Beta) = 1/2 BetaT dot Q bate + pT dot Beta (3-148)
    // Deta f(Beta) = Q dot Beta + P (3-149)
    // 在 Q 为对称矩阵的时候，以上的 微分公式才能成立。

    // TODO: 1 根据 SVM 类型，初始化 Beta、Q、P 的矩阵或者向量。
    matrix2_t* Beta  = Mat2_create(1,1);
    matrix2_t* Q     = Mat2_create(1,1);
    matrix2_t* P     = Mat2_create(1,1);
    matrix2_t* G     = Mat2_create(1,1);
    
    // 初始化 Beta、Q、P
    __initialize(Beta, Q, P, G, train_data, train_label, svm_type, K, k_params);

    Selct_working_set_func S = \
        ((svm_type == c_svc || svm_type ==  epsilon_svr || svm_type == one_class) ? __working_set_1 : __working_set_2) ;

    // TODO: 2 根据 SVM 类型，选出要优化的两个 Beta, 然后计算两个 beta 的 detal 值，直到那个什么 两个 beta 的和小于阀值
    int out_i;
    int out_j;
    int iter = 0;
    while ( !S(G, Beta, train_label, svm_params, &out_i, &out_j) || iter++ > svm_params->max_iter) {

    }
    
    
    

    // TODO: 3 根据 SVM 类型，算出 Beta 的差分值，然后更新公式 (1.2)

    // 释放内存
    Mat2_destroy(Beta);
    Mat2_destroy(Q);
    Mat2_destroy(P);
    return 0;
}

int svm_predict(matrix2_t* _Input, matrix2_t* alpha, vfloat_t* predict);