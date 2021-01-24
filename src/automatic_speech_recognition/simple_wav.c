/*
 * @Author: your name
 * @Date: 2021-01-24 21:32:28
 * @LastEditTime: 2021-01-25 00:24:01
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/automatic_speech_recognition/sample_wav.c
 */
#include <stdlib.h>
#include "simple_wav.h"

char* Simple_wav_load(char src_dir[], wav_t* wav) 
{
    FILE* fp = fopen(src_dir, "rb");
    fread(wav, 1, sizeof(wav_t), fp);
    return NULL;
}