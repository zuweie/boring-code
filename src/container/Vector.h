/*
 * @Author: your name
 * @Date: 2020-10-13 07:09:37
 * @LastEditTime: 2020-10-13 07:29:17
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/LinearArray.h
 */
#ifndef _VECTOR_2_H_
#define _VECTOR_2_H_

#include "cn.h"
#include "base/__vector.h"

typedef Container Vector;

#define Vector_init(v, cmp) CN_init(v, vector, cmp)
#define Vector_free(v) CN_free(v, vector)

#endif
