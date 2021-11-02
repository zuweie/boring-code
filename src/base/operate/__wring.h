/*
 * @Author: your name
 * @Date: 2020-10-17 18:50:29
 * @LastEditTime: 2021-11-01 17:39:21
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/base/operate/__wring.h
 */
#ifndef __WRING_H__
#define __WRING_H__

int wring(container_t*, int (*compare)(type_value_t*, type_value_t*, int context), int (*callback)(void*));

#endif
