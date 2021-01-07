/*
 * @Author: your name
 * @Date: 2020-12-29 16:12:36
 * @LastEditTime: 2021-01-08 00:49:40
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
int Discrete_fourier_transform (float sequence[], size_t sequence_size, complex_t out[]) 
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

int Recursive_fast_fourier_transform(complex_t sequence[], size_t n, complex_t y[], int reverse) 
{
    if (n == 1) {
        y[0] = sequence[0];
        return 0;
    }
    
    size_t half_n = n>>1;
    complex_t sub_seq[n];
    complex_t wn = reverse?complex_rw(n, 1):complex_w(n, 1);
    complex_t w1 = _complex(1,0);

    for (int i=0, j=0; i < n; i = i+2, ++j) {
        sub_seq[j]        = sequence[i];
        sub_seq[j+half_n] = sequence[i+1];
    }
    
    complex_t yk[n];
    Recursive_fast_fourier_transform(sub_seq, half_n, yk, reverse);
    Recursive_fast_fourier_transform(&sub_seq[half_n], half_n, &yk[half_n], reverse);

    for (int k=0; k < half_n; k++) {
        complex_t wy1 = complex_multiply(w1, yk[k+half_n]);
        y[k] = complex_add(yk[k], wy1);
        y[k+half_n] = complex_substract(yk[k], wy1);
        w1 = complex_multiply(w1, wn);
    }
    return 0;
}

static void __Bit_reverse_copy(complex_t a[], size_t size_a, complex_t A[]) 
{
    int r = log2(size_a);

    for (int k=0; k<size_a; ++k) {
        int k_index = Bit_reverse_32(k, r);
        A[k_index] = a[k];
    }
    
    return;
}

int Iterative_fast_fourier_transform(complex_t sequence[], size_t N, complex_t A[], int reverse) 
{
    // bit reverse_copy
    __Bit_reverse_copy(sequence, N, A);
    
    int n = log2(N);

    for (int s=1; s<=n; ++s) {
        int m = pow(2, s);
        int half_m = m>>1;
        complex_t wm = reverse? complex_rw(m, 1):complex_w(m, 1);
        for (int k=0; k<N; k=k+m) {
            complex_t w = _complex(1, 0);
            for (int j=0; j<(half_m); ++j) {
                complex_t t = complex_multiply(w, A[k+j+half_m]);
                complex_t u = A[k+j];
                A[k+j] = complex_add(u, t);
                A[k+j+half_m] = complex_substract(u, t);
                w = complex_multiply(w, wm);
            }
        }
    }
    return 0;
}

int Reverse_recursive_fast_fourier_transorm(complex_t sequence[], size_t n, complex_t out[]) 
{
    Recursive_fast_fourier_transform(sequence, n, out, 1);
    for (int i=0; i<n; ++i) {
        out[i].real  /= (double) n;
        out[i].image /= (double) n;
    }
    return 0;
}

int Reverse_iterative_fast_fourier_transorm(complex_t sequence[], size_t N, complex_t A[])
{
    Iterative_fast_fourier_transform(sequence, N, A, 1);

    for (int i=0; i<N; ++i) {
        A[i].real  /= (double) N;
        A[i].image /= (double) N;
    }
    return 0;
}
