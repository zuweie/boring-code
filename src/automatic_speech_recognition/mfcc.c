/*
 * @Author: your name
 * @Date: 2021-01-12 07:19:35
 * @LastEditTime: 2021-01-28 15:04:12
 * @LastEditors: Please set LastEditors
 * @Description: 倒梅儿系数计算
 * @FilePath: /boring-code/src/mfcc/mfcc.c
 */
#include <stdlib.h>
#include <math.h>
#include "signal_process.h"
#include "matrix/DenseMatrix.h"
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

double** create_mel_filtebank(int filter_n, int fft_n, int samplerate, int low_freq, int high_freq) 
{
    double **filters = calloc(filter_n, (fft_n/2+1)*sizeof(double));
    __calcate_mel_filterbank(filter_n, fft_n, samplerate, low_freq, high_freq, filters);
    return filters;
}

int mfcc(double* raw, size_t raw_length, float frame_duration, float step_duration, int samplerate, int filter_n, int coe_n)
{  
    int frame_fftn;
    int frame_size;
    int frame_number;

    int low_freq = 0;
    int high_freq = samplerate / 2;

    // 做分帧 426 * 129
    void* frame_data   = frames_signale(raw, raw_length, frame_duration, step_duration, samplerate, &frame_fftn, &frame_size, &frame_size, NULL);
    // 获取梅尔滤波 26 * 129
    void* filter_data  = create_mel_filtebank(filter_n, frame_fftn, samplerate, low_freq, high_freq);
    
    
    DenseMatrix* frames  = DenseMatrix_set(frame_number, frame_fftn/2+1, frame_data);
    DenseMatrix* filters = DenseMatrix_set(filter_n, frame_fftn/2+1, filter_data);
    DenseMatrix* fb      = DenseMatrix_create(frame_number, filter_n);
    Matrix_trans(filters);
    Matrix_product(frames, filters, fb);

    // log 滤波后的的数据。
    // for (int i=0; i<frame_number; ++i) {
    //     for (int j; j<filter_n; ++j) {
    //         fb[i][j] = log(fb[i][j]);
    //     }
    // }

    
    // clean up memory
    DenseMatrix_destroy(frames);
    DenseMatrix_destroy(filters);
    DenseMatrix_destroy(fb);
    
    // 两个数据做点
    free(frame_data);
    free(filter_data);

    // 最后努力了，可以出梅尔系数了。我操尼玛的。
}