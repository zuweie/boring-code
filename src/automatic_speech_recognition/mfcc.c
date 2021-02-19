/*
 * @Author: your name
 * @Date: 2021-01-12 07:19:35
 * @LastEditTime: 2021-02-19 14:22:46
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



int f_bank(double* raw, size_t raw_length, float frame_duration, float step_duration, int samplerate, int filter_n,  int fft_n, float emphasis, u_array_t* feat, u_array_t* energy)
{  
    int low_freq = 0;
    int high_freq = samplerate / 2;

    // 做分帧 以及计算每一帧的能量 返回 426 * 129 的数组
    u_array_t frames = frames_pow_signale(raw, raw_length, frame_duration, step_duration, samplerate, fft_n, emphasis, NULL);
    *energy = UA_copy(&frames);
    UA_sum(energy, 1);

    //double* energy_data = __calculate_frame_energy(frame_data,frame_number, frame_fftn/2+1);

    // 获取梅尔滤波 返回 26 * 129 的二维数组。 26 为 梅尔滤波的个数。filter_n 传入。
    u_array_t filters = create_mel_filterbank(filter_n, fft_n, samplerate, low_freq, high_freq);

    // frames dot filters 后 为 426 X 26 的 数组。
    *feat = UA_dot(&frames, UA_T(&filters));
    UA_where(feat, == 0.f, 0.001f);

    UArray_(&frames);
    UArray_(&filters);
    return 0;
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
    // 最后努力了，可以出梅尔系数了。我操尼玛的。
}

u_array_t mfcc(double* raw, size_t raw_len, int samplerate, float win_len, \
        float win_step, int cep_n, int filter_n, int fft_n, int freq_low, \
        int freq_high, float preemph, int cep_lifter, int append_energy) 
{
    // 计算一帧所要的 fft_n 的点数。
    fft_n = fft_n == 0 ? calculate_fft_n(win_len * samplerate) : fft_n;
    u_array_t feat, energy;
    f_bank(raw, raw_len, win_len, win_step, samplerate, filter_n, fft_n, preemph, &feat, &energy);
    UA_log(&feat);

    __dct(&feat, cep_n);
    
    char router[128] = {'\0'};
    sprintf(router, ":,:%d", cep_n);
    u_array_t feat1 = UA_fission(&feat, router); 

    __lifter(&feat1, cep_lifter);
    if (append_energy) {
        UA_assimilate(&feat1, ":,0", &energy);
    }
    
    UArray_(&feat);
    UArray_(&energy);
    return feat1;
}
