/*
 * @Author: your name
 * @Date: 2020-10-13 07:09:37
 * @LastEditTime: 2020-10-15 07:53:59
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/LinearArray.h
 */
#ifndef _VECTOR_2_H_
#define _VECTOR_2_H_

#include "Cn.h"
#include "base/__vector.h"

typedef Container Vector;

#define Vector_init(v, cmp) CN_initialize(v, vector, cmp)
#define Vector_uninit(v, cleanup) CN_uninitialize(v, vector, cleanup)

#endif
