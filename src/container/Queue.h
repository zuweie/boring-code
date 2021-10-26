/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-14 19:09:21
 * @LastEditTime: 2021-10-26 09:24:38
 * @LastEditors: Please set LastEditors
 */
#ifndef __QUEUE_H__
#define __QUEUE_H__

#include "cn.h"
#include "base/__list.h"

typedef CN Queue;

#define Queue_create(T)        \
    ({                            \
        Queue q = CN_create(LIST,T); \
        q;                        \
    })
    
#define Queue_finalize(q, cleanup) CN_finalize(q, cleanup)

// 尾部插入
#define Queue_offer(q, ...) CN_add_at(q, CN_tail(q), __VA_ARGS__)

// 头部输出
#define Queue_poll(q, rdata) CN_rm_first(q, CN_first(q), rdata)

#endif