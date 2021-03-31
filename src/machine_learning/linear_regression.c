/*
 * @Author: your name
 * @Date: 2021-03-22 15:04:24
 * @LastEditTime: 2021-03-31 21:08:52
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/machine_learning/linear_regression.c
 */
#include <stdlib.h>
#include "ultra_array/ultra_array.h"
#include "matrix/DenseMatrix.h"

static double 
__vet_dot_vet(double X[], double Y[], size_t n) 
{
    double v = 0.f;
    for (size_t i=0; i<n; ++i) {
        v += X[i] * Y[i];
    }
    return v;
}

static double 
__vet_sum(double X[], size_t n) 
{
    double v = 0.f;
    for (size_t i=0; i<n; ++i) {

        v += X[i];

    }
    return v;
}

int Linear_Regression(u_array_t* X, u_array_t* Y,  double* W, double* b)
{
    // 把数据集做成线性方程。

    size_t Xc = UA_shape_axis(X, 1); // cols of X
    size_t Xr = UA_shape_axis(X, 0); // rows of X    
    double* Y_ptr = UA_data_ptr(Y);

    DenseMatrix* X_mat = DenseMatrix_wrap(Xr, Xc, UA_data_ptr(X));

    // number of Xi + b = Xc + 1;
    size_t coe_n = Xc + 1; 
    DenseMatrix* coe_mat = DenseMatrix_create(coe_n, coe_n);
    DenseMatrix_elem_ptr(coe_mat, coe_ptr);
    size_t i, j, k, l, m;
    mx_float_t _Y[coe_n];
    mx_float_t _X_col[Xr];
    mx_float_t _X_col_2[Xr];

    // 计算 dRh / dW 的参数矩阵。
    for (i=0; i<Xc; ++i) {
        
        // Y 的参数列表
        Matrix_get_col(X_mat, i, _X_col);
        _Y[i] = 2 * __vet_dot_vet(Y_ptr, _X_col, Xr);

        // w dot X 的参数列表 的参数列表
        for (j=0; j<Xr; ++j) {
            if (i == j) {

                coe[i][j] = 2 * __vet_dot_vet(_X_col, _X_col, Xr);

            } else {
                
                Matrix_get_col(X_mat, i, _X_col_2, Xr);
                coe[i][j] = 2 * __vet_dot_vet(_X_col, _X_col_2, Xr);

            }
        }

        // b 的参数列表
        coe[i][j] = 2 * __vet_sum(_X_col, Xr);
    }

    // 计算 dRh / db 的参数矩阵
    for(l=0; l<Xr; ++l) {
        
        // 最后一个 Y 的参数
        _Y[i] = 2 * __vet_sum(Y_ptr, Xr);

        // 最后一个 X 的参数
        Matrix_get_col(X_mat, l, _X_col);
        for (m=0; m<Xc; ++m) {
            coe[i][m] = 2 * __vet_sum(_X_col, Xr);
        }

        coe[i][m] = 2 * Xr;
    }
    
    // 线性方程求解
    DenseMatrix_solve(coe_mat, Y, coe_n);
    for (i=0; i<coe_n-1;++i) 
    {
        W[i] = Y[i];
    }
    *b = Y[coe_n-1];
    DenseMatrix_destroy(coe_mat);
    DenseMatrix_destroy(X_mat);
    return 0;
}

double Linear_Regression_predict(u_array_t* X, double* w, size_t wn, double b)
{
    return 0.f;
}

