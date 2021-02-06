/*
 * @Author: your name
 * @Date: 2021-01-12 07:19:25
 * @LastEditTime: 2021-02-06 11:24:57
 * @LastEditors: Please set LastEditors
 * @Description: 倒梅儿系数计算
 * @FilePath: /boring-code/src/mfcc/mfcc.h
 */

#ifndef _MFCC_H_
#define _MFCC_H_
#include "ultra_array/ultra_array.h"
#include "mem_pool/__mem_pool.h"
u_array_t create_mel_filterbank(int n_filter, int n_fft, int samplerate, int low_freq, int high_freq, pool_t* alloc);

void* mfcc(double* raw, size_t raw_length, float frame_duration, float step_duration, int samplerate, int filter_n, int coe_n, int ceplifter, pool_t* alloc);

#endif