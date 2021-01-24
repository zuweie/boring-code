/*
 * @Author: your name
 * @Date: 2021-01-24 03:03:52
 * @LastEditTime: 2021-01-24 15:41:33
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/unit_test_ars.c
 */
#include <stdlib.h>
#include <stdio.h>
#include <CUnit/Basic.h>
#include "test_data.h"
#include "automatic_speech_recognition/signal_process.h"
#include "automatic_speech_recognition/mfcc.h"

#define PRINTF_FRAMES(frames, frame_number, frame_size) \
    ({ \
        for (int i=0; i<frame_number; ++i) { \
            for (int j=0; j<frame_size; ++j) { \
                printf("%lf ", frames[i*frame_size+j]); \
            } \
            printf("\n"); \
        } \
    })
    
#define PRINTF_MEL_FILTER(filters, filter_number, filter_size) \
    ({ \
        for (int i=0; i<filter_number; ++i) { \
            for (int j=0; j<filter_size; ++j) { \
                printf("%lf ", filters[i*filter_size+j]); \
            } \
        } \
    })

static int  suite_success_init (void) 
{
    printf("\nasr suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("\nasr suite success clean\n");
}

static void test_frames_signal (void) 
{
    double raw[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17};
    int samplerate = 100;
    float step_duration = 0.03f;
    float frame_duration = 0.05f;
    int frame_size;
    int frame_number;
    int frame_fftn;
    double* frames = frames_signale(raw, 17, frame_duration, step_duration, 100, &frame_fftn, &frame_size, &frame_number);
    printf("\n raw frames data: \n");
    PRINTF_FRAMES(frames, frame_number, frame_fftn);
    printf("fftn %d \n", frame_fftn);
    printf("frame_size %d \n", frame_size);
    printf("frame_number %d \n", frame_number);
    free(frames);
    return;
}
static void test_filter_bank (void) 
{
    int filter_n = 26;
    int fft_n = 256;
    int samplerate = 8000;
    int low_freq = 0;
    int high_freq = samplerate / 2;

    double* filters = create_mel_filtebank(filter_n, fft_n, samplerate, low_freq, high_freq);
    PRINTF_MEL_FILTER(filters, filter_n, (fft_n/2+1));
    free(filters);
    return;
}
int do_asr_test (void) 
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("Test asr Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if (NULL == CU_add_test(pSuite, "test frame signal", test_frames_signal) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test mel filter", test_filter_bank) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
}