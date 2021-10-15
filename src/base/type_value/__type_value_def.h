/*
 * @Author: your name
 * @Date: 2021-10-07 20:08:54
 * @LastEditTime: 2021-10-15 15:52:03
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
    void (*read_vargs)(va_list, T*);
    void (*write_vargs)(va_list, T*);
    void (*bit_cpy)(T*, T*);
} T_adapter;

typedef struct {

    int ty_id;
    int ty_size;
    int (*ty_cmp)(T*, T*);
    int (*ty_hasher)(T*, int);
    T_adapter ty_adapter;
} T_def;

int T_def_reg(int T_size, int(*)(T*, T*), int (*)(T*, T*), void(*)(va_list, T*), void(*)(va_list, T*), void(*)(T*, T*));
int T_def_unreg(int T_id);
int T_def_is_reg(int T_id);
T_def* T_def_get(int T_id);
#endif