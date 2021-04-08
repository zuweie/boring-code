/*
 * @Author: your name
 * @Date: 2021-01-24 21:32:28
 * @LastEditTime: 2021-03-08 14:41:43
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/automatic_speech_recognition/sample_wav.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "simple_wav.h"

vfloat_t* Wav_load_signal(FILE* fp, wav_t* wav, char* raw, size_t raw_n, int* buffer_n) 
{
    *buffer_n = raw_n / (wav->fmt.bits_per_sample / 8);
    vfloat_t* buffer = malloc( (*buffer_n) * sizeof(vfloat_t) );
    int byte_n = wav->fmt.bits_per_sample / 8;
    char v[byte_n];
    int i,j;
    for(i=0, j=0; i<raw_n; i+=byte_n, ++j) {
        switch(byte_n) {
            case 2:
                v[0] = raw[i]; v[1] = raw[i+1];
                buffer[j] = *((uint16_t*)v);
                break;
            case 4:
                v[0] = raw[i]; v[1] = raw[i+1]; v[2] = raw[i+2]; v[3] = raw[i+3];
                buffer[j] = *((uint32_t*)v);
                break;
        }
    }
    return buffer;
}

int Wav_load(char *src_dir, wav_t* wav, vfloat_t **buffer, int *buffer_n) 
{
    int ret = -1;
    *buffer = NULL;
    FILE* fp = fopen(src_dir, "rb");
    if (fp) {
        fread(wav, 1, sizeof(wav_t), fp);
        
        // 1 calculate the data-block size:
        size_t raw_size = wav->riff.chunk_size + 8 - sizeof(wav_t);
        char* raw_buffer = malloc(raw_size);
        fread(raw_buffer, raw_size, 1, fp);
        *buffer = Wav_load_signal(fp, wav, raw_buffer, raw_size, buffer_n);
        free(raw_buffer);
        ret = 0;
    }
    fclose(fp);
    return ret;
}