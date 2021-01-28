/*
 * @Author: your name
 * @Date: 2021-01-12 07:19:25
 * @LastEditTime: 2021-01-28 15:04:35
 * @LastEditors: Please set LastEditors
 * @Description: 倒梅儿系数计算
 * @FilePath: /boring-code/src/mfcc/mfcc.h
 */

#ifndef _MFCC_H_
#define _MFCC_H_

double** create_mel_filtebank(int n_filter, int n_fft, int samplerate, int low_freq, int high_freq);

int mfcc(double* raw, size_t raw_length, float frame_duration, float step_duration, int samplerate, int filter_n, int coe_n);

#endif