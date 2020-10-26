/*
 * @Author: zuweie
 * @Date: 2020-06-07 12:36:10
 * @LastEditTime: 2020-10-26 22:59:13
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /kitc/src/container/list.h
 */
#ifndef _LIST_2_H_
#define _LIST_2_H_

#include "Cn.h"
#include "base/__list.h"

typedef Container List;

#define _List(search_cmp) \
({           \
    List ls; \
    CN_initialize(ls, list, search_cmp, NULL, NULL); \
    ls;      \
})
#define List_(ls, cleanup) CN_uninitialize(ls, list, cleanup)

#endif