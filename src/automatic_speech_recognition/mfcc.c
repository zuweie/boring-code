/*
 * @Author: your name
 * @Date: 2021-01-12 07:19:35
 * @LastEditTime: 2021-01-28 10:10:06
 * @LastEditors: Please set LastEditors
 * @Description: 倒梅儿系数计算
 * @FilePath: /boring-code/src/mfcc/mfcc.c
 */
#include <stdlib.h>
#include <math.h>
#include "mfcc.h"

#define __hz_2_mel(f) (2595.f * log10(1.f + (f)/700.f))
#define __mel_2_hz(m) (700.f *(pow(10.f, (m)/2595.f)-1.f))
#define __min(x,y) ((x)<(y)?(x):(y))
#define __max(x,y) __min(y,x)
/**
 * 将高频和低频线性切分：
 * 
 */
static int __calculate_fft_bin(double high_mel, double low_mel, int n_filter, int n_fft, int bin[], int samplerate)
{
    double _per_mel = (high_mel - low_mel) / (double)(n_filter+1);
    
    for (int i=0; i<(n_filter+2); ++i) {
        double mel_point = i*_per_mel + low_mel;
        bin[i] = (int)((n_fft + 1) * __mel_2_hz(mel_point) / (double)(samplerate));
    }
    return 0;
}

static int __calcate_mel_filterbank(int n_filter, int n_fft, int samplerate, int low_freq, int high_freq, double filters[][n_fft/2+1])
{
    double low_mel  = __hz_2_mel(low_freq);
    double high_mel = __hz_2_mel(high_freq);

    int bin[n_filter+2];
    double per_mel = (high_mel - low_mel) / (double)(n_filter+1);
    for (int i=0; i<(n_filter+2); ++i) {
        double mel_point = i*per_mel + low_mel;
        bin[i] = (int)(floor( (n_fft+1)*__mel_2_hz(mel_point)/(double)(samplerate) ));
    }
    for (int i=0; i<n_filter; ++i) {
        for (int j=bin[i]; j<bin[i+1]; ++j) {
            filters[i][j] = (double)(j-bin[i]) / (double)(bin[i+1]-bin[i]);
        }
        for (int j=bin[i+1]; j<bin[i+2]; ++j){
            filters[i][j] = (double)(bin[i+2]-j) / (double)(bin[i+2] - bin[i+1]);
        }
    }
    return 0;
}

double** create_mel_filtebank(int n_filter, int n_fft, int samplerate, int low_freq, int high_freq) 
{
    double **filters = calloc(n_filter, (n_fft/2+1)*sizeof(double));
    __calcate_mel_filterbank(n_filter, n_fft, samplerate, low_freq, high_freq, filters);
    return filters;
}

int mfcc(void) 
{  
    // 最后努力了，可以出梅尔系数了。我操尼玛的。
}