/*
 * @Author: your name
 * @Date: 2020-12-01 15:00:06
 * @LastEditTime: 2020-12-01 16:14:06
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/HashmapComponent.h
 */
#ifndef _HASHMAP_COMPONENT_H_
#define _HASHMAP_COMPONENT_H_
#include "Tv.h"
#include "Entity.h"

static int 
HASHMAP_INT_HASHER(Tv v, size_t slot_size)
{
    Entity* entity = t2p(v);
    size_t key = 0;
    for (int i=0; i<entity->value_index; ++i) {
        key += t2i(entity->tv[i]);
    }
    return key % slot_size;
}

static int



#endif