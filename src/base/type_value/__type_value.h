/*
 * @Author: your name
 * @Date: 2021-10-07 20:10:29
 * @LastEditTime: 2021-10-27 13:36:25
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/base/type_value/__type_value.h
 */
#ifndef __TYPE_VALUE_H__
#define __TYPE_VALUE_H__
#define type_value_cpy(pty1, pty2, ty_size) \
({ \
    type_value_t* ty1 = (pty1); \
    type_value_t* ty2 = (pty2); \
    int size = (ty_size);       \
    while(--size) *ty1++ = *ty2++; \
})

#define type_value_swap(pty1, pty2, ty_size) \
({ \
    type_value_t* ty1 = (pty1); \
    type_value_t* ty2 = (pty2); \
    int size = (ty_size); \
    while (--size) \
    { \
        (*ty1) = (*ty1)^(*ty2); \
        (*ty2) = (*ty1)^(*ty2); \
        (*ty1) = (*ty1)^(*ty2); \
    } \
})
#define type_value_(ty_ptr, type) *((type*)(ty_ptr))
typedef char type_value_t;

#endif