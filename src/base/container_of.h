/*
 * @Author: your name
 * @Date: 2021-10-15 15:10:58
 * @LastEditTime: 2021-10-15 15:12:59
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/base/container_of.h
 */
#ifndef __CONTAINER_OF_H__
#define __CONTAINER_OF_H__
#define offset_of(TYPE, MEMBER) ((size_t)&(((TYPE *)0)->MEMBER))
#define container_of(ptr, TYPE, MEMBER) \
({ \
    TYPE* type_ptr =  (((char*)ptr) - offset_of(TYPE, MEMBER)); \
    type_ptr; \
})
#endif