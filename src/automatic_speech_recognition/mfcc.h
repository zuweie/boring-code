/*
 * @Author: your name
 * @Date: 2021-01-12 07:19:25
 * @LastEditTime: 2021-03-09 20:58:05
 * @LastEditors: Please set LastEditors
 * @Description: 倒梅儿系数计算
 * @FilePath: /boring-code/src/mfcc/mfcc.h
 */

#ifndef _MFCC_H_
#define _MFCC_H_
#include "ultra_array/ultra_array.h"

u_array_t create_mel_filterbank(int n_filter, int n_fft, int samplerate, int low_freq, int high_freq);

int f_bank(double* raw, size_t raw_length, float frame_duration, \
            float step_duration, int samplerate, int filter_n,  \
            int fft_n, int freq_low, int freq_high, float preemph, \
            u_array_t* feat, u_array_t* energy);
            
u_array_t log_f_bank(double* raw, size_t raw_len, float win_len, \
                    float win_step, int samplerate, int filter_n,  \
                    int fft_n, int freq_low, int freq_high, float preemph);

u_array_t mfcc(double* raw, size_t raw_len, int samplerate, \
    float win_len, float win_step, int cep_n, \
    int filter_n, int fft_n, \
    int freq_low, int freq_high, \
    float preemph, int cep_lifter_n, int append_energy);

u_array_t delta(u_array_t* feat, int N);


#endif