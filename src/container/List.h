/*
 * @Author: zuweie
 * @Date: 2020-06-07 12:36:10
 * @LastEditTime: 2020-10-25 09:22:18
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /kitc/src/container/list.h
 */
#ifndef _LIST_2_H_
#define _LIST_2_H_

#include "Cn.h"
#include "base/__list.h"

typedef Container List;

#define List_init(ls, search_cmp, NULL, sort_cmp, wring_cmp, wring_cb, cleanup) \
    CN_initialize(ls, list, search_cmp, NULL, sort_cmp, wring_cmp, wring_cb, cleanup)

#define List_uninit(ls) CN_uninitialize(ls, list)

#endif