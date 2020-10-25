/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-14 19:09:21
 * @LastEditTime: 2020-10-24 23:38:32
 * @LastEditors: Please set LastEditors
 */
#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "Cn.h"
#include "base/__list.h"

typedef Container Queue;

#define Queue_init(queue, search_cmp, sort_cmp, wring_cmp, wring_cb, cleanup) \
    CN_initialize(ls, list, search_cmp, NULL, sort_cmp, wring_cmp, wring_cb, cleanup)
    
#define Queue_uninit(queue) CN_uninitialize(queue, list)

// 尾部插入
#define Queue_offer(queue, tv) CN_add_tail(queue, tv)

// 头部输出
#define Queue_poll(queue, rdata) CN_rm_first(queue, rdata)

#endif