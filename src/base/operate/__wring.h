/*
 * @Author: your name
 * @Date: 2020-10-17 18:50:29
 * @LastEditTime: 2020-10-17 22:47:04
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/base/operate/__wring.h
 */
#ifndef _WRING_H_
#define _WRING_H_
#include "base/__container.h"
#include "base/__type_value.h"

int wring(container_t*, int (*compare)(type_value_t, type_value_t), int (*callback)(void*));

#endif
