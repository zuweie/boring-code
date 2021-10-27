/*
 * @Author: your name
 * @Date: 2021-10-07 20:08:54
 * @LastEditTime: 2021-10-27 12:59:03
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/base/type_value/type_def.h
 */
#ifndef __TYPE_VALUE_DEF_H__
#define __TYPE_VALUE_DEF_H__
#include <stdarg.h>
#include "__type_value.h"

#define MAX_T_DEF_SLOT_SIZE 32
#define MAX_T_VARGS_ADAPTER_SLOT_SIZE 64
#define VARGS_ADAPTER_SIZE 4

typedef struct {
    int cache_size;
    void (*adapter_func)(va_list, type_value_t* cache, int cache_size);
} T_vargs_adapter;

typedef struct {
    int ty_id;
    int ty_size;
    int (*ty_cmp)(type_value_t*, type_value_t*);
    int (*ty_hasher)(type_value_t*, int);
    int vargs_adapter[VARGS_ADAPTER_SIZE];
} T_def;

int T_def_reg( 
    int T_size, 
    int(*ty_cmp)(type_value_t*, type_value_t*), 
    int(*ty_hasher)(type_value_t*, type_value_t*)
);
int T_def_vargs_reg(
    int ty_id,
    int vargs_cache_size,
    int vargs_adapter_slot,
    void (*vargs_adapter_func)(va_list, type_value_t* cache, int cache_size)
)

int T_def_unreg(int T_id);
int T_def_is_reg(int T_id);
T_def* T_def_get(int T_id);
#endif