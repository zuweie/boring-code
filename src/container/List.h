/*
 * @Author: zuweie
 * @Date: 2020-06-07 12:36:10
 * @LastEditTime: 2020-10-15 07:46:56
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /kitc/src/container/list.h
 */
#ifndef _LIST_2_H_
#define _LIST_2_H_

#include "Cn.h"
#include "base/__list.h"

typedef Container List;

#define List_init(l, compare) CN_initialize(l, list, compare)
#define List_uninit(l, cleanup) CN_uninitialize(l, list, cleanup)

#endif