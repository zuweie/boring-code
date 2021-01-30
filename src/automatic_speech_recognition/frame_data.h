/*
 * @Author: your name
 * @Date: 2021-01-30 00:10:20
 * @LastEditTime: 2021-01-30 14:34:01
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/automatic_speech_recognition/dataset.h
 */

#ifndef _FRAME_DATA_H_
#define _FRAME_DATA_H_
#include "mem_pool/__mem_pool.h"

typedef struct _frame_data {
    double* data;
    size_t dimen_1;
    size_t dimen_2;
    size_t dimen_3;
    /* maybe more dimen, only 3 now */
    pool_t* alloc;
} Frame_data_t;

#define _Frame_data_1d(d1, pool) \
    ({ \
        Frame_data_t fd; \
        fd.dimen_1 = d1; \
        fd.dimen_2 = 0; \
        fd.dimen_3 = 0; \
        fd.alloc = poll; \
        fd.data  = fd.alloc->allocate(fd.dimen_1 * sizeof(double)); \
        fd; \
    })

#define _Frame_data_2d(d1, d2, pool) \
    ({ \
        Frame_data_t fd; \
        fd.dimen_1 = d1; \
        fd.dimen_2 = d2; \
        fd.dimen_3 = 0; \
        fd.alloc = poll; \
        fd.data  = fd.alloc->allocate(fd.dimen_1 * fd.dimen_2 * sizeof(double)); \
        fd; \
    })
    
#define _Frame_data_3d(d1, d2, d3, pool) \
    ({ \
        Frame_data_t fd; \
        fd.dimen_1 = d1; \
        fd.dimen_2 = d2; \
        fd.dimen_3 = d3; \
        fd.alloc = poll; \
        fd.data  = fd.alloc->allocate(fd.dimen_1 * fd.dimen_2 * fd.dimen_3, sizeof(double)); \
        fd; \
    })
#endif