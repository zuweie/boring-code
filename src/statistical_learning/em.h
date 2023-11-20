/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-08-15 14:50:39
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-11-20 10:25:23
 * @FilePath: /boring-code/src/statistical_learning/em.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __EM_H__
#define __EM_H__

#include "matrix2/matrix2.h"

int EM_train(matrix2_t* _X, int K, int Max_iter, double eps, double** alphas, matrix2_t** mus, matrix2_t** sigmas, void (*progress)(const char*, double, unsigned long, unsigned long));
int EM_recycle(int K, double** alphas, matrix2_t** mus, matrix2_t** sigmas);
#endif