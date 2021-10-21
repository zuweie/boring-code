/*
 * @Author: your name
 * @Date: 2021-10-21 11:59:07
 * @LastEditTime: 2021-10-21 15:07:47
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

// build code
enum {
    vector = (unsigned long) 1 << 8,
    list   = (unsigned long) 1 << 9,
    tree_set = (unsigned long) 1 << 10,
    hash_set = (unsigned long) 1 << 11,
    use_entity = (unsigned long) 1 << 4,
    custom
}

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

int CN_del(CN, ...);
int CN_set(CN, ...);
int CN_get(CN, ...);

#endif