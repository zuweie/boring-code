/*
 * @Author: your name
 * @Date: 2020-12-29 16:12:36
 * @LastEditTime: 2021-01-04 09:59:03
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/fourier_transform/fourier_transform.c
 */
#include <stdlib.h>
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
            k_point.image += sin(2*PI/N * i * n) * sequence[n];
        }
        out[i] = k_point;
    }
    return 0;
}

int recursive_fast_fourier_transform(double* sequence, size_t n, complex_t* y) 
{
    if (n == 1) {
        y[0] = _complex(sequence[0], 0);
        return 0;
    }
    
    size_t half_n = n>>1;
    double sub_seq[n];
    complex_t wn = complex_w(n, 1);
    complex_t w1 = _complex(1,0);

    for (int i=0, j=0; i < n; i = i+2, ++j) {
        sub_seq[j]        = sequence[i];
        sub_seq[j+half_n] = sequence[i+1];
    }
    
    complex_t yk[n];
    recursive_fast_fourier_transform(sub_seq, half_n, yk);
    recursive_fast_fourier_transform(&sub_seq[half_n], half_n, &yk[half_n]);

    for (int k=0; k < half_n; k++) {
        complex_t wy1 = complex_multiply(w1, yk[half_n+k]);
        y[k] = complex_add(yk[k], wy1);
        y[k+half_n] = complex_substract(yk[k], wy1);
        w1 = complex_multiply(w1, wn);
    }
    return 0;
}