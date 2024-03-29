/*
 * @Author: your name
 * @Date: 2021-01-12 07:19:25
 * @LastEditTime: 2021-10-24 09:18:48
 * @LastEditors: Please set LastEditors
 * @Description: 倒梅儿系数计算
 * @FilePath: /boring-code/src/mfcc/mfcc.h
 */

#ifndef __MFCC_H__
#define __MFCC_H__
#include "ultra_array/ultra_array.h"
#include "vtype/vfloat_type.h"
u_array_t create_mel_filterbank(int n_filter, int n_fft, int samplerate, int low_freq, int high_freq);

int f_bank(vfloat_t* raw, size_t raw_length, float frame_duration, \
            float step_duration, int samplerate, int filter_n,  \
            int fft_n, int freq_low, int freq_high, float preemph, \
            u_array_t* feat, u_array_t* energy);
            
u_array_t log_f_bank(vfloat_t* raw, size_t raw_len, float win_len, \
                    float win_step, int samplerate, int filter_n,  \
                    int fft_n, int freq_low, int freq_high, float preemph);

u_array_t mfcc(vfloat_t* raw, size_t raw_len, int samplerate, \
    float win_len, float win_step, int cep_n, \
    int filter_n, int fft_n, \
    int freq_low, int freq_high, \
    float preemph, int cep_lifter_n, int append_energy);

u_array_t delta(u_array_t* feat, int N);


#endif