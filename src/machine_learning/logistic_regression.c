/*
 * @Author: your name
 * @Date: 2021-04-04 15:29:30
 * @LastEditTime: 2021-05-08 16:31:02
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/machine_learning/logistic_regression.c
 */
#include "vtype/vfloat_type.h"
#include "logistic_regression.h"
#include "ultra_array/ultra_array.h"
#include "ultra_array/ultra_router.h"
#include "matrix/matrix.h"


// 具体公式看下面。
// https://www.matongxue.com/lessons/908 
// 使用梯度下降算法。

// 计算 Rh() 函数对 向量 W 的第 i 个分量的求导。
static vfloat_t __dRh_dw(u_array_t* X, u_array_t* y, size_t i, u_array_t* W) 
{
    u_array_t _y1 = UA_copy(y);
    u_array_t _y2 = UA_copy(y);
    u_array_t _y3 = UA_copy(y);

    u_array_t _X1  = UA_copy(X);
    u_array_t _X2  = UA_copy(X);

    // X[:,i], 这里截取 X 的第 i 列。
    __indicators_scope_init(index, 2);
    void* inext = __indicators_scope_start_tail(index, 0, 0);
    __indicators_scope_picked(index, i);

    u_array_t _Xi = UA_fission_indicator(X, index);

    // -y
    UA_mulitply_var(&_y1, -1.f);
    // -y * X[:,i]
    UA_mulitply_uar(&_y1, &_Xi);

    // X@W
    UA_dot(_X1, W);
    // -y
    UA_mulitply_var(&_y2, -1.f);
    // -y*(X@W)
    UA_mulitply_uar(&_y2, _X1);

    // np.exp(-y*(X@W))
    UA_exp(&_y2);

    // -y*X[:,i]*np.exp(-y*(X@W))
    UA_mulitply_uar(&_y1, &_y2);

    // -y
    UA_mulitply_var(&_y3, -1.f);
    // X@W
    UA_dot(&_X2, W);
    // -y * (X@W)
    UA_mulitply_uar(&_y3, &_X2);
    // np.exp(-y*(X@W))
    UA_exp(&_y3);
    // 1 + np.exp(-y*(X@W))
    UA_sum_uar(&_y3, 1);

    // np.mean ( -y*X[:,i]*np.exp(-y*(X@W)) / (1 + np.exp(-y*(X@W))) )
    UA_div_uar(&_y1, &_y3);

    UA_mean(&_y1, -1);


    vfloat_t v = ((vfloat_t*)(UA_data_ptr(&_y1)))[0];

    UArray_(&_y1);
    UArray_(&_y2);
    UArray_(&_y3);

    UArray_(&_X1);
    UArray_(&_X2);

    UArray_(&_Xi);

    return v;
    
}

static vfloat_t __norm_arr(vfloat_t arr[], size_t n)
{
    vfloat_t v = 0.f;
    for (size_t i=0; i<n; ++i) {
        v += arr[i] * arr[i];
    }
    return sqrt(v);
}

/**
 *  X 训练集
 *  y 结果集
 *  W 要训练的参数
 *  eta 梯度下降的步进
 *  epochs 训练的总次数
 *  epsilon 训练退出的阀门
 */
int logistic_regression_train(u_array_t* X, u_array_t* y, u_array_t* W, vfloat_t eta, vfloat_t epochs, vfloat_t epsilon)
{
    size_t Wn = UA_shape_axis(W, 0);
    u_array_t drhdw = UA_empty_like(W);
    for (int i=0; i<epochs; ++i) {

        for (size_t j=0; j<Wn; ++j) {
            vfloat_t v = dRhdw(X, y, j, W);
            UA_set(drhdw, v, j);
        }

        if (__norm_arr(new_W, Wn) < epsilon) break;

        UA_mulitply_var(&drhdw, -1* (eta));
        
        UA_sum_uar(W, &drhdw);
    }
    UArray_(&drhdw);
    return 0;
}