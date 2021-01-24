/*
 * @Author: your name
 * @Date: 2021-01-24 21:32:45
 * @LastEditTime: 2021-01-25 00:08:11
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/automatic_speech_recognition/simple_wav.h
 */
#ifndef _SIMPLE_WAV_H_
#define _SIMPLE_WAV_H_

#include <stdint.h>

typedef struct _wav_riff {
    char chunk_id[4];
    uint32_t chunk_size;
    char format[4];
} wav_riff_t;

typedef struct _wav_format {
    char chunk_id[4];
    uint32_t sub_chunk1_size;
    uint16_t audio_format;
    uint16_t number_channels;
    uint32_t sample_rate;
    uint32_t byte_rate;
    uint16_t block_align;
    uint16_t bits_per_sample;
} wav_format_t;

typedef struct _wav_data {
    char chunk_id[4];
    uint32_t sub_chunk2_size;
} wav_data_t;

typedef struct wav {
    wav_riff_t riff;
    wav_format_t fmt;
    wav_data_t data;
} wav_t;

char* Simple_wav_load(FILE* fp, wav_t* wav);

#endif