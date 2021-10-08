/*
 * @Author: your name
 * @Date: 2021-10-07 20:11:49
 * @LastEditTime: 2021-10-08 16:02:45
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/base/type_value/__built_in_type.c
 */

#include <string.h>
#include <math.h>
#include "__built_in_type.h"

int cmp_char(T* t1, T* t2)
{
    char v1 = T_(t1, char);
    char v2 = T_(t2, char);
    return v1 > v2 ? 1 : (v1 = v2 ? 0 : -1);
}
int cmp_uchar(T* t1, T* t2)
{
    unsigned char v1 = T_(t1, unsigned char);
    unsigned char v2 = T_(t2, unsigned char);
    return v1 > v2 ? 1 : (v1 = v2 ? 0 : -1);
}
int cmp_short(T* t1, T* t2)
{
    short v1 = T_(t1, short);
    short v2 = T_(t2, short);
    return v1 > v2 ? 1 : (v1 = v2 ? 0 : -1);
}
int cmp_ushort(T* t1, T* t2)
{
    unsigned short v1 = T_(t1, unsigned short);
    unsigned short v2 = T_(t2, unsigned short);
    return v1 > v2 ? 1 : (v1 = v2 ? 0 : -1);

}
int cmp_int(T* t1, T* t2)
{
    int v1 = T_(t1, int);
    int v2 = T_(t2, int);
    return v1 > v2 ? 1 : (v1 = v2 ? 0 : -1);
}
int cmp_uint(T* t1, T* t2)
{
    unsigned int v1 = T_(t1, unsigned int);
    unsigned int v2 = T_(t2, unsigned int);
    return v1 > v2 ? 1 : (v1 = v2 ? 0 : -1);
}
int cmp_long(T* t1, T* t2)
{
    long v1 = T_(t1, long);
    long v2 = T_(t2, long);
    return v1 > v2 ? 1 : (v1 = v2 ? 0 : -1);
}

int cmp_ulong(T* t1, T* t2)
{
    unsigned long v1 = T_(t1, unsigned long);
    unsigned long v2 = T_(t2, unsigned long);
    return v1 > v2 ? 1 : (v1 = v2 ? 0 : -1);
}

int cmp_flv(T* t1, T* t2)
{
    float v1 = T_(t1, double);
    float v2 = T_(t2, double);
    if (fabs(v1 - v2) <= 0.00001) return 0;
    else if (v1 > v2) return 1;
    else return -1;
}

int cmp_dbl(T* t1, T* t2)
{
    double v1 = T_(t1, double);
    double v2 = T_(t2, double);
    if (fabs(v1-v2) <= 0.00001) return 0;
    else if (v1>v2) return 1;
    else return -1;
}

int cmp_str(T* t1, T* t2)
{
    char* v1 = T_(t1, char*);
    char* v2 = T_(t1, char*);
    return strcmp(v1, v2);
}
int cmp_ptr(T* t1, T* t2)
{
    return cmp_long(t1, t2);
}

int hash_char(T* t1, int slot_size)
{
    return T_(t1, char) % slot_size;
}
int hash_uchar(T* t1, int slot_size)
{
    return  T_(t1, unsigned int) % slot_size;
}
int hash_short(T* t1, int slot_size)
{
    return T_(t1, short) % slot_size;
}
int hash_ushort(T* t1, int slot_size)
{
    return T_(t1, unsigned short) % slot_size;
}
int hash_int(T* t1, int slot_size)
{
    return T_(t1, int) % slot_size;
}
int hash_uint(T* t1, int slot_size)
{
    return T_(t1, unsigned int) % slot_size;
}
int hash_long(T* t1, int slot_size)
{
    return T_(t1, long) % slot_size;
}
int hash_ulong(T* t1, int slot_size)
{
    return T_(t1, unsigned long) % slot_size;
}
int hash_float(T* t1, int slot_size)
{  
    return T_(t1, unsigned long) % slot_size;
}
int hash_double(T* t1, int slot_size)
{
    return T_(t1, unsigned long) % slot_size;
}
int hash_str(T* t1, int slot_size)
{
    char* str = t1;
    int str_size = strlen(str);
    int sum = 0;
    for (int i=0; i<str_size; ++i) {
        sum += str[i];
    }
    return sum % slot_size;
}

int hash_ptr(T* t1, int slot_size)
{
    return hash_ulong(t1, slot_size);
}

void read_vargs_char(va_list valist, T* pt)
{
    READ_VARGS(valist, char, pt);
}

void read_vargs_uchar(va_list valist, T* pt)
{
    READ_VARGS(valist, unsigned char, pt);
}

void read_vargs_short(va_list valist, T* pt)
{
    READ_VARGS(valist, short, pt);
}

void read_vargs_ushort(va_list valist, T* pt)
{
    READ_VARGS(valist, unsigned short, pt);
}

void read_vargs_int(va_list valist, T* pt)
{
    READ_VARGS(valist, int, pt);
}
void read_vargs_uint(va_list valist, T* pt)
{
    READ_VARGS(valist, unsigned int, pt);
}

void read_vargs_long(va_list valist, T* pt)
{
    READ_VARGS(valist, long, pt);
}

void read_vargs_ulong(va_list valist, T* pt)
{
    READ_VARGS(valist, unsigned long, pt);
}

void read_vargs_float(va_list valist, T* pt)
{
    //READ_VARGS(valist, double, pt);
    double v = va_arg(valist, double);
    (*(float*)pt) = (float) v;
}
void read_vargs_double(va_list valist, T* pt)
{
    READ_VARGS(valist, double, pt);
}

void read_vargs_str(va_list valist, T* pt)
{
    READ_VARGS(valist, char*, pt);
}
void read_vargs_ptr(va_list valist, T* pt)
{
    READ_VARGS(valist, char*, pt);
}

void write_vargs_char(va_list valist, T* pt)
{
    WRITE_VARGS(valist, char, pt);
}

void write_vargs_uchar(va_list valist, T* pt)
{
    WRITE_VARGS(valist, unsigned char, pt);
}

void write_vargs_short(va_list valist, T* pt)
{
    WRITE_VARGS(valist, short, pt);
}

void write_vargs_ushort(va_list valist, T* pt)
{
    WRITE_VARGS(valist, unsigned short, pt);
}
void write_vargs_int(va_list valist, T* pt)
{
    WRITE_VARGS(valist, int, pt);
}

void write_vargs_uint(va_list valist, T* pt)
{
    WRITE_VARGS(valist, unsigned int, pt);
}
void write_vargs_long(va_list valist, T* pt)
{
    WRITE_VARGS(valist, long, pt);
}
void write_vargs_ulong(va_list valist, T* pt)
{
    WRITE_VARGS(valist, unsigned long, pt);
}
void write_vargs_float(va_list valist, T* pt)
{
    char* pv = va_arg(valist, char*);
    *((float*)pv) = (float)(*((double*)pt));
}
void write_vargs_double(va_list valist, T* pt)
{
    WRITE_VARGS(valist, double, pt);
}
void write_vargs_str(va_list valist, T* pt)
{
    WRITE_VARGS(valist, char*, pt);
}

void write_vargs_ptr(va_list valist, T* pt)
{
    WRITE_VARGS(valist, char*, pt);
}

void bit_cpy_char(T* p_dest, T* p_src)
{
    BIT_CPY(p_dest, p_src, char);
}
void bit_cpy_uchar(T* p_dest, T* p_src)
{
    BIT_CPY(p_dest, p_src, unsigned char);
}
void bit_cpy_short(T* p_dest, T* p_src)
{
    BIT_CPY(p_dest, p_src, short);
}
void bit_cpy_ushort(T* p_dest, T* p_src)
{
    BIT_CPY(p_dest, p_src, unsigned short);
}
void bit_cpy_int(T* p_dest, T* p_src)
{
    BIT_CPY(p_dest, p_src, int);
}
void bit_cpy_uint(T* p_dest, T* p_src)
{
    BIT_CPY(p_dest, p_src, unsigned int);
}
void bit_cpy_long(T* p_dest, T* p_src)
{
    BIT_CPY(p_dest, p_src, long);
}
void bit_cpy_ulong(T* p_dest, T* p_src)
{
    BIT_CPY(p_dest, p_src, unsigned long);
}
void bit_cpy_float(T* p_dest, T* p_src)
{
    BIT_CPY(p_dest, p_src, float);
}
void bit_cpy_double(T* p_dest, T* p_src)
{
    BIT_CPY(p_dest, p_src, double);
}
void bit_cpy_str(T* p_dest, T* p_src)
{
    BIT_CPY(p_dest, p_src, char*);
}
void bit_cpy_ptr(T* p_dest, T* p_src)
{
    BIT_CPY(p_dest, p_src, char*);
}