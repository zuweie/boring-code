/*
 * @Author: your name
 * @Date: 2020-10-17 18:50:29
 * @LastEditTime: 2020-10-18 09:51:08
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/base/operate/__wring.h
 */
#ifndef _WRING_H_
#define _WRING_H_

int wring(container_t*, int (*compare)(type_value_t, type_value_t), int (*callback)(void*));

#endif
