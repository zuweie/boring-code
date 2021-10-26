/*
 * @Author: your name
 * @Date: 2020-12-07 08:50:00
 * @LastEditTime: 2021-10-26 09:41:25
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/MxQueue.h
 */

#ifndef __MX_QUEUE_H__
#define __MX_QUEUE_H__
#include <stdio.h>
#include "Cn.h"
#include "base/__vector.h"
#include "base/operate/__heap_sort.h"

typedef CN MxQueue;


#define MxQueue_create(T, sort_cmp) \
    ({ \
        MxQueue queue = CN_create(VECTOR|customized_compare, T, sort_cmp); \
        queue; \
    })

#define MxQueue_finalize(q, cleanup) CN_finalize(q, cleanup)

#define MxQueue_extract(q, rdata, cmp) CN_mx_extract(q, rdata, cmp)

#endif