/*
 * @Author: your name
 * @Date: 2021-10-07 20:10:29
 * @LastEditTime: 2025-06-02 10:03:31
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/base/type_value/__type_value.h
 */
#ifndef __TYPE_VALUE_H__
#define __TYPE_VALUE_H__
#define type_value_cpy(pty1, pty2, ty_size) \
do { \
    type_value_t* ty1 = (pty1); \
    type_value_t* ty2 = (pty2); \
    int size = (ty_size);       \
    while(size--) *ty1++ = *ty2++; \
} while(0)

#define type_value_swap(pty1, pty2, ty_size) \
do { \
    type_value_t* ty1 = (pty1); \
    type_value_t* ty2 = (pty2); \
    int size = (ty_size); \
    while (ty1 != ty2 && size--) \
    { \
        (*ty1) = (*ty1)^(*ty2); \
        (*ty2) = (*ty1)^(*ty2); \
        (*ty1) = (*ty1)^(*ty2); \
        ty1++;\
        ty2++;\
    } \
} while(0)

#define type_value_(ty_ptr, type) *((type*)(ty_ptr))

typedef char type_value_t;

#endif