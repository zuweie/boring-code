/*
 * @Author: your name
 * @Date: 2021-01-12 07:19:35
 * @LastEditTime: 2021-01-21 11:28:03
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

static int __div_frames(double *raw) 
{

}
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
    double high_mel = __hz_2_mel(high_mel);

    int bin[n_filter+2];
    double per_mel = (high_mel - low_mel) / (double)(n_filter+1);
    for (int i=0; i<(n_filter+2); ++i) {
        double mel_point = i*per_mel + low_mel;
        bin[i] = (int)(floor( (n_fft+1)*__mel_2_hz(mel_point)/(double)(samplerate) ));
    }
    for (int i=0; i<n_filter; ++i) {
        for (int j=bin[j]; j<bin[j+i]; ++j) {
            filters[i][j] = (double)(i-bin[j]) / (double)(bin[j+i]-bin[j]);
        }
        for (int j=bin[j+1]; j<bin[j+2]){
            filters[i][j] = (double)(bin[j+2]-i) / (double)(bin[j+2] - bin[j+1]);
        }
    }
    return 0;
}

static double** __create_mel_filtebank(int n_filter, int n_fft, int samplerate, int low_freq, int high_freq, double **filters) 
{
    double **filters = (double**)calloc(n_filter, n_fft/2+1);
    __calcate_mel_filterbank(n_filter, n_fft, samplerate, low_freq, high_freq, filters);
    return filters;
}

int mfcc(void) 
{
    return 0;
}