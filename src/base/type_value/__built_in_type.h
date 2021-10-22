/*
 * @Author: your name
 * @Date: 2021-10-07 20:11:11
 * @LastEditTime: 2021-10-22 13:24:57
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/base/type_value/__built_in_type.h
 */
#ifndef __BUILT_IN_TYPE_H__
#define __BUILT_IN_TYPE_H__
#include <stdarg.h>
#include "__type_value.h"
    
#define READ_VARGS(valist, type, pt) \
({  \
    type type_v = va_arg(valist, type); \
    *((type*)pt) = type_v; \
})

#define WRITE_VARGS(valist, type, pt) \
({  \
    char* dest = va_arg(valist, char*); \
    *((type*)dest) = *((type*)pt); \
})

#define BIT_CPY(p_dest, p_src, type) \
({  \
    *((type*)p_dest) = *((type*)p_src); \
})

enum {
    char_t=1, 
    uchar_t, 
    short_t, 
    ushort_t, 
    int_t, 
    uint_t, 
    long_t, 
    ulong_t, 
    fl_t, 
    db_t, 
    str_t, 
    ptr_t
};

int cmp_char(type_value_t* t1, type_value_t* t2);
int cmp_uchar(type_value_t* t1, type_value_t* t2);
int cmp_short(type_value_t* t1, type_value_t* t2);
int cmp_ushort(type_value_t* t1, type_value_t* t2);
int cmp_int(type_value_t* t1, type_value_t* t2);
int cmp_uint(type_value_t* t1, type_value_t* t2);
int cmp_long(type_value_t* t1, type_value_t* t2);
int cmp_ulong(type_value_t* t1, type_value_t* t2);
int cmp_flv(type_value_t* t1, type_value_t* t2);
int cmp_dbl(type_value_t* t1, type_value_t* t2);
int cmp_str(type_value_t* t1, type_value_t* t2);
int cmp_ptr(type_value_t* t1, type_value_t* t2);

int hash_char(type_value_t*, int);
int hash_uchar(type_value_t*, int);
int hash_short(type_value_t*, int);
int hash_ushort(type_value_t*, int);
int hash_int(type_value_t*, int);
int hash_uint(type_value_t*, int);
int hash_long(type_value_t*, int);
int hash_ulong(type_value_t*, int);
int hash_float(type_value_t*, int);
int hash_double(type_value_t*, int);
int hash_str(type_value_t*, int);
int hash_ptr(type_value_t*, int);

void read_vargs_char(va_list, type_value_t*);
void read_vargs_uchar(va_list, type_value_t*);
void read_vargs_short(va_list, type_value_t*);
void read_vargs_ushort(va_list, type_value_t*);
void read_vargs_int(va_list, type_value_t*);
void read_vargs_uint(va_list, type_value_t*);
void read_vargs_long(va_list, type_value_t*);
void read_vargs_ulong(va_list, type_value_t*);
void read_vargs_float(va_list, type_value_t*);
void read_vargs_double(va_list, type_value_t*);
void read_vargs_str(va_list, type_value_t*);
void read_vargs_ptr(va_list, type_value_t*);

void write_vargs_char(va_list, type_value_t*);
void write_vargs_uchar(va_list, type_value_t*);
void write_vargs_short(va_list, type_value_t*);
void write_vargs_ushort(va_list, type_value_t*);
void write_vargs_int(va_list, type_value_t*);
void write_vargs_uint(va_list, type_value_t*);
void write_vargs_long(va_list, type_value_t*);
void write_vargs_ulong(va_list, type_value_t*);
void write_vargs_float(va_list, type_value_t*);
void write_vargs_double(va_list, type_value_t*);
void write_vargs_str(va_list, type_value_t*);
void write_vargs_ptr(va_list, type_value_t*);

void bit_cpy_char(type_value_t*, type_value_t*);
void bit_cpy_uchar(type_value_t*, type_value_t*);
void bit_cpy_short(type_value_t*, type_value_t*);
void bit_cpy_ushort(type_value_t*, type_value_t*);
void bit_cpy_int(type_value_t*, type_value_t*);
void bit_cpy_uint(type_value_t*, type_value_t*);
void bit_cpy_long(type_value_t*, type_value_t*);
void bit_cpy_ulong(type_value_t*, type_value_t*);
void bit_cpy_float(type_value_t*, type_value_t*);
void bit_cpy_double(type_value_t*, type_value_t*);
void bit_cpy_str(type_value_t*, type_value_t*);
void bit_cpy_ptr(type_value_t*, type_value_t*);


#endif