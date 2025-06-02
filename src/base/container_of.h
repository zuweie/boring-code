/*
 * @Author: your name
 * @Date: 2021-10-15 15:10:58
 * @LastEditTime: 2025-06-02 10:03:59
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/base/container_of.h
 */
#ifndef __CONTAINER_OF_H__
#define __CONTAINER_OF_H__
#define offset_of(TYPE, MEMBER) ((char*)&(((TYPE *)0)->MEMBER))

// ({}),这种语法，标准 C 语言并不支持。换一种。
// #define container_of(ptr, TYPE, MEMBER) \
// ({ \
//     TYPE* type_ptr =  (((char*)ptr) - offset_of(TYPE, MEMBER)); \
//     type_ptr; \
// })

#define container_of(ptr, TYPE, MEMBER) ((void*)(((char*)(ptr)) - offset_of(TYPE, MEMBER)))
#endif