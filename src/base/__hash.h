/*
 * @Author: your name
 * @Date: 2020-10-11 19:54:27
 * @LastEditTime: 2021-10-19 16:31:53
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/base/__hashmap.h
 */
#ifndef __HASH_H__
#define __HASH_H__

#include "__container.h"
#include "__iterator.h"
#include "type_value/__type_value.h"
#include "type_value/__type_value_def.h"
#include "__list.h"

typedef struct _hash_node
{
    int slot_index;
    void* list_node_ptr;
    type_value_t w[1];
} hash_node_t;

typedef struct _hash
{
    container_t container;
    list_t*     _hash_table;
    iterator_t  _slot[0];
    int         _slot_size;
    int (*setup) (type_value_t*, type_value_t*);
    int (*conflict_fix) (type_value_t*, type_value_t*);
} hash_t;

container_t* hashmap_create(T_def* __ty_def, int slot, int (*setup)(type_value_t*, type_value_t*), int (*conflict_fix)(type_value_t*, type_value_t*));
int hashmap_destroy(container_t*);

#endif