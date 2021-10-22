/*
 * @Author: your name
 * @Date: 2021-10-21 11:59:07
 * @LastEditTime: 2021-10-22 11:54:25
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/Cn.h
 */
#ifndef __CN_H__
#define __CN_H__
#include "base/__container.h"
#include "it.h"
#include "ty.h"

#define CAPACITY_NUMBER 1024

enum {
    // base container type 
    VECTOR = (unsigned long) 1 << 27,
    LIST   = (unsigned long) 1 << 26,
    TREE_SET = (unsigned long) 1 << 25,
    HASH_SET = (unsigned long) 1 << 24,

    // build options
    multi_key = (unsigned long) 1 << 15,
    use_entity = (unsigned long) 1 << 14,
    customized_entity = (unsigned long) 1 << 13,
    customized_compare = (unsigned long) 1 << 12,
    customized_hasher = (unsigned long) 1 << 11,
};  

enum {
    // combine container
    TREE_MAP = tree_set | use_entity,
    HASH_MAP = hash_set | use_entity,
    MULTI_TREE_SET = tree_set | multi_key,
    MULTI_TREE_MAP = tree_set | use_entity | multi_key,
    MULTI_HASH_SET = hash_set | multi_key,
    MULTI_HASH_MAP = hash_set | use_entity | multi_key,
};

// error code
enum {
    err_ok = 0;
    err_no_found = -1,
    err_empty = -2,
    err_out_of_capcity = -10,
    err_unsupported_type = -11,
    err_unsupported_eng = -12,
    err_out_of_max_ent_field = -13,
    err_invalid_ent_keys_number = -14,
};

typedef int CN;
typedef struct __cn cn_t;

struct __cn {
    container_t* eng;
    unsigned long build_code;
    void* type_info;
    unsigned char is_forward;
}

CN CN_create(unsigned long build_code, ...);
CN CN_finalize(CN cn, int(*cleanup)(T*));

It CN_head(CN cn);
It CN_tail(CN cn);
It CN_first(CN cn);
It CN_last(CN cn);

int CN_reverse(CN cn);

int CN_add(CN, ...);
int CN_remove(CN, It, T*);
It CN_find(CN, T*, int (*cmp)(T*, T*));

int CN_del(CN, ...);
int CN_set(CN, ...);
int CN_get(CN, ...);

#endif