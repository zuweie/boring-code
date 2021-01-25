/*
 * @Author: your name
 * @Date: 2021-01-21 11:28:35
 * @LastEditTime: 2021-01-26 00:37:12
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

static int __do_processing_frames_raw(double* raw, size_t raw_length, int frame_step_length, int frame_fft_n, int frame_size, int frame_number, double frames[][frame_fft_n])
{
    // 大于 512 位的傅立叶变换就不搞了。
    if (frame_fft_n > 512) return -1;

    // 预加权， 不知道有啥用。
    for (int i=1; i<raw_length; ++i) {
        raw[i] = raw[i] - emphasis * raw[i-1];
    }

    // 计算一帧数据汉明窗的参数。
    double hamwin[frame_size];
    for (int i=0; i<frame_size; ++i) {
        hamwin[i] = (double)(0.54 - 0.46 * cos(2 * 3.1415926535898 * (double) i/ ((double)(frame_size-1))));
    }
    
    // 开始分家这个有排调试
    // 做汉明窗，计算 fft 能量。
    complex_t in[frame_fft_n];
    complex_t out[frame_fft_n];
    int i,j,k;
    for (i=0; i<frame_number; ++i) {
        
        // 把每一帧数据刷入。
        if ((frame_step_length*i+frame_size) <= raw_length) {
            memcpy(frames[i], &raw[i*frame_step_length],sizeof(double)*frame_size);
        } else if (frame_step_length*i < raw_length){
            memcpy(frames[i], &raw[i*frame_step_length],sizeof(double)*(raw_length-frame_step_length*i));
        } else {
            // frame_step_length * i >= raw_length means calculate frame wrongly.
            return -1;
        }
        // 对每一帧做处理
        // 1 hamming 窗
        for (j=0; j<frame_fft_n; ++j) {
            if (j < frame_size) {
                frames[i][j] *= hamwin[j];
                in[j] = _complex(frames[i][j], 0.f);
            } else {
                // 剩下的凑数。
                in[j] = _complex(0.f, 0.f);
            }
            
        }
        // 2 做fft 能量
        Iterative_fast_fourier_transform(in, frame_fft_n, out, 0);

        // 计算每一帧的 fft 能量。
        for (k=0; k<frame_size; ++k) {
            frames[i][k] = 1.f / (float)(frame_fft_n) * complex_pow(out[k]);
        }
    }
    return 0;
}

double** frames_signale(double* raw, size_t raw_length, float frame_duration, float step_duration, int samplerate, int *frame_fftn, int *frame_size, int *frame_number) 
{
    // 计算所有的帧所需要的
    *frame_size = frame_duration * samplerate;
    int frame_step_length = step_duration * samplerate;
    *frame_fftn = calculate_fft_n(*frame_size);
    // 
    if (raw_length <= *frame_size) {
        *frame_number = 1;
    }else{
        *frame_number = (int)floor((double)raw_length / (double)frame_step_length);
    }
    // 申请内存
    double** frames = malloc((*frame_number) * (*frame_size) * sizeof(double));
    __do_processing_frames_raw(raw, raw_length, frame_step_length, *frame_fftn, *frame_size, *frame_number, frames);
    
    return frames;
}