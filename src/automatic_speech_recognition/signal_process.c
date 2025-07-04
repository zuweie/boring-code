/*
 * @Author: your name
 * @Date: 2021-01-21 11:28:35
 * @LastEditTime: 2025-06-03 13:09:39
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/mfcc/signal_process.c
 */
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include "fourier_transform/fourier_transform.h"
#include "signal_process.h"
#include "ultra_array/ultra_array.h"

static int hamwin_func(vfloat_t* frame, int frame_size)
{
    for(int i=0; i<frame_size; ++i) {
        vfloat_t ham = (vfloat_t) (0.54 - 0.46 * cos( 2 * 3.1415926535898 * ((vfloat_t) i / (vfloat_t)(frame_size-1)) ) );
        frame[i] *= ham;
    }
}
/**
 * 将原始数据切成一帧一帧并且，对每一帧做相关的预处理。
 * */

static u_array_t __do_processing_frames_raw(vfloat_t* raw, size_t raw_length, int frame_step_length, int frame_fft_n, int frame_size, int frame_number, float emphasis, int (*winfunc)(vfloat_t*, int))
{
    // 大于 512 位的傅立叶变换就不搞了。
    if (frame_fft_n > 512) return ua_unable;

    u_array_t frames = _UArray2d(frame_number, frame_fft_n/2+1);
    
    // 预加权， 不知道有啥用。
    for (int i=raw_length-1; i>=1; --i) {
        raw[i] = raw[i] - emphasis * raw[i-1];
    }
    // 申请哪一帧的做 fft 的时序内容... 
    vfloat_t sequence[frame_fft_n];
    // 因为是实数序列的输入，所以只需要 fft_n / 2 + 1 个复数保存复数数据。详情请搜索 实数时序的傅立叶变换。
    complex_t out[frame_fft_n / 2 + 1];

    int i,j,k;
    
    // 分帧开始：
    for (i=0; i<frame_number; ++i) {
        // 把每一帧的内存清空。
        memset(sequence, 0, frame_fft_n * sizeof(vfloat_t));
        // 把每一帧数据刷入。
        if ((frame_step_length*i+frame_size) <= raw_length) {
            memcpy(sequence, &raw[i*frame_step_length], sizeof(vfloat_t)*frame_size);
        } else if (frame_step_length*i < raw_length){
            memcpy(sequence, &raw[i*frame_step_length], sizeof(vfloat_t)*(raw_length-frame_step_length*i));
        } else {
            return ua_unable;
        }

        // 对每一帧做处理
        // 1 hamming 窗
        if (winfunc) 
            winfunc(sequence, frame_fft_n/2+1);
        
        // 2 做实数的 fft 。
        //Iterative_fast_fourier_transform(in, frame_fft_n, out, 0);
        Real_fast_fourier_transform_complex(sequence, frame_fft_n, out);
        
        // // 计算每一帧的 fft 能量。
        vfloat_t (*__frames)[frame_fft_n/2+1] = UA_data_ptr(&frames);

        for (k=0; k<frame_fft_n/2+1; ++k) {
            __frames[i][k] = 1.f / (float)(frame_fft_n) * complex_pow(out[k]);
        }
    }
    return frames;
}

/**
 * 分帧以及计算每一帧的能量。
*/
u_array_t frames_pow_signale(vfloat_t* raw, size_t raw_length, float frame_duration, float step_duration, int samplerate, int fft_n, float emphasis, int (*winfunc)(vfloat_t*, int)) 
{
    // 计所有的帧所需要的
    int frame_size = frame_duration * samplerate;
    int frame_step_length = step_duration * samplerate;
    int frame_number = 0;

    if (raw_length <= frame_size) {
        frame_number = 1;
    }else{
        frame_number = (int)floor((vfloat_t)raw_length / (vfloat_t)frame_step_length);
    }
    // 申请内存
    // 输入的信号为实数序列，做 fft 后只需要 每帧只需要 fft / 2 +1 位数据。

    return __do_processing_frames_raw(raw, raw_length, frame_step_length, fft_n, frame_size, frame_number, emphasis, winfunc);
}