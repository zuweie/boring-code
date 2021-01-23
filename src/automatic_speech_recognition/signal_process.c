/*
 * @Author: your name
 * @Date: 2021-01-21 11:28:35
 * @LastEditTime: 2021-01-24 03:02:54
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/mfcc/signal_process.c
 */
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "fourier_transform/fourier_transform.h"
#include "signal_process.h"

#define emphasis 0.95

/**
 * 将原始数据切成一帧一帧并且，对每一帧做相关的预处理。
 * */

static int __do_processing_frames_raw(double* raw, size_t raw_length, int frame_step_length, int frame_fft_n, int frame_size, int frame_number, double (*frames)[frame_fft_n])
{
    // 大于 512 位的傅立叶变换就不搞了。
    if (fft_n > 512) return -1;

    // 预加权， 不知道有用。
    for (int i=1; i<n; ++i) {
        raw[i] = raw[i] - emphasis * raw[i-1];
    }

    // 计算一帧数据汉明窗的参数。
    double hamming[frame_size];
    for (int i=0; i<frame_size; ++i) {
        hamwin[i] = (double)(0.54 - 0.46 * cos(2 * pi * (double) i/ ((double)(frame_size-1))));
    }
    
    // 开始分家这个有排调试
    // 做汉明窗，计算 fft 能量。
    complex_t in[frame_fft_n];
    complex_t out[frame_fft_n];
    int i,j,k;
    for (i=0; i<frame_number; ++i) {
        
        // 把每一帧数据刷入。
        memcpy(frames[i], &raw[i*frame_step_length], sizeof(double)*frame_size);
        // 对每一帧做处理
        // 1 hamming 窗
        for (j=0; j<fft_n; ++j) {
            if (j <= frame_size) {
                frames[i][j] *= hamming[j];
            }
            in[j] = _complex(frames[i][j], 0.f);
        }
        // 2 做fft 能量
        Iterative_fast_fourier_transform(in, fft_n, out, 0);

        // 计算每一帧的 fft 能量。
        for (k=0; k<frame_size; ++k) {
            frames[i][k] = 1.f / (float)(fft_n) * complex_pow(out[k]);
        }
    }
    return 0;
}

double** frames_signale(double* raw, size_t raw_length, float frame_duration, float step_duration, int samplerate, int *frame_size, int *frame_number) 
{
    // 计算所有的帧所需要的
    *frame_size = frame_duration * raw_length;
    int frame_step_length = step_duration * raw_length;
    int fft_n = calculate_fft_n(*frame_size);
    
    // 
    if (raw_length <= *frame_size) {
        *frame_number = 1;
    }else{
        *frame_number = (int)ceil( (double)raw_length / (double)frame_step_length );
    }

    double** frames = malloc((*frame_number) * (*frame_size) * sizeof(double));
    __do_processing_frames_raw(raw, raw_length, frame_step_length, fft_n, *frame_size, *frame_number, frames);
    return frames;
}