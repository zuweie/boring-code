/*
 * @Author: your name
 * @Date: 2021-10-21 15:16:31
 * @LastEditTime: 2021-10-21 16:27:35
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/Entity.h
 */
#ifndef __ENTITY_H__
#define __ENTITY_H__
#include "ty.h"

#define MAX_ENT_FIELD_NUM 32

typedef struct __entity_template {
    int field_num;
    int value_idx;
    int* field_types;
} entity_template_t;

typedef struct __entity {
    entity_template_t* tpl;
    T** block;
} entity_t;

#endif