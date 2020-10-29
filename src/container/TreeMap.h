/*
 * @Author: your name
 * @Date: 2020-10-29 11:33:49
 * @LastEditTime: 2020-10-29 12:53:48
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/TreeMap.h
 */
#ifndef _TREE_MAP_H_
#define _TREE_MAP_H_
#include "base/__rb_tree.h"
#include "Cn.h"
#include "Entity.h"
#include "Map.h"
#define _Treemap(insert_cmp) \
    ({                       \
        Map tm;              \
        Map_init(tm, rb_tree, Map_setup, Map_conflict_fix, insert_cmp);\
        CN_set_extra_func(tm, Treemap_Expose_Entity); \
        tm; \
    })

#define Treemap_(tm) Map_uninit(tm, rb_tree, Hashmap_cleanup_entity)

static inline 
int Treemap_cleanup_entity (Tv v) 
{
    Entity* pentity = t2p(v);
    free(pentity);
    return 0;
}

static inline
Entity* Treemap_Expose_Entity (Tv v) 
{
    return t2p(v);
}

#endif