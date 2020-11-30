/*
 * @Author: your name
 * @Date: 2020-11-30 08:05:26
 * @LastEditTime: 2020-11-30 12:20:13
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/TreeSet.h
 */
#ifndef _TREE_SET_H_
#define _TREE_SET_H_

#include "base/__rb_tree.h"
#include "Cn.h"
#include "Entity.h"
#include "Set.h"

#define _Treeset(insert_cmp) \
    ({                       \
        Set set;              \
        Set_init(set, rb_tree, Set_setup, Set_conflict_fix, insert_cmp);\
        CN_set_extra_func(tm, Treeset_Expose_Entity); \
        set; \
    })

#define Treeset_(set) Set_uninit(set, rb_tree, Treeset_cleanup_entity)

static inline 
int Treeset_cleanup_entity (Tv v) 
{
    Entity* pentity = t2p(v);
    free(pentity);
    return 0;
}

static inline
Entity* Treeset_Expose_Entity (Tv v) 
{
    return t2p(v);
}

#endif