/*
 * @Author: your name
 * @Date: 2021-03-09 15:46:55
 * @LastEditTime: 2021-10-24 09:18:34
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/automatic_speech_recognition/compare_mfcc.h
 */
#ifndef __COMPARE_MFCC_H__
#define __COMPARE_MFCC_H__

typedef struct _u_array u_array_t;

u_array_t compare_mfcc_cosine(u_array_t* mfcc1, u_array_t* mfcc2);
u_array_t compare_mfcc_distance(u_array_t* mfcc1, u_array_t* mfcc2);
u_array_t compare_mfcc_pearson(u_array_t* mfcc1, u_array_t* mfcc2);
#endif