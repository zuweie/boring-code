/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-11-20 09:28:39
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-11-20 10:35:30
 * @FilePath: /boring-code/src/deep_learning/ann.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "ann.h"
int ann_train(matrix2_t* data, matrix2_t* label, int* hidden_layers, int hidden_layers_number, int (*active)(matrix2_t* _X,vfloat_t* params), int (*d_active)(matrix2_t* _X, vfloat_t* params), matrix2_t** out_Wbs)
{
    return 0;
}
int ann_predict(matrix2_t* _Input, matrix2_t* Wbs, int hidden_layers_number, vfloat_t* predict)
{
    return 0;
}

int ann_recycle(matrix2_t* Wbs)
{
    return 0;
}
