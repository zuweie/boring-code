/*
 * @Author: your name
 * @Date: 2020-11-29 10:05:22
 * @LastEditTime: 2020-12-10 17:00:40
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/Set.h
 */

#ifndef _SET_H_
#define _SET_H_

#include "Cn.h"
#include "Entity.h"

typedef Container Set;

#define Set_init(set, set_clazz, setup, conflict, ...) CN_initialize(set, set_clazz, NULL, setup, conflict, __VA_ARGS__);
#define Set_uninit(set, set_clazz, cleanup) CN_uninitialize(set, set_clazz, cleanup)

#define set_hasx(con, x, ...)     \
    ({                            \
        Tv __marco_tv[x];                  \
        Entity __marco_entity;            \
        Entity_temp(&__marco_entity, x, x, __marco_tv, __VA_ARGS__); \
        It it = CN_search(con, __null_iterator, p2t(&__marco_entity));\
        int ret = It_valid(it); \
        ret; \
    })
#define Set_has(con, key) set_hasx(con, 1, key)

#define Set_setx(con, x, ...) do {          \
    Tv __marco_tv[x];                                   \
    Entity __macro_entity;                             \
    Entity_temp(&__macro_entity, x, x, __marco_tv, __VA_ARGS__); \
    CN_set(con, p2t(&__macro_entity));                 \
} while(0)

#define Set_set(con, key) Set_setx(con, 1, key)

#define Set_getx(con, value, x, ...)   \
    ({                                     \
        int ret = -1;                      \
        Tv __marco_tv[x];                           \
        Entity __marco_entity;                     \
        Entity_temp(&__marco_entity, x, x, __marco_tv, __VA_ARGS__);  \
        It it = CN_search(con, __null_iterator, p2t(&__marco_entity)); \
        if (It_valid(it)) {                         \
            Expose_Entity exposer                   \
                = (Expose_Entity) c_extra_func(con);\
            Entity* ent = exposer(It_dref(it));     \
            value = ent->tv[0];                     \
            ret = 0;                                \
        }                                           \
        ret;                                        \
    })
#define Set_get(con, key, value) Set_getx(con, value, 1, key)

#define Set_delx(con, prdata, x, ...)        \
    ({                                      \
        int ret = -1;                       \
        Tv __marco_tv[x];                            \
        Entity __marco_entity;                      \
        Entity_temp(&__marco_entity, x, x, __marco_tv, __VA_ARGS__);  \
        Tv __marco_rentity;                                       \
        ret = CN_rm_target(con, p2t(&__marco_entity), &__marco_rentity); \
        if (ret == 0) {                                   \
            Entity* pentity = t2p(__marco_rentity);       \
            Tv* ttprdata = prdata;                        \
            if (ttprdata)                                 \
                *ttprdata = pentity->tv[pentity->value_index]; \
            free(pentity);                              \
        }                                               \ 
        ret;                                            \
    })

#define Set_del(con, key, prdata) Set_delx(con, prdata, 1, key)

#define Set_union(set1, set2) do { \
    for (It first = CN_first(set2); !It_equal(first, CN_tail(set2)); first = It_next(first)) { \
        Entity* __marco_entity = It_getptr(first); \
        Entity __marco_temp_entity; \
        Tv __marco_temp_tv[__marco_entity->number]; \
        Entity_cypto_temp(__marco_entity, &__marco_temp_entity, __marco_temp_tv); \
        CN_set(set1, p2t(&__marco_temp_entity)); \
    } \
} while(0)

#define Set_intersect(set1, set2) \
    ({ \
        int ret = 0; \
        It first = CN_size(set1) < CN_size(set2) ? CN_first(set1) : CN_first(set2); \
        It tail  = CN_size(set1) < CN_size(set2) ? CN_tail(set1) : CN_tail(set2); \
        Set __marco_set = CN_size(set1) >= CN_size(set2) ? set1 : set2; \
        for (;!It_equal(first, tail); first = It_next(first)) { \
            It it = CN_search(__marco_set, __null_iterator, It_dref(first)); \
            if (It_valid(it)) { \
                ret = 1; \
                break; \
            } \
        } \
        ret; \
    })

#define Set_get_item(con, it) \
    ({ \
        Entity* __marco_entity = It_getptr(it); \
        Tv __marco_tv = __marco_entity->tv[0]; \
        __marco_tv; \
    })

static inline 
int Set_setup (Tv* v1, Tv v2) 
{
    Entity* temp = t2p(v2);
    Entity* lentity = Entity_cpyto_heap_entity(temp);
    *v1 = p2t(lentity);
}

static inline 
int Set_conflict_fix(Tv* v1, Tv v2) 
{
    return 0;
}

static inline
int Set_entity_key_equl (Tv v1, Tv v2) 
{
    Entity* e1 = t2p(v1);
    Entity* e2 = t2p(v2);
    return Entity_is_key_equal(e1, e2);
}
#endif