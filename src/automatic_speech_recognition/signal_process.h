/*
 * @Author: your name
 * @Date: 2021-01-21 11:28:28
 * @LastEditTime: 2021-01-26 10:36:35
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/mfcc/signal_process.h
 */
#ifndef _SIGNAL_PROCESS_H_
#define _SIGNAL_PROCESS_H_

#define calculate_fft_n(fl) \
    ({ \
        unsigned int fft_n = 1; \
        while((fft_n <<= 1) < (fl)); \
        fft_n; \
    })

double** frames_signale( \
    double* raw, \
    size_t raw_length, \
    float frame_duration, \
    float step_duration, \
    int samplerate, \
    int *frame_fftn, \
    int *frame_size, \
    int *frame_number, \
    int (*winfunc)(double* frame, int frame_size)\
    );

#endif