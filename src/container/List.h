/*
 * @Author: zuweie
 * @Date: 2020-06-07 12:36:10
 * @LastEditTime: 2020-10-13 10:39:21
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /kitc/src/container/list.h
 */
#ifndef _LIST_2_H_
#define _LIST_2_H_

#include "Cn.h"
#include "base/__list.h"

typedef Container List;

#define List_init(l, compare) CN_init(l, list, compare)
#define List_free(l) CN_free(l, list)

#endif