/*
 * @Author: your name
 * @Date: 2020-10-13 07:09:37
 * @LastEditTime: 2020-10-24 20:14:04
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/LinearArray.h
 */
#ifndef _VECTOR_2_H_
#define _VECTOR_2_H_

#include "Cn.h"
#include "base/__vector.h"

typedef Container Vector;

#define Vector_init(vet, search_cmp, sort_cmp, wring_cmp, wring_cb, cleanup) CN_initialize(vet, vector, search_cmp, NULL, sort_cmp, wring_cmp, wring_cb, cleanup)
#define Vector_uninit(vet) CN_uninitialize(vet, vector)

#endif
