/*
 * @Author: your name
 * @Date: 2020-12-07 08:50:00
 * @LastEditTime: 2025-06-09 12:17:10
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/MxQueue.h
 */

#ifndef __MX_QUEUE_H__
#define __MX_QUEUE_H__
#include <stdio.h>
//#include "base/__vector.h"
//#include "base/operate/__heap_sort.h"
#include "Cont.h"

typedef CN MxQueue;


#define MxQueue_create(T, sort_cmp) \
    ({ \
        MxQueue queue = CN_create(VECTOR|customized_compare, T, sort_cmp); \
        queue; \
    })

#define MxQueue_finalize(q, cleanup) CN_finalize(q, cleanup)

#define MxQueue_extract(q, prdata) CN_extract(q, prdata, NULL)

#endif