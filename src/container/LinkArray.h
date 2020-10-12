/*
 * @Author: zuweie
 * @Date: 2020-06-07 12:36:10
 * @LastEditTime: 2020-10-12 12:11:36
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /kitc/src/container/list.h
 */
#ifndef _LINKARRAY_H_
#define _LINKARRAY_H_

#include "cn.h"
#include "base/__list.h"

typedef Container LinkArray;

#define LinkArr_init(la, compare) CN_init(la, list, compare)
#define LinkArr_free(la) CN_free(la, list)

#endif