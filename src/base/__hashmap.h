/*
 * @Author: your name
 * @Date: 2020-10-11 19:54:27
 * @LastEditTime: 2020-10-22 16:21:39
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/base/__hashmap.h
 */
#ifndef _HASHMAP_H_
#define _HASHMAP_H_

#include "__container.h"
#include "__iterator.h"
#include "__list.h"

typedef struct _hash_node
{
    type_value_t entity;
    int slot_index;
} hash_node_t;

typedef struct _hashmap 
{
    container_t container;
    int (*key_hasher)(type_value_t, size_t);
    int (*key_compare)(type_value_t, type_value_t);
    list_t*     _hash_table;
    size_t      _slot_size;
    iterator_t  _slot[0];
} hashmap_t;

container_t* hashmap_create(size_t, int (*key_hasher)(type_value_t, size_t), int (*key_compare)(type_value_t, type_value_t));
int hashmap_destroy(container_t*);

#endif