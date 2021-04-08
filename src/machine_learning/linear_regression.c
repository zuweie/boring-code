/*
 * @Author: your name
 * @Date: 2021-03-22 15:04:24
 * @LastEditTime: 2021-04-08 21:47:01
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/machine_learning/linear_regression.c
 */
#include <stdio.h>
#include <stdlib.h>
#include "ultra_array/ultra_array.h"
#include "matrix/matrix.h"

static vfloat_t 
__vet_dot_vet(vfloat_t X[], vfloat_t Y[], size_t n) 
{
    vfloat_t v = 0.f;
    for (size_t i=0; i<n; ++i) {
        v += X[i] * Y[i];
    }
    return v;
}

static vfloat_t 
__vet_sum(vfloat_t X[], size_t n) 
{
    vfloat_t v = 0.f;
    for (size_t i=0; i<n; ++i) {

        v += X[i];

    }
    return v;
}

// 线性回归算法 解方程法
int Linear_Regression_solve(u_array_t* X, u_array_t* Y,  vfloat_t* W, vfloat_t* b)
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

    vfloat_t* Y_ptr = UA_data_ptr(Y);
    matrix_t X_mat = Mat_load(Xr, Xc, UA_data_ptr(X));

    // number of Xi + b = Xc + 1;
    size_t coe_n = Xc + 1; 
    matrix_t coe_mat = Mat_create(coe_n, coe_n);
    Mat_eptr(&coe_mat, coe_ptr);
    size_t i, j, k, l, m;
    vfloat_t _Y[coe_n];
    vfloat_t _X_col[Xr];
    vfloat_t _X_col_2[Xr];

    // 计算 dRh / dW 的参数矩阵。
    for (i=0; i<Xc; ++i) {
        
        // Y 的参数列表
        Mat_get_col(&X_mat, i, _X_col);
        _Y[i] = 2 * __vet_dot_vet(Y_ptr, _X_col, Xr);

        // w dot X 的参数列表 的参数列表
        for (j=0; j<Xc; ++j) {
            if (i == j) {
                coe_ptr[i][j] = 2 * __vet_dot_vet(_X_col, _X_col, Xr);
            } else {
                Mat_get_col(&X_mat, i, _X_col_2);
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
        Mat_get_col(&X_mat, l, _X_col);
        coe_ptr[i][l] = 2 * __vet_sum(_X_col, Xr);
    }
    // 最后一个 b 的参数。
    coe_ptr[i][l] = 2 * Xr;

    // 线性方程求解
    Mat_solve(&coe_mat, _Y, coe_n);
    for (i=0; i<coe_n-1;++i) {
        W[i] = _Y[i];
    }
    *b = _Y[coe_n-1];
    Mat_destroy(&coe_mat);
    Mat_destroy(&X_mat);
    return 0;
}

// 线性回归法，伪逆矩阵法
int Linear_Regression_pseudo_inverse(u_array_t* X, u_array_t* Y, vfloat_t* W, vfloat_t* b) 
{
    int shape = UA_axisn(X);
    size_t Xr, Xc, i, j, k;
    if (shape == 2) {
        
        Xr = UA_shape_axis(X, 0);
        Xc = UA_shape_axis(X, 1);

    } else return -1;

    if (UA_axisn(Y) != 1 && (Y, 0) != Xr) return -1;

    matrix_t X_mat = Mat_load(Xr, Xc, UA_data_ptr(X));
    matrix_t Y_mat = Mat_load(UA_shape_axis(Y,0), 1, UA_data_ptr(Y));

    Mat_eptr(&X_mat, X_ptr);
    vfloat_t (*UX_ptr)[Xc] = UA_data_ptr(X);

    Mat_insert_col_by_value(&X_mat, 0, 1.f);

    Mat_pseudo_inverse(&X_mat);

    Mat_dot(&X_mat, &Y_mat);
    
    *b = X_mat.elems[0];

    for (k=1, i=0; k<Mat_rows(&X_mat); ++k, ++i) {
        W[i] = X_mat.elems[k];
    }

    Mat_destroy(&X_mat);
    Mat_destroy(&Y_mat);
    return 0;
}

vfloat_t Linear_Regression_predict(u_array_t* X, vfloat_t* w, size_t wn, vfloat_t b)
{
    return 0.f;
}

