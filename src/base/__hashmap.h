#ifndef _HASHMAP_H_
#define _HASHMAP_H_

#include "__type_value.h"
#include "__container.h"
#include "__list.h"

#define HASHCODE_INDEX_SIZE 128

typedef struct _entity 
{
    type_value_t key;
    type_value_t value;
} entity_t;

typedef struct _hash_node
{
    entity_t entity;
    int slot_index;
} hash_node_t;

typedef struct _hashmap 
{
    container_t container;
    iterator_t _slot[HASHCODE_INDEX_SIZE];
    list_t*     _hash_table;
    int (*key_hasher)(type_value_t);
    int (*key_compare)(type_value_t, type_value_t);
    size_t _size;

} hashmap_t;

container_t* hashmap_create(int (*key_hasher)(type_value_t), int (*key_compare)(type_value_t, type_value_t));
int hashmap_destroy(container_t*);

#endif