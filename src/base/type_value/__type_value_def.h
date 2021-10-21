/*
 * @Author: your name
 * @Date: 2021-10-07 20:08:54
 * @LastEditTime: 2021-10-21 15:07:49
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/base/type_value/type_def.h
 */
#ifndef __TYPE_VALUE_DEF_H__
#define __TYPE_VALUE_DEF_H__
#include <stdarg.h>
#include "__type_value.h"

#define MAX_T_DEF_SLOT_SIZE 32

typedef struct {
    void (*read_vargs)(va_list, type_value_t*);
    void (*write_vargs)(va_list, type_value_t*);
    void (*bit_cpy)(type_value_t*, type_value_t*);
} T_adapter;

typedef struct {

    int ty_id;
    int ty_size;
    int (*ty_cmp)(type_value_t*, type_value_t*);
    int (*ty_hasher)(type_value_t*, int);
    T_adapter ty_adapter;
} T_def;

int T_def_reg(int T_size, int(*)(type_value_t*, type_value_t*), int (*)(type_value_t*, type_value_t*), void(*)(va_list, type_value_t*), void(*)(va_list, type_value_t*), void(*)(type_value_t*, type_value_t*));
int T_def_unreg(int T_id);
int T_def_is_reg(int T_id);
T_def* T_def_get(int T_id);
#endif