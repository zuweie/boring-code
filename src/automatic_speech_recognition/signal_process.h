/*
 * @Author: your name
 * @Date: 2021-01-21 11:28:28
 * @LastEditTime: 2021-04-08 15:07:10
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/mfcc/signal_process.h
 */
#ifndef _SIGNAL_PROCESS_H_
#define _SIGNAL_PROCESS_H_
#include "ultra_array/ultra_array.h"
#include "boring_type/vfloat_type.h"
#define calculate_fft_n(fl) \
    ({ \
        unsigned int fft_n = 1; \
        while((fft_n <<= 1) < (fl)); \
        fft_n; \
    })

u_array_t frames_pow_signale( \
    vfloat_t* raw, \
    size_t raw_length, \
    float frame_duration, \
    float step_duration, \
    int samplerate, \
    int fft_n, \
    float emphasis, \
    int (*winfunc)(vfloat_t* frame, int frame_size)\
    );

#endif