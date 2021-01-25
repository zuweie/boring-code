/*
 * @Author: your name
 * @Date: 2021-01-24 21:32:28
 * @LastEditTime: 2021-01-25 16:13:05
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/automatic_speech_recognition/sample_wav.c
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "simple_wav.h"

double* Wav_load_signal(FILE* fp, wav_t* wav, char* raw, int* buffer_n) 
{
    *buffer_n = wav->data.sub_chunk2_size / (wav->fmt.bits_per_sample / 8);
    double* buffer = malloc((*buffer_n) * sizeof(double));
    int byte_n = wav->fmt.bits_per_sample / 8;
    char v[byte_n];
    int i,j;
    for(i=0, j=0; i<wav->data.sub_chunk2_size; i+=byte_n, ++j) {
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

int Wav_load(char *src_dir, wav_t* wav, double **buffer, int *buffer_n) 
{
    //*buffer = NULL;
    FILE* fp = fopen(src_dir, "rb");
    if (fp) {
        fread(wav, 1, sizeof(wav_t), fp);
        char* raw = malloc(wav->data.sub_chunk2_size);
        fread(raw, wav->data.sub_chunk2_size, 1, fp);
        *buffer = Wav_load_signal(fp, wav, raw, buffer_n);
        free(raw);
        return 0;
    }
    fclose(fp);
    return -1;
}