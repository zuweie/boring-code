/*
 * @Author: your name
 * @Date: 2020-12-07 08:50:00
 * @LastEditTime: 2021-06-29 15:31:04
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
        CN_initialize(&queue, vector, NULL, NULL, NULL); \
        CN_set_extra_func(&queue, sort_cmp); \
        queue; \
    })

#define MxQueue_(queue_ptr, cleanup) CN_uninitialize(queue_ptr, vector, cleanup)

#define MxQueue_extract(queue_ptr, __marco_rdata) \
    ({  \
        int ret = -1; \
        if (CN_size(queue_ptr) > 0) { \
            heap_build_max_heap(c_base(queue_ptr), c_extra_func(queue_ptr)); \
            It first = CN_first(queue_ptr); \
            It last  = CN_last(queue_ptr); \
            It_exchange(first, last); \
            ret = CN_rm_last(queue_ptr, &__marco_rdata); \
        } else { \
            ret = -1; \
        } \
        ret; \
    })

#endif