/*
 * @Author: your name
 * @Date: 2020-11-29 10:05:22
 * @LastEditTime: 2020-11-30 12:09:05
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
        int ret = CN_has(con, p2t(&__marco_entity));       \
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