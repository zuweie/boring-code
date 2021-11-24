/*
 * @Author: your name
 * @Date: 2021-11-15 16:45:08
 * @LastEditTime: 2021-11-24 16:45:16
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /boring-code/src/machine_learning/neural_network.c
 */
#include "neural_network.h"

int ann_mpl_train(ann_mpl_model_t* model, u_array_t* layer_size, u_array_t* X, u_array_t* Y, ann_mpl_param_t* params)
{
    int len_Xc = UA_shape_axis(X, 1);
    int len_Xr = UA_shape_axis(X, 0);
    
}

int ann_mpl_predict(ann_mpl_model_t* model, u_array_t* sample, u_array_t* prediction)
{

}

int ann_mpl_model_finalize(ann_mpl_model_t* model)
{

}