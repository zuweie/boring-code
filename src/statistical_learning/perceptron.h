/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-03-28 16:59:44
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-06-15 08:27:34
 * @FilePath: /boring-code/src/statistical_learning/perceptron.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __PERCEPTRON_H__
#define __PERCEPTRON_H__

#include "matrix2.h"


int pct_train(matrix2_t* _Data, matrix2_t** _W_b, float learning_rate, int max_iter);
int pct_predict(matrix2_t* _X, matrix2_t* _W_b, vfloat_t* perdict);

#endif