/*
 * @Author: your name
 * @Date: 2021-01-21 11:28:28
 * @LastEditTime: 2025-06-03 13:10:43
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/mfcc/signal_process.h
 */
#ifndef __SIGNAL_PROCESS_H__
#define __SIGNAL_PROCESS_H__
#include "ultra_array/ultra_array.h"
#include "vtype/vfloat_type.h"

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

static inline unsigned int calculate_fft_n(unsigned int fl)
{
    unsigned int fft_n = 1; 
    while((fft_n <<= 1) < (fl)); 
    return fft_n; 
}
// unsigned int calculate_fft_n(unsigned int fl);

#endif