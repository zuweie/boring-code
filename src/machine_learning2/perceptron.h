/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-03-28 16:59:44
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-04-03 13:28:19
 * @FilePath: /boring-code/src/machine_learning2/perceptron.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __PERCEPTRON_H__
#define __PERCEPTRON_H__

#include "matrix2.h"


int pct_train(matrix2_t* _Data, matrix2_t** _W_b, float learning_rate, int max_iter);
int pct_perdict(matrix2_t* _X, matrix2_t* _W_b, vfloat_t* perdict);

#endif