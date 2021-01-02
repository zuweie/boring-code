/*
 * @Author: your name
 * @Date: 2020-12-29 16:12:36
 * @LastEditTime: 2021-01-02 09:28:50
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/fourier_transform/fourier_transform.c
 */
#include <stdio.h>
#include <math.h>
#include "complex/complex.h"
#include "fourier_transform.h"
#define PI 3.1415926

/**
 * 离散傅立叶变换, O(n^2) 版本
 * squence 采样数据，大小一般为 2^x 大小
 * out 傅立叶变换输出的复数
*/
int discrete_fourier_transform (float sequence[], size_t sequence_size, complex_t out[]) 
{
    size_t N = sequence_size;
    complex_t k_point;
    for (int i = 0; i<N; ++i) {
        k_point = _complex(0.f, 0.f);
        for (int n=0; n<N; ++n) {
            k_point.real  += cos(2*PI/N * i * n) * sequence[n];
            k_point.image -= sin(2*PI/N * i * n) * sequence[n];
        }
        out[i] = k_point;
    }
}

int fast_fourier_transform(void) 
{
    
}