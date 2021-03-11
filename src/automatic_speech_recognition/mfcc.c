/*
 * @Author: your name
 * @Date: 2021-01-12 07:19:35
 * @LastEditTime: 2021-03-11 12:11:38
 * @LastEditors: Please set LastEditors
 * @Description: 倒梅儿系数计算
 * @FilePath: /boring-code/src/mfcc/mfcc.c
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include "signal_process.h"
#include "matrix/DenseMatrix.h"
#include "cosine_transform/cosine_transform.h"
#include "ultra_array/ultra_router.h"
#include "ultra_array/ultra_array.h"
#include "mfcc.h"

#define __hz_2_mel(f) (2595.f * log10(1.f + (f)/700.f))
#define __mel_2_hz(m) (700.f *(pow(10.f, (m)/2595.f)-1.f))
#define __min(x,y) ((x)<(y)?(x):(y))
#define __max(x,y) __min(y,x)
/**
 * 将高频和低频线性切分：
 * 
 */
static void __lifter(u_array_t* arr, int L) 
{
    
    size_t frame_size = UA_shape_axis(arr, 1);
    size_t frame_number = UA_shape_axis(arr, 0);
    double lift[frame_size];

    if (L > 0) {
        for (int n=0; n<frame_size; ++n) {
            lift[n] = 1 + (L/2.f)*sin(3.1415926535898 * n/L);
        }
    }
    double (*__frames)[frame_size] = UA_data_ptr(arr);
    for (int i=0; i<frame_number; ++i) {
        for (int j=0; j<frame_size; ++j) {
            __frames[i][j] = lift[j] * __frames[i][j];
        }
    }
    return;
}

static void
__dct(u_array_t* arr, int cep_n) {
    double (*__frames)[UA_shape_axis(arr, 1)] = UA_data_ptr(arr);
    double dct[cep_n];
    for (int i=0; i<UA_shape_axis(arr, 0); ++i) {
        Discrete_cosine_transform(__frames[i], UA_shape_axis(arr, 1), cep_n, dct, dct_ortho, dct_ii);
        // 计算完了把结果复制到原来的数组。
        memcpy(__frames[i], dct, cep_n * sizeof(double));
    }
    return;
}

u_array_t create_mel_filterbank(int filter_n, int fft_n, int samplerate, int low_freq, int high_freq)
{
    double low_mel  = __hz_2_mel(low_freq);
    double high_mel = __hz_2_mel(high_freq);

    int bin[filter_n+2];
    double per_mel = (high_mel - low_mel) / (double)(filter_n+1);

    // 计算 bin 不知道有什么用。
    for (int i=0; i<(filter_n+2); ++i) {
        double mel_point = i*per_mel + low_mel;
        bin[i] = (int)(floor( (fft_n+1)*__mel_2_hz(mel_point)/(double)(samplerate) ));
    }

    u_array_t filters = _UArray2d(filter_n, fft_n/2+1);
    // 先把内存清 0.f;
    UA_ones(&filters, 0.f);
    double (*_filters)[fft_n/2+1] = UA_data_ptr(&filters);

    for (int i=0; i<filter_n; ++i) {
        for (int j=bin[i]; j<bin[i+1]; ++j) {
            _filters[i][j] = (double)(j-bin[i]) / (double)(bin[i+1]-bin[i]);
        }
        for (int j=bin[i+1]; j<bin[i+2]; ++j){
            _filters[i][j] = (double)(bin[i+2]-j) / (double)(bin[i+2] - bin[i+1]);
        }
    }
    return filters;
}



int f_bank(double* raw, size_t raw_length, float frame_duration, float step_duration, int samplerate, int filter_n,  int fft_n, int freq_low, int freq_high, float preemph, u_array_t* feat, u_array_t* energy)
{  
    freq_high = freq_high ? freq_high : samplerate / 2;
    // 做分帧 以及计算每一帧的能量 返回 426 * xxx 的数组
    u_array_t frames = frames_pow_signale(raw, raw_length, frame_duration, step_duration, samplerate, fft_n, preemph, NULL);
    if (energy != NULL) {
        *energy = UA_copy(&frames);
        UA_sum(energy, 1);
        UA_where(energy, == 0.f, 0.001f);
    }
    // 获取梅尔滤波 返回 26 * xxxx 的二维数组。 26 为 梅尔滤波的个数。filter_n 传入。
    u_array_t filters = create_mel_filterbank(filter_n, fft_n, samplerate, freq_low, freq_high);
    // frames dot filters 后 为 426 X xxxx 的 数组。
    *feat = UA_dot(&frames, UA_T(&filters));
    UA_where(feat, == 0.f, 0.001f);
    UArray_(&frames);
    UArray_(&filters);
    return 0;
}

u_array_t log_f_bank(double* raw, size_t raw_len, float win_len, float win_step, int samplerate, int filter_n, int fft_n, int freq_low, int freq_high, float preemph)
{
    fft_n = fft_n == 0 ? calculate_fft_n(win_len * samplerate) : fft_n;
    u_array_t feat;
    f_bank(raw, raw_len, win_len, win_step, samplerate, filter_n, fft_n, freq_low, freq_high, preemph, &feat, NULL);
    UA_log(&feat);
    return feat;
}

u_array_t mfcc(double* raw, size_t raw_len, int samplerate, float win_len, \
        float win_step, int cep_n, int filter_n, int fft_n, int freq_low, \
        int freq_high, float preemph, int cep_lifter, int append_energy) 
{
    // 计算一帧所要的 fft_n 的点数。
    fft_n = fft_n == 0 ? calculate_fft_n(win_len * samplerate) : fft_n;
    u_array_t feat, energy;
    f_bank(raw, raw_len, win_len, win_step, samplerate, filter_n, fft_n, freq_low, freq_high, preemph, &feat, &energy);
    UA_log(&feat);

    __dct(&feat, cep_n);
    
    //char router[128] = {'\0'};
    //sprintf(router, ":,:%d", cep_n);
    ua_indicator_t* idx = __indicators_start_tail(NULL, 0, 0);
    idx = __indicators_tail(idx, cep_n);

    u_array_t feat1 = UArray_fission_with_indicators(&feat, idx); 

    __lifter(&feat1, cep_lifter);
    if (append_energy) {
       UA_log(&energy);
       UA_assimilate(&feat1, ":,0", &energy);
    }
    
    UArray_(&feat);
    UArray_(&energy);
    UArray_indicator_release(idx);
    return feat1;
}


u_array_t delta(u_array_t* feat, int N) 
{
    if (N > 1) {
        
        size_t NUMFRAMES = UA_shape_axis(feat, 0);
        int denominator = 0;
        for (int i =1; i < N+1; ++i) {
            denominator += i*i;
        }
        denominator *= 2;
        u_array_t delta_feat = UA_empty_like(feat);

        char pad_str_format = "((%d,%d),(0,0))";
        //char router_str_format = "%d:%d";
        //char router_str_format2 = "%d,:";
        char str_buf[64];
        sprintf(str_buf, pad_str_format, N, N);

        u_array_t padded = UA_pad_edge(feat, str_buf);

        u_array_t u1 = _UArray1d(5);
        UA_scope(&u1, -1*N, N+1);

        for (int t=0; t<NUMFRAMES; ++t) {

            ua_indicator_t* indicators_fission = __indicators_start_tail(NULL, t, t+2*N+1);
            
            u_array_t u2 = UArray_fission_with_indicators(&padded, indicators_fission);//UA_fission(&padded, str_buf);
            u_array_t u3 = UA_dot(&u1, &u2);

            ua_indicator_t* indicators_assimilate = __indicators_picked(NULL, t);
            indicators_assimilate = __indicators_start_tail(indicators_assimilate, 0, 0);

            UArray_assimilate_with_indicators(&delta_feat, indicators_assimilate, &u3);

            UArray_indicator_release(indicators_fission);
            UArray_indicator_release(indicators_assimilate);
            
            UArray_(&u2);
            UArray_(&u3);
        }
        UArray_(&u1);
        return delta_feat;
    }
    return ua_unable;
}