/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-11-24 12:03:40
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-12-14 11:28:17
 * @FilePath: /boring-code/src/deep_learning/active.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __ACTIVE_H__
#define __ACTIVE_H__
#include "matrix2/matrix2.h"

int sigmoid (matrix2_t* vec, double* params);
int d_sigmoid (matrix2_t* vec, double* params);
int symmetric_sigmoid (matrix2_t* vec, double* params);
int d_symmetric_sigmoid(matrix2_t* vec, double* params);

int tanh1 (matrix2_t* vec, double* params);
int d_tanh1 (matrix2_t* vec, double* params);
int softmax(matrix2_t* vec, double* params);
int d_softmax(matrix2_t* vec, double* params);

#endif