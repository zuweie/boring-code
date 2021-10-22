/*
 * @Author: zuweie
 * @Date: 2020-06-07 12:36:10
 * @LastEditTime: 2021-06-29 13:24:15
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
    CN_initialize(&ls, list, search_cmp, NULL, NULL); \
    ls;      \
})
#define List_(ls_ptr, cleanup) CN_uninitialize(ls_ptr, list, cleanup)

#endif