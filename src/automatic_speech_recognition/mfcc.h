/*
 * @Author: your name
 * @Date: 2021-01-12 07:19:25
 * @LastEditTime: 2021-01-24 14:49:34
 * @LastEditors: Please set LastEditors
 * @Description: 倒梅儿系数计算
 * @FilePath: /boring-code/src/mfcc/mfcc.h
 */

#ifndef _MFCC_H_
#define _MFCC_H_

double** create_mel_filtebank(int n_filter, int n_fft, int samplerate, int low_freq, int high_freq);

int mfcc(void);

#endif