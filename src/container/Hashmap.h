/*
 * @Author: your name
 * @Date: 2020-10-29 11:33:41
 * @LastEditTime: 2021-06-29 13:49:15
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/Hashmap.h
 */
#ifndef _HASH_MAP_H_
#define _HASH_MAP_H_
#include "base/__hashmap.h"
#include "Map.h"
#include "Cn.h"
#include "Tv.h"
#include "Entity.h"

typedef hash_node_t HashmapNode;

#define HASHMAP_SLOT_SIZE 1000

#define _Hashmap(key_hasher) \
    ({ \
        Map hm; \
        Map_init(&hm, hashmap, Map_setup, Map_conflict_fix, HASHMAP_SLOT_SIZE, key_hasher, Map_entity_key_equl);\
        CN_set_extra_func(&hm, Hashmap_Expose_Entity);\
        hm; \
    })

#define Hashmap_(hm_ptr) Map_uninit(hm_ptr, hashmap, Hashmap_cleanup_entity)
#define Hahsmap_node_2_entity(pnode) \
    ({ \
        Entity* __p_marco_entity = t2p(((HashmapNode*)(pnode))->entity); \
        __p_marco_entity; \
    })
    
static inline
int Hashmap_cleanup_entity (Tv v) 
{
    HashmapNode *pnode = t2p(v);
    Entity* pentity = t2p(pnode->entity);
    free(pentity);
    return 0;
}

static inline 
Entity* Hashmap_Expose_Entity (Tv v) 
{
    HashmapNode *pnode = t2p(v);
    Entity* pentity = t2p(pnode->entity);
    return pentity;
}
#endif