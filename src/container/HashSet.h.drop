/*
 * @Author: your name
 * @Date: 2020-11-30 08:05:18
 * @LastEditTime: 2021-06-29 15:13:08
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/HashSet.h
 */

#ifndef _HASH_SET_H_
#define _HASH_SET_H_

#include "base/__hashmap.h"
#include "Set.h"
#include "Cn.h"
#include "Tv.h"
#include "Entity.h"

typedef hash_node_t HashsetNode;

#define HASHSET_SLOT_SIZE 1000

#define _Hashset(key_hasher) \
    ({ \
        Set set; \
        Set_init(&set, hashmap, Set_setup, Set_conflict_fix, HASHSET_SLOT_SIZE, key_hasher, Set_entity_key_equl);\
        CN_set_extra_func(&set, Hashset_Expose_Entity);\
        set; \
    })

#define Hashset_(set_ptr) Set_uninit(set_ptr, hashmap, Hashset_cleanup_entity)
#define Hashset_node_2_entity(pnode) \
    ({ \
        Entity* __p_marco_entity = t2p(((HashsetNode*)(pnode))->entity); \
        __p_marco_entity; \
    })
    
static inline
int Hashset_cleanup_entity (Tv v) 
{
    HashsetNode *pnode = t2p(v);
    Entity* pentity = t2p(pnode->entity);
    free(pentity);
    return 0;
}

static inline 
Entity* Hashset_Expose_Entity (Tv v) 
{
    HashsetNode *pnode = t2p(v);
    Entity* pentity = t2p(pnode->entity);
    return pentity;
}

#endif