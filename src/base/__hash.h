/*
 * @Author: your name
 * @Date: 2020-10-11 19:54:27
 * @LastEditTime: 2021-10-20 16:33:13
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

#define hash_table_head(phash) (&((phash)->_hash_table._sentinel))
#define hash_table_tail(phash) (&((phash)->_hash_table._sentinel))

typedef struct _hash_inner_list_node hash_inner_list_node_t;

struct _hash_inner_list_node
{
    hash_inner_list_node_t* prev;
    hash_inner_list_node_t* next;
    int slot_index;
    type_value_t w[1];
};

typedef struct _hash_inner_list
{
    hash_inner_list_node_t _sentinel;
} hash_inner_list;

typedef struct _hash
{
    container_t container;
    int (*setup) (type_value_t*, type_value_t*);
    int (*conflict_fix) (type_value_t*, type_value_t*);
    int  _slot_size;
    hash_inner_list _hash_table;
    hash_inner_list_node_t*  _slot[0];
} hash_t;

container_t* hashmap_create(T_def* __ty_def, int slot, int (*setup)(type_value_t*, type_value_t*), int (*conflict_fix)(type_value_t*, type_value_t*));
int hashmap_destroy(container_t*);

#endif