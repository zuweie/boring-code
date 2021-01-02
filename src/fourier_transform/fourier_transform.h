/*
 * @Author: your name
 * @Date: 2020-12-29 16:14:16
 * @LastEditTime: 2021-01-02 09:29:17
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/fourier_transform/fourier_transform.h
 */

#ifndef _FOURIER_TRANSFORM_
#define _FOURIER_TRANSFORM_
#include "complex/complex.h"

int discrete_fourier_transform (float[], size_t, complex_t[]);
int fast_fourier_transform(void);

#endif