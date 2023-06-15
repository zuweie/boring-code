/*
 * @Author: your name
 * @Date: 2021-10-07 20:11:11
 * @LastEditTime: 2021-11-03 14:57:41
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/base/type_value/__built_in_type.h
 */
#ifndef __BUILT_IN_TYPE_ADAPTERS_H__
#define __BUILT_IN_TYPE_ADAPTERS_H__
#include <stdarg.h>
#include "__type_value.h"
    
#define READ_VARGS(valist, pt, type) \
    type type_v = va_arg(valist, type); \
    *((type*)pt) = type_v \

#define BIT_CPY(p_dest, p_src, type) \
    *((type*)(p_dest)) = *((type*)(p_src))
    

int cmp_char(type_value_t*, type_value_t*);
int cmp_uchar(type_value_t*, type_value_t*);
int cmp_int(type_value_t*, type_value_t*);
int cmp_uint(type_value_t*, type_value_t*);
int cmp_long(type_value_t*, type_value_t*);
int cmp_ulong(type_value_t*, type_value_t*);
int cmp_float(type_value_t*, type_value_t*);
int cmp_double(type_value_t*, type_value_t*);
int cmp_str(type_value_t*, type_value_t*);
int cmp_ptr(type_value_t*, type_value_t*);

int hash_char(type_value_t*, int);
int hash_uchar(type_value_t*, int);
int hash_int(type_value_t*, int);
int hash_uint(type_value_t*, int);
int hash_long(type_value_t*, int);
int hash_ulong(type_value_t*, int);
int hash_float(type_value_t*, int);
int hash_double(type_value_t*, int);
int hash_str(type_value_t*, int);
int hash_ptr(type_value_t*, int);

int setup_char(type_value_t*, type_value_t*, unsigned char old);
int setup_uchar(type_value_t*, type_value_t*, unsigned char old);
int setup_int(type_value_t*, type_value_t*, unsigned char old);
int setup_uint(type_value_t*, type_value_t*, unsigned char old);
int setup_long(type_value_t*, type_value_t*, unsigned char old);
int setup_ulong(type_value_t*, type_value_t*, unsigned char old);
int setup_float(type_value_t*, type_value_t*, unsigned char old);
int setup_double(type_value_t*, type_value_t*, unsigned char old);
int setup_str(type_value_t*, type_value_t*, unsigned char old);
int setup_ptr(type_value_t*, type_value_t*, unsigned char old);

int vargs_char(va_list, type_value_t*, int);
int vargs_uchar(va_list, type_value_t*, int);
int vargs_int(va_list, type_value_t*, int);
int vargs_uint(va_list, type_value_t*, int);
int vargs_long(va_list, type_value_t*, int);
int vargs_ulong(va_list, type_value_t*, int);
int vargs_float(va_list, type_value_t*, int);
int vargs_double(va_list, type_value_t*, int);
int vargs_str(va_list, type_value_t*, int);
int vargs_ptr(va_list, type_value_t*, int);


#endif