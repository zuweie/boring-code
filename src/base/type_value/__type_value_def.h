/*
 * @Author: your name
 * @Date: 2021-10-07 20:08:54
 * @LastEditTime: 2021-10-29 17:34:31
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
    adapter_cmp=0,
    adapter_hash,
    adapter_setup,
    adapter_vargs,
};

typedef void (*T_adapter)(void);
typedef struct {
    int ty_id;
    int ty_size;
} T_def;

int T_def_reg(    
    int T_size, 
    int (*cmp)(type_value_t*, type_value_t*, int), 
    int (*hasher)(type_value_t*, int, int), 
    int (*setup)(type_value_t*, type_value_t*), 
    int (*vargs_reader)(va_list, type_value_t*, int)
);
int T_def_unreg(int T_id);
int T_def_is_reg(int T_id);
T_def* T_def_get_base(int T_id);

#endif