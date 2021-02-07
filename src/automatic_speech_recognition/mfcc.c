/*
 * @Author: your name
 * @Date: 2021-01-12 07:19:35
 * @LastEditTime: 2021-02-07 10:07:39
 * @LastEditors: Please set LastEditors
 * @Description: 倒梅儿系数计算
 * @FilePath: /boring-code/src/mfcc/mfcc.c
 */
#include <stdlib.h>
#include <math.h>
#include "signal_process.h"
#include "matrix/DenseMatrix.h"
#include "cosine_transform/cosine_transform.h"
#include "ultra_array/ultra_array.h"
#include "mem_pool/__mem_pool.h"
#include "mfcc.h"

#define __hz_2_mel(f) (2595.f * log10(1.f + (f)/700.f))
#define __mel_2_hz(m) (700.f *(pow(10.f, (m)/2595.f)-1.f))
#define __min(x,y) ((x)<(y)?(x):(y))
#define __max(x,y) __min(y,x)
/**
 * 将高频和低频线性切分：
 * 
 */
static void __lifter(double* feat, size_t feat_size, int L) 
{
    double f[feat_size];

    if (L > 0) {
        for (int n=0; n<feat_size; ++n) {
            f[n] = 1 + (L/2.f)*sin(3.1415926535898 * n/L);
        }
    }
    for (int i=0; i<feat_size; ++i){
        feat[i] *= f[i];
    }
    return;
}

static void* __calculate_frame_energy(void* frames, int frame_number, int frame_size) {
    double* energy = malloc(frame_number * sizeof(double));
    double (*ff)[frame_size]  = frames;
    for (int i=0; i<frame_number; ++i) {
        double sum = 0.f;
        for (int j=0; j<frame_size; ++j) {
            sum += ff[i][j];
        }
        energy[i] = sum;
    }
    return energy;
}


u_array_t create_mel_filterbank(int filter_n, int fft_n, int samplerate, int low_freq, int high_freq, pool_t* alloc)
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

    u_array_t filters = _UArray2d(alloc, filter_n, fft_n/2+1);
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

void* mfcc(double* raw, size_t raw_length, float frame_duration, float step_duration, int samplerate, int filter_n, int coe_n, int ceplifter, pool_t* alloc)
{  
    int frame_fftn;
    int frame_size;
    int frame_number;

    int low_freq = 0;
    int high_freq = samplerate / 2;

    // 做分帧 以及计算每一帧的能量 返回 426 * 129 的数组
    u_array_t frames = frames_pow_signale(raw, raw_length, frame_duration, step_duration, samplerate, &frame_fftn, &frame_size, &frame_number, NULL, alloc);

    //double* energy_data = __calculate_frame_energy(frame_data,frame_number, frame_fftn/2+1);

    // 获取梅尔滤波 返回 26 * 129 的二维数组。
    u_array_t filters = create_mel_filterbank(filter_n, frame_fftn, samplerate, low_freq, high_freq, alloc);
    
    u_array_t feat = UA_dot(&frames, UA_T(&filters));

    /*****************************************************************************************************/
    // 以下为旧代码：
    // // 申请一块空的内存装下 frame 与 filter 的内积。
    // void* feat_data   = malloc(frame_number*filter_n*sizeof(double));
    
    // // 
    // double (*dct_data)[coe_n] = malloc(frame_number * coe_n * sizeof(double));

    // DenseMatrix* frames  = DenseMatrix_wrap(frame_number, frame_fftn/2+1, frame_data);
    // DenseMatrix* filters = DenseMatrix_wrap(filter_n, frame_fftn/2+1, filter_data);
    // DenseMatrix* feat    = DenseMatrix_wrap(frame_number, filter_n, feat_data);

    // // 转置一下 filters
    // Matrix_trans(filters);

    // // fft * mel filter banks
    // DenseMatrix_product(frames, filters, feat);
    // DenseMatrix_foreach(feat, __log_feat);
    
    

    // DenseMatrix_elem_ptr(feat, pelem);

    // for (int i=0; i<Matrix_rows(feat); ++i) {
    //     // 每一帧做 log 
    //     for (int j=0; j<Matrix_cols(feat); ++j) {
    //         if (pelem[i][j] != 0.f) log(pelem[i][j]);
    //     }
    //     // 每一帧做 dct
    //     Discrete_cosine_transform(pelem[i], filter_n, coe_n, dct_data[i], dct_ortho, dct_ii);
    //     // 每一帧做 lifter
    //     __lifter(dct_data[i], coe_n, ceplifter);
    // }

    // // clean up memory
    // DenseMatrix_destroy(frames);
    // DenseMatrix_destroy(filters);
    // DenseMatrix_destroy(feat);

    // free(energy_data);
    // free(frame_data);
    // free(filter_data);
    // free(feat_data);
    // free(dct_data);
    return NULL;
    // 最后努力了，可以出梅尔系数了。我操尼玛的。
}