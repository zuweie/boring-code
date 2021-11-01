/*
 * @Author: your name
 * @Date: 2021-10-07 20:08:54
 * @LastEditTime: 2021-11-01 10:42:28
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/base/type_value/type_def.h
 */
#ifndef __TYPE_VALUE_DEF_H__
#define __TYPE_VALUE_DEF_H__
#include <stdarg.h>
#include "__type_value.h"

#define MAX_T_DEF_SLOT_SIZE 16

typedef enum {
    char_t=1, 
    uchar_t, 
    int_t, 
    uint_t, 
    long_t, 
    ulong_t, 
    fl_t, 
    db_t, 
    str_t, 
    ptr_t
} built_in_type_t;

typedef enum {
    e_cmp=0,
    e_hash,
    e_setup,
    e_vargs,
} adapter_t;

typedef void (*T_adapter)(void);
typedef int (*adapter_cmp) (type_value_t*, type_value_t*, int);
typedef int (*adapter_hasher) (type_value_t*, int, int);
typedef int (*adapter_setup) (type_value_t*, type_value_t*, unsigned char);
typedef int (*adapter_vargs_reader)(va_list, type_value_t*, int);


typedef struct {
    int ty_id;
    int ty_size;
} T_def;

typedef struct {
    T_def _def;
    T_adapter _adapter[4];
} T_clazz;

int T_def_reg(    
    int T_size,
    adapter_cmp cmp,
    adapter_hasher hahser,
    adapter_setup setup, 
    adapter_vargs_reader vargs_reader
);

int T_def_unreg(int T_id);
int T_def_is_reg(int T_id);
T_def T_def_get(int T_id);
#endif