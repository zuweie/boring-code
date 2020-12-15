/*
 * @Author: your name
 * @Date: 2020-12-07 08:50:00
 * @LastEditTime: 2020-12-13 11:08:38
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

typedef struct {
    
    Container con;
    short build_max_heap;

} MxQueue; 

#define _MxQueue(sort_cmp) \
    ({ \
        MxQueue queue; \
        CN_initialize(queue.con, vector, NULL, NULL, NULL); \
        CN_set_extra_func(queue.con, sort_cmp); \
        queue.build_max_heap = 0; \
        queue; \
    })

#define MxQueue_(queue, cleanup) CN_uninitialize(queue.con, vector, cleanup)

#define MxQueue_extract(queue, __marco_rdata) \
    ({  \
        int ret = -1; \
        if (CN_size(queue.con) >= 0) { \
            if (!queue.build_max_heap) {\
                heap_build_max_heap(cc(queue.con), c_extra_func(queue.con)); \
                queue.build_max_heap = 1; \
            } else { \
                heap_max_heapify(cc(queue.con), 0, CN_size(cc(queue.con)), c_extra_func(queue.con)); \
            } \
            It first = CN_first(queue.con); \
            It last  = CN_last(queue.con); \
            It_exchange(first, last); \
            ret = CN_rm_last(queue.con, &__marco_rdata); \
        } else { \
            ret = -1; \
        } \
        ret; \
    })

#define MxQueue_add(queue, data) \
    ({ \
        CN_add(queue.con, data);  \
        queue.build_max_heap = 0; \
    })

#define MXQueue_reset(queue) (queue.build_max_heap=0)
#endif