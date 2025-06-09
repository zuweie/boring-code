/*
 * @Author: your name
 * @Date: 2021-10-21 13:42:12
 * @LastEditTime: 2025-06-09 12:05:52
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/t.h
 */
#ifndef __TYP_H__
#define __TYP_H__
#include "base/type_value/__type_value.h"

#define T_type(T_ptr, type) type_value_(T_ptr, type)
#define T_char(T_ptr) T_type(T_ptr, char)
#define T_uchar(T_ptr) T_type(T_ptr, unsigned char)
#define T_short(T_ptr) T_type(T_ptr, short)
#define T_ushort(T_ptr) T_type(T_ptr, unsigned short)
#define T_int(T_ptr) T_type(T_ptr, int)
#define T_uint(T_ptr) T_type(T_ptr, unsigned int)
#define T_long(T_ptr) T_type(T_ptr, long)
#define T_ulong(T_ptr) T_type(T_ptr, unsigned long)
#define T_float(T_ptr) T_type(T_ptr, float)
#define T_double(T_ptr) T_type(T_ptr, double)
#define T_ptr(T_ptr) T_type(T_ptr, char*)
#define T_str(T_ptr) T_type(T_ptr, char*)

typedef type_value_t T;

#endif