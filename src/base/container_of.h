/*
 * @Author: your name
 * @Date: 2021-10-15 15:10:58
 * @LastEditTime: 2026-05-01 17:34:21
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/base/container_of.h
 */
#ifndef __CONTAINER_OF_H__
#define __CONTAINER_OF_H__
#define offset_of(TYPE, MEMBER) ((char*)&(((TYPE *)0)->MEMBER))

// ({}),这种语法，标准 C99 语言并不支持。换一种。
// #define container_of(ptr, TYPE, MEMBER) \
// ({ \
//     TYPE* type_ptr =  (((char*)ptr) - offset_of(TYPE, MEMBER)); \
//     type_ptr; \
// })

#define container_of(ptr, TYPE, MEMBER) ((void*)(((char*)(ptr)) - offset_of(TYPE, MEMBER)))
#endif