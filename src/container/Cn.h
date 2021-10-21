/*
 * @Author: your name
 * @Date: 2021-10-21 11:59:07
 * @LastEditTime: 2021-10-21 16:27:27
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
    vector = (unsigned long) 1 << 27,
    list   = (unsigned long) 1 << 26,
    tree_set = (unsigned long) 1 << 25,
    hash_set = (unsigned long) 1 << 24,

    // build options
    multi_key = (unsigned long) 1 << 15,
    use_entity = (unsigned long) 1 << 14,
    customized_entity = (unsigned long) 1 << 13,
    customized_compare = (unsigned long) 1 << 12,
    customized_hasher = (unsigned long) 1 << 11,
};  

enum {
    // combine container
    tree_map = tree_set | use_entity,
    hash_map = hash_set | use_entity,
    multi_tree_set = tree_set | multi_key,
    multi_tree_map = tree_set | use_entity | multi_key,
    mulit_hash_set = hash_set | multi_key,
    multi_hash_map = hash_set | use_entity | multi_key,
};

// error code
enum {
    err_ok = 0;
    err_no_found = -1,
    err_empty = -2,
    err_out_of_capcity = -10,
    err_unsupported_type = -11,
    err_unsupported_container = -12,
    err_out_of_max_entity_field = -13,
};

typedef int CN;
typedef struct __cn cn_t;

struct __cn {
    container_t* container;
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