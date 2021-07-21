/*
 * @Author: your name
 * @Date: 2021-06-22 14:54:10
 * @LastEditTime: 2021-07-21 16:16:19
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/machine_learning/svm/svm_model.c
 */
#include "svm_kernel_function.h"
#include "vtype/vfloat_type.h"
#include "svm_model.h"

int svm_model_finalize(svm_model_t* model)
{
    UArray_(&model->_star_alpha);
    UArray_(&model->_star_X);
    UArray_(&model->_star_Y);

}

u_array_t svm_model_calculate_liner(svm_model_t* model, u_array_t* sample)
{
    int len_Xr = UA_shape_axis(&model->_star_X, 0);
    int len_Xc = UA_shape_axis(&model->_star_X, 1);
    vfloat_t (*X_ptr)[len_Xc] = UA_data_ptr(&model->_star_X);
    vfloat_t* sample_ptr      = UA_data_ptr(sample);
    u_array_t calculate_result = _UArray1d(len_Xr);
    vfloat_t* cal_result_ptr   = UA_data_ptr(&calculate_result);

    for (int i=0; i<len_Xr; ++i) {
        cal_result_ptr[i] = kernel_function_calculate_liner(X_ptr[i], sample_ptr, len_Xc, 1, 0);
    }
    return calculate_result;
}
u_array_t svm_model_calculate_poly(svm_model_t* model, u_array_t* sample)
{
    int len_Xr = UA_shape_axis(&model->_star_X, 0);
    int len_Xc = UA_shape_axis(&model->_star_X, 1);
    vfloat_t (*X_ptr)[len_Xc] = UA_data_ptr(&model->_star_X);
    vfloat_t* sample_ptr      = UA_data_ptr(sample);
    u_array_t calculate_result = _UArray1d(len_Xr);
    vfloat_t* cal_result_ptr   = UA_data_ptr(&calculate_result);

    for (int i=0; i<len_Xr; ++i) {
        cal_result_ptr[i] = kernel_function_calculate_poly(X_ptr[i], sample_ptr, len_Xc, model->k_param.degree);
    }
    return calculate_result;
}

u_array_t svm_model_calculate_sigmoid(svm_model_t* model, u_array_t* sample)
{
    int len_Xr = UA_shape_axis(&model->_star_X, 0);
    int len_Xc = UA_shape_axis(&model->_star_X, 1);
    vfloat_t (*X_ptr)[len_Xc] = UA_data_ptr(&model->_star_X);
    vfloat_t* sample_ptr      = UA_data_ptr(sample);
    u_array_t calculate_result = _UArray1d(len_Xr);
    vfloat_t* cal_result_ptr   = UA_data_ptr(&calculate_result);

    for (int i=0; i<len_Xr; ++i) {
        cal_result_ptr[i] = kernel_function_calculate_sigmoid(X_ptr[i], sample_ptr, len_Xc, model->k_param.gammer, model->k_param.coef0);
    }
    return calculate_result;
}

u_array_t svm_model_calculate_rbf(svm_model_t* model, u_array_t* sample)
{
    int len_Xr = UA_shape_axis(&model->_star_X, 0);
    int len_Xc = UA_shape_axis(&model->_star_X, 1);
    vfloat_t (*X_ptr)[len_Xc] = UA_data_ptr(&model->_star_X);
    vfloat_t* sample_ptr      = UA_data_ptr(sample);
    u_array_t calculate_result = _UArray1d(len_Xr);
    vfloat_t* cal_result_ptr   = UA_data_ptr(&calculate_result);

    for (int i=0; i<len_Xr; ++i) {
        cal_result_ptr[i] = kernel_function_calculate_rbf(X_ptr[i], sample_ptr, len_Xc, model->k_param.gammer);
    }
    return calculate_result;
}