/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-14 19:09:21
 * @LastEditTime: 2020-10-15 07:43:21
 * @LastEditors: Please set LastEditors
 */
#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "Cn.h"
#include "base/__list.h"

typedef Container Queue;

#define Queue_init(queue, compare) CN_initialize(queue, list, compare)
#define Queue_uninit(queue, cleanup) CN_uninitialize(queue, list, cleanup)

// 尾部插入
#define Queue_offer(queue, tv) CN_add_tail(queue, tv)

// 头部输出
#define Queue_poll(queue, rdata) CN_rm_first(queue, rdata)

#endif