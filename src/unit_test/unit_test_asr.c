/*
 * @Author: your name
 * @Date: 2021-01-24 03:03:52
 * @LastEditTime: 2021-01-28 15:29:09
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
#include "automatic_speech_recognition/simple_wav.h"

#define PRINTF_FRAME(frame, frame_row, frame_size) \
    ({ \
        for (int __ii=0; __ii<frame_size; ++__ii) { \
            printf("%lf ", frame[frame_row][__ii]); \
        } \
        printf("\n");\
    })

#define PRINTF_FRAMES(frames, frame_number, frame_size) \
    ({ \
        for (int i=0; i<frame_number; ++i) { \
            for (int j=0; j<frame_size; ++j) { \
                printf("%lf ", frames[i][j]); \
            } \
            printf("\n");\
        } \
    })
    
#define PRINTF_MEL_FILTER(filters, filter_number, filter_size) \
    ({ \
        for (int i=0; i<filter_number; ++i) { \
            printf("filter %d \n", i+1);\
            for (int j=0; j<filter_size; ++j) { \
                printf("%.2f ", filters[i*filter_size+j]); \
            } \
            printf("\n\n");\
        } \
    })

#define PRINTF_WAV_INFO(w) \
    ({ \
        wav_riff_t riff  = (w)->riff; \
        wav_format_t fmt = (w)->fmt; \
        wav_data_t data  = (w)->data; \
        printf("ChunkID %c%c%c%c\n", riff.chunk_id[0], riff.chunk_id[1],riff.chunk_id[2], riff.chunk_id[3]); \
        printf("ChunkSize %d\n", riff.chunk_size); \
        printf("Format %c%c%c%c\n", riff.format[0], riff.format[1], riff.format[2], riff.format[3]); \
        printf("\n"); \
        printf("Subchunk1ID %c%c%c%c\n", fmt.chunk_id[0], fmt.chunk_id[1], fmt.chunk_id[2], fmt.chunk_id[3]); \
        printf("Subchunk1size %d\n", fmt.sub_chunk1_size); \
        printf("AudioFomat %d\n", fmt.audio_format); \
        printf("NumberChannels %d\n", fmt.number_channels); \
        printf("SampleRate %d\n", fmt.sample_rate); \
        printf("ByteRate %d\n", fmt.byte_rate); \
        printf("BlockAlign %d\n", fmt.block_align); \
        printf("BitsPerSample %d\n", fmt.bits_per_sample); \
        printf("\n"); \
        printf("BlockID %c%c%c%c\n",data.chunk_id[0], data.chunk_id[1], data.chunk_id[2], data.chunk_id[3]); \
        printf("BlockSize %d \n", data.sub_chunk2_size); \
        printf("\n"); \
        printf("duration: %d \n", data.sub_chunk2_size / fmt.byte_rate); \
        printf("sizeof wav_t %d\n", sizeof(wav_t)); \
    })
    
#define PRINTF_WAV_BUFFER(buffer, buffer_n) \
    ({ \
        for(int i=0; i<buffer_n; ++i) { \
            printf("%.2f ", buffer[i]); \
            if ( (i+1) % 20 == 0) { \
                printf("\n"); \
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
    // double raw[] = {1,2,3,4,5,6,7,8,9,10,11,12,13,14,15,16,17};
    // int samplerate = 100;
    // float step_duration = 0.03f;
    // float frame_duration = 0.05f;
    wav_t w;
    double* buffer;
    int buffer_n;
    Wav_load("/Users/zuweie/code/c-projects/boring-code/build/english.wav", &w, &buffer, &buffer_n);

    int samplerate = w.fmt.sample_rate;
    float step_duration = 0.01f;
    float frame_duration = 0.025f;

    int frame_size;
    int frame_number;
    int frame_fftn;
    void *data = frames_signale(buffer, buffer_n, frame_duration, step_duration, samplerate, &frame_fftn, &frame_size, &frame_number, NULL);
    double (*frames)[frame_fftn/2+1] = data; 
    printf("\n raw frames data: \n");

    PRINTF_FRAME(frames, 0, (frame_fftn/2+1));
    printf("\n\n");
    PRINTF_FRAME(frames, 425, (frame_fftn/2+1));

    printf("fftn %d \n", frame_fftn);
    printf("frame_size %d \n", frame_size);
    printf("frame_number %d \n", frame_number);
    free(frames);
    return;
}

static void test_mffc (void) 
{
    wav_t w;
    double* buffer;
    int buffer_n;
    Wav_load("/Users/zuweie/code/c-projects/boring-code/build/english.wav", &w, &buffer, &buffer_n);

    int samplerate = w.fmt.sample_rate;
    float step_duration = 0.01f;
    float frame_duration = 0.025f;
    mfcc(buffer, buffer_n, frame_duration, step_duration, samplerate, 26, 13);
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
    printf("\n filters :\n");
    PRINTF_MEL_FILTER(filters, filter_n, (fft_n/2+1));
    printf("\n\n");
    free(filters);
    return;
}

static void test_wav_load(void) 
{
    wav_t w;
    double* buffer;
    int buffer_n;
    Wav_load("/Users/zuweie/code/c-projects/boring-code/build/english.wav", &w, &buffer, &buffer_n);
    printf("\n wav info: \n");
    PRINTF_WAV_INFO(&w);
    printf("\n signal info: \n");
    PRINTF_WAV_BUFFER(buffer, buffer_n);
    if (buffer) free(buffer);
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
    
    // if (NULL == CU_add_test(pSuite, "test frame signal", test_frames_signal) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "test mel filter", test_filter_bank) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "test wav info", test_wav_load) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // final test
    if (NULL == CU_add_test(pSuite, "test mffc", test_mffc) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
}