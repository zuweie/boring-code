/*
 * @Author: your name
 * @Date: 2020-12-29 16:14:16
 * @LastEditTime: 2021-04-09 09:31:28
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/fourier_transform/fourier_transform.h
 */

#ifndef _FOURIER_TRANSFORM_
#define _FOURIER_TRANSFORM_
#include "complex/complex.h"
#include "vtype/vfloat_type.h"

#define Bit_reverse_32(v, s) \
    ({ \
        unsigned __marco_r = v; \
        __marco_r =((__marco_r >>1)&0x55555555)|((__marco_r &0x55555555)<<1); \
        __marco_r =((__marco_r >>2)&0x33333333)|((__marco_r &0x33333333)<<2); \
        __marco_r =((__marco_r >>4)&0x0F0F0F0F)|((__marco_r &0x0F0F0F0F)<<4); \
        __marco_r =((__marco_r >>8)&0x00FF00FF)|((__marco_r &0x00FF00FF)<<8); \
        __marco_r =(__marco_r>>16)|(__marco_r<<16); \
        __marco_r >> (32-s); \
    })

int Discrete_fourier_transform (float[], int, complex_t[]);
int Recursive_fast_fourier_transform(complex_t[], int, complex_t[], int reverse);
int Iterative_fast_fourier_transform(complex_t[], int, complex_t[], int reverse);
int Iterative_fast_fourier_transform2(complex_t[], int, int reverse);

// 实数序列的傅立叶变换。
int Real_fast_fourier_transform(vfloat_t[], int);
int Real_fast_fourier_transform_complex(vfloat_t[], int, complex_t[]);

int Reverse_recursive_fast_fourier_transorm(complex_t[], int, complex_t[]);
int Reverse_iterative_fast_fourier_transorm(complex_t[], int, complex_t[]);
int Reverse_iterative_fast_fourier_transorm2(complex_t sequence[], int N);

#endif