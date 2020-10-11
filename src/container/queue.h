/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-14 19:09:21
 * @LastEditTime: 2020-06-06 10:20:59
 * @LastEditors: Please set LastEditors
 */
#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "cn.h"
//#include "__list.h"

typedef Container Queue;

#define Queue_init(queue, compare) CN_init(queue, list, compare)
#define Queue_free(queue) CN_free(queue, list)

// 尾部插入
#define Queue_offer(queue, tv) CN_add_tail(queue, tv)

// 头部输出
#define Queue_poll(queue, rdata) CN_rm_first(queue, rdata)

#endif