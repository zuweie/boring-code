/*
 * @Author: your name
 * @Date: 2021-10-07 20:11:11
 * @LastEditTime: 2021-10-08 15:50:25
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

int cmp_char(T* t1, T* t2);
int cmp_uchar(T* t1, T* t2);
int cmp_short(T* t1, T* t2);
int cmp_ushort(T* t1, T* t2);
int cmp_int(T* t1, T* t2);
int cmp_uint(T* t1, T* t2);
int cmp_long(T* t1, T* t2);
int cmp_ulong(T* t1, T* t2);
int cmp_flv(T* t1, T* t2);
int cmp_dbl(T* t1, T* t2);
int cmp_str(T* t1, T* t2);
int cmp_ptr(T* t1, T* t2);

int hash_char(T*, int);
int hash_uchar(T*, int);
int hash_short(T*, int);
int hash_ushort(T*, int);
int hash_int(T*, int);
int hash_uint(T*, int);
int hash_long(T*, int);
int hash_ulong(T*, int);
int hash_float(T*, int);
int hash_double(T*, int);
int hash_str(T*, int);
int hash_ptr(T*, int);

void read_vargs_char(va_list, T*);
void read_vargs_uchar(va_list, T*);
void read_vargs_short(va_list, T*);
void read_vargs_ushort(va_list, T*);
void read_vargs_int(va_list, T*);
void read_vargs_uint(va_list, T*);
void read_vargs_long(va_list, T*);
void read_vargs_ulong(va_list, T*);
void read_vargs_float(va_list, T*);
void read_vargs_double(va_list, T*);
void read_vargs_str(va_list, T*);
void read_vargs_ptr(va_list, T*);

void write_vargs_char(va_list, T*);
void write_vargs_uchar(va_list, T*);
void write_vargs_short(va_list, T*);
void write_vargs_ushort(va_list, T*);
void write_vargs_int(va_list, T*);
void write_vargs_uint(va_list, T*);
void write_vargs_long(va_list, T*);
void write_vargs_ulong(va_list, T*);
void write_vargs_float(va_list, T*);
void write_vargs_double(va_list, T*);
void write_vargs_str(va_list, T*);
void write_vargs_ptr(va_list, T*);

void bit_cpy_char(T*, T*);
void bit_cpy_uchar(T*, T*);
void bit_cpy_short(T*, T*);
void bit_cpy_ushort(T*, T*);
void bit_cpy_int(T*, T*);
void bit_cpy_uint(T*, T*);
void bit_cpy_long(T*, T*);
void bit_cpy_ulong(T*, T*);
void bit_cpy_float(T*, T*);
void bit_cpy_double(T*, T*);
void bit_cpy_str(T*, T*);
void bit_cpy_ptr(T*, T*);


#endif