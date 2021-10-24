/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-14 19:09:21
 * @LastEditTime: 2021-10-24 09:18:00
 * @LastEditors: Please set LastEditors
 */
#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "cn.h"
#include "base/__list.h"

typedef Container Queue;

#define _Queue(search_cmp) \
    ({                     \
        Queue q;           \
        CN_initialize(&q, list, search_cmp, NULL, NULL); \
        q;                 \
    })
    
#define Queue_(queue_ptr, cleanup) CN_uninitialize(queue_ptr, list, cleanup)

// 尾部插入
#define Queue_offer(queue_ptr, tv) CN_add_tail(queue_ptr, tv)

// 头部输出
#define Queue_poll(queue_ptr, rdata) CN_rm_first(queue_ptr, rdata)

#endif