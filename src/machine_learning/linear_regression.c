/*
 * @Author: your name
 * @Date: 2021-03-22 15:04:24
 * @LastEditTime: 2021-04-02 16:53:09
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/machine_learning/linear_regression.c
 */
#include <stdio.h>
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

// 线性回归算法
int Linear_Regression_solve(u_array_t* X, u_array_t* Y,  double* W, double* b)
{
    // 把数据集做成线性方程。
    int shape = UA_axisn(X);
    size_t Xc, Xr;
    if (shape == 2) {
        Xc = UA_shape_axis(X, 1);
        Xr = UA_shape_axis(X, 0);
    } else 
        return -1;

    if (UA_axisn(Y) != 1 && UA_shape_axis(Y, 0) != Xr) return -1;

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
        for (j=0; j<Xc; ++j) {
            if (i == j) {
                coe_ptr[i][j] = 2 * __vet_dot_vet(_X_col, _X_col, Xr);
            } else {
                Matrix_get_col(X_mat, i, _X_col_2);
                coe_ptr[i][j] = 2 * __vet_dot_vet(_X_col, _X_col_2, Xr);
            }
        }

        // b 的参数列表
        coe_ptr[i][j] = 2 * __vet_sum(_X_col, Xr);
    }

    /* 计算 dRh / db 的参数矩阵 */
    // 最后一个 Y 的参数
    _Y[i] = 2 * __vet_sum(Y_ptr, Xr);

    // 最后一个 W dot X 的参数
    for (l=0; l<Xc; ++l) {
        Matrix_get_col(X_mat, l, _X_col);
        coe_ptr[i][l] = 2 * __vet_sum(_X_col, Xr);
    }
    // 最后一个 b 的参数。
    coe_ptr[i][l] = 2 * Xr;

    // 线性方程求解
    DenseMatrix_solve(coe_mat, _Y, coe_n);
    for (i=0; i<coe_n-1;++i) {
        W[i] = _Y[i];
    }
    *b = _Y[coe_n-1];
    DenseMatrix_destroy(coe_mat);
    DenseMatrix_destroy(X_mat);
    return 0;
}

int Linear_Regression_pseudo_inverse(u_array_t* X, u_array_t* Y, double* W, double* b) 
{
    int shape = UA_shape_axisn(X);
    size_t Xr, Xc, i, j, k;
    if (shape != 2) {
        
        Xr = UA_shape_axis(X, 0);
        Xc = UA_shape_axis(X, 0);

    } else return -1;

    if (UA_axisn(Y) != 1 && (Y, 0) != Xr) return -1;

    DenseMatrix* X_mat = DenseMatrix_create(Xr, Xc+1);
    DenseMatrix* Y_mat = DenseMatrix_wrap(1, UA_shape_axis(Y, 0), UA_data_ptr(Y));

    DenseMatrix_elem_ptr(X_mat, X_ptr);
    double (*UX_ptr)[Xc] = UA_data_ptr(X);

    // 最前面填入一行
    for (i=0; i<Xr; ++i) {
        X_mat[i][0] = 1f;
        for (j=0; j<Xc; ++j) {
            X_mat[i][j+1] = UX_ptr[i][j];
        }
    }

    DenseMatrix* pinv_mat = DenseMatrix_create(Matrix_cols(X_mat), Matrix_cols(X_mat));
    DenseMatrix_pseudo_inverse(X_mat, pinv_mat);

    DenseMatrix* W_mat = DenseMatrix_create(Matrix_rows(pinv_mat), Matrix_cols(Y_mat));
    DenseMatrix_dot(pinv_mat, Y_mat, W_mat);

    DenseMatrix_elem_ptr(W_mat, W_ptr);
    
    *b = W_ptr[0];

    for (k=1, i=0; k<Matrix_cols(W_mat); ++k, ++i) {
        W[i] = W_ptr[k];
    }

    DenseMatrix_destroy(X_mat);
    DenseMatrix_destroy(Y_mat);
    DenseMatrix_destroy(pinv_mat);
    DenseMatrix_destroy(W_mat);
    return 0;
}

double Linear_Regression_predict(u_array_t* X, double* w, size_t wn, double b)
{
    return 0.f;
}

