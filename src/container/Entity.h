/*
 * @Author: your name
 * @Date: 2020-10-23 13:08:02
 * @LastEditTime: 2020-11-22 22:53:42
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/Entity.h
 */
#ifndef _ENTITY_H_
#define _ENTITY_H_
#include <stdlib.h>
#include "Tv.h"

typedef struct _entity {
    size_t value_index;
    size_t number;
    Tv* tv;
} Entity;

void Entity_temp(Entity*, int num, int value_index, Tv t[],  ...);

// 这个需要手动释放内存。
Entity* Entity_cpyto_heap_entity(Entity*);

int Entity_copy_Value(Entity*, Entity*);
int Entity_is_value_equal(Entity* e1, Entity* e2);
int Entity_is_key_equal(Entity* e1, Entity* e2);
#endif