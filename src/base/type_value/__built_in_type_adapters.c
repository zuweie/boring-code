/*
 * @Author: your name
 * @Date: 2021-10-07 20:11:49
 * @LastEditTime: 2021-11-02 09:53:17
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/base/type_value/__built_in_type.c
 */

#include <string.h>
#include <math.h>
#include "__built_in_type_adapters.h"

int cmp_char(type_value_t* t1, type_value_t* t2)
{
    char v1 = type_value_(t1, char);
    char v2 = type_value_(t2, char);
    return v1 > v2 ? 1 : (v1 = v2 ? 0 : -1);
}

int cmp_uchar(type_value_t* t1, type_value_t* t2)
{
    unsigned char v1 = type_value_(t1, unsigned char);
    unsigned char v2 = type_value_(t2, unsigned char);
    return v1 > v2 ? 1 : (v1 = v2 ? 0 : -1);
}
int cmp_int(type_value_t* t1, type_value_t* t2)
{
    int v1 = type_value_(t1, int);
    int v2 = type_value_(t2, int);
    return v1 > v2 ? 1 : (v1 = v2 ? 0 : -1);
}
int cmp_uint(type_value_t* t1, type_value_t* t2)
{
    unsigned int v1 = type_value_(t1, unsigned int);
    unsigned int v2 = type_value_(t2, unsigned int);
    return v1 > v2 ? 1 : (v1 = v2 ? 0 : -1);
}
int cmp_long(type_value_t* t1, type_value_t* t2)
{
    long v1 = type_value_(t1, long);
    long v2 = type_value_(t2, long);
    return v1 > v2 ? 1 : (v1 = v2 ? 0 : -1);
}
int cmp_ulong(type_value_t* t1, type_value_t* t2)
{
    unsigned long v1 = type_value_(t1, unsigned long);
    unsigned long v2 = type_value_(t2, unsigned long);
    return v1 > v2 ? 1 : (v1 = v2 ? 0 : -1);
}
int cmp_float(type_value_t* t1, type_value_t* t2)
{
    float v1 = type_value_(t1, float);
    float v2 = type_value_(t2, float);

    if (fabs(v1 - v2) <= 0.00001) return 0;
    else if (v1 > v2) return 1;
    else return -1;
}
int cmp_double(type_value_t* t1, type_value_t* t2)
{
    double v1 = type_value_(t1, double);
    double v2 = type_value_(t2, double);
    
    if (fabs(v1 - v2) <= 0.00001) return 0;
    else if (v1 > v2) return 1;
    else return -1;
}
int cmp_str(type_value_t* t1, type_value_t* t2)
{
    char* v1 = type_value_(t1, char*);
    char* v2 = type_value_(t1, char*);
    return strcmp(v1, v2);
}

int cmp_ptr(type_value_t* t1, type_value_t* t2)
{
    return cmp_long(t1, t2);
}

int hash_char(type_value_t* t, int slot_size)
{
    return type_value_(t, char) % slot_size;
}
int hash_uchar(type_value_t* t, int slot_size)
{
    return type_value_(t, unsigned int) % slot_size;
}
int hash_int(type_value_t* t, int slot_size)
{
    return type_value_(t, int) % slot_size;
}
int hash_uint(type_value_t* t, int slot_size)
{
    return type_value_(t, unsigned int) % slot_size;
}
int hash_long(type_value_t* t, int slot_size)
{
    return type_value_(t, long) % slot_size;
}
int hash_ulong(type_value_t* t, int slot_size)
{
    return type_value_(t, unsigned long) % slot_size;
}

int hash_float(type_value_t* t, int slot_size)
{
    return type_value_(t, unsigned int) % slot_size;
}
int hash_double(type_value_t* t, int slot_size)
{
    return type_value_(t, unsigned long) % slot_size;
}
int hash_str(type_value_t* t, int slot_size)
{
    char* str = t;
    int str_size = strlen(str);
    int sum = 0;
    for (int i=0; i<str_size; ++i) {
        sum += str[i];
    }
    return sum % slot_size;
}

int hash_ptr(type_value_t* t, int slot_size)
{
    return type_value_(t, unsigned long) % slot_size;
}


int setup_char(type_value_t* p_dest, type_value_t* p_src, unsigned char is_new)
{
    BIT_CPY(p_dest, p_src, char);
    return 0;
}
int setup_uchar(type_value_t* p_dest, type_value_t* p_src, unsigned char is_new)
{
    BIT_CPY(p_dest, p_src, unsigned char);
    return 0;
}
int setup_int(type_value_t* p_dest, type_value_t* p_src, unsigned char is_new)
{
    BIT_CPY(p_dest, p_src, int);
    return 0;
}
int setup_uint(type_value_t* p_dest, type_value_t* p_src, unsigned char is_new)
{
    BIT_CPY(p_dest, p_src, unsigned int);
    return 0;
}
int setup_long(type_value_t* p_dest, type_value_t* p_src, unsigned char is_new)
{
    BIT_CPY(p_dest, p_src, long);
    return 0;
}
int setup_ulong(type_value_t* p_dest, type_value_t* p_src, unsigned char is_new)
{
    BIT_CPY(p_dest, p_src, unsigned long);
    return 0;
}
int setup_float(type_value_t* p_dest, type_value_t* p_src, unsigned char is_new)
{
    BIT_CPY(p_dest, p_src, float);
    return 0;
}
int setup_double(type_value_t* p_dest, type_value_t* p_src, unsigned char is_new)
{
    BIT_CPY(p_dest, p_src, double);
    return 0;
}
int setup_str(type_value_t* p_dest, type_value_t* p_src, unsigned char is_new)
{
    BIT_CPY(p_dest, p_src, char*);
    return 0;
}
int setup_ptr(type_value_t* p_dest, type_value_t* p_src, unsigned char is_new)
{
    BIT_CPY(p_dest, p_src, char*);
    return 0;
}

int vargs_char(va_list valist, type_value_t* t, int using_at)
{
    READ_VARGS(valist, t, char);
    return 0;
}
int vargs_uchar(va_list valist, type_value_t* t, int using_at)
{
    READ_VARGS(valist, t, unsigned char);
    return 0;
}
int vargs_int(va_list valist, type_value_t* t, int using_at)
{
    READ_VARGS(valist, t, int);
    return 0;
}
int vargs_uint(va_list valist, type_value_t* t, int using_at)
{
    READ_VARGS(valist, t, unsigned int);
    return 0;
}
int vargs_long(va_list valist, type_value_t* t, int using_at)
{
    READ_VARGS(valist, t, long);
    return 0;
}
int vargs_ulong(va_list valist, type_value_t* t, int using_at)
{
    READ_VARGS(valist, t, unsigned long);
    return 0;
}
int vargs_float(va_list valist, type_value_t* t, int using_at)
{
    double v = va_arg(valist, double);
    *((float*)t) = (float)v;
    return 0;
}
int vargs_double(va_list valist, type_value_t* t, int using_at)
{
    double v = va_arg(valist, double);
    *((double*)t) = v;
    return 0;
}
int vargs_str(va_list valist, type_value_t* t, int using_at)
{
    READ_VARGS(valist, t, char*);
    return 0;
}
int vargs_ptr(va_list valist, type_value_t* t, int using_at)
{
    READ_VARGS(valist, t, char*);
    return 0;
}