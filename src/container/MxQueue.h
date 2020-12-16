/*
 * @Author: your name
 * @Date: 2020-12-07 08:50:00
 * @LastEditTime: 2020-12-17 02:33:03
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/MxQueue.h
 */

#ifndef _MX_QUEUE_H_
#define _MX_QUEUE_H_
#include <stdio.h>
#include "Cn.h"
#include "base/__vector.h"
#include "base/operate/__heap_sort.h"

typedef Container MxQueue;


#define _MxQueue(sort_cmp) \
    ({ \
        MxQueue queue; \
        CN_initialize(queue, vector, NULL, NULL, NULL); \
        CN_set_extra_func(queue, sort_cmp); \
        queue; \
    })

#define MxQueue_(queue, cleanup) CN_uninitialize(queue, vector, cleanup)

#define MxQueue_extract(queue, __marco_rdata) \
    ({  \
        int ret = -1; \
        if (CN_size(queue) > 0) { \
            heap_build_max_heap(cc(queue), c_extra_func(queue)); \
            It first = CN_first(queue); \
            It last  = CN_last(queue); \
            It_exchange(first, last); \
            ret = CN_rm_last(queue, &__marco_rdata); \
        } else { \
            ret = -1; \
        } \
        ret; \
    })

#endif