/*
 * @Author: your name
 * @Date: 2020-10-14 21:35:27
 * @LastEditTime: 2020-12-11 13:25:04
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/Hashmap.h
 */
#ifndef _MAP_H_
#define _MAP_H_
#include "Cn.h"
#include "Entity.h"

typedef Container   Map;
typedef Entity* (*Expose_Entity)(Tv);

#define Map_init(map, map_clazz, setup, conflict, ...) CN_initialize(map, map_clazz, NULL, setup, conflict, __VA_ARGS__)

#define Map_uninit(map, map_clazz, cleanup) CN_uninitialize(map, map_clazz, cleanup)

#define Map_hasx(con, x, ...)     \
    ({                            \
        Tv __marco_tv[x];                  \
        Entity __marco_entity;            \
        Entity_temp(&__marco_entity, x, x, __marco_tv, __VA_ARGS__); \
        It it = CN_search(con, __null_iterator, p2t(&__marco_entity));\
        int ret = It_valid(it);\
        ret; \
    })
#define Map_has(con, key) Map_hasx(con, 1, key)
#define Map_has2(con, key1, key2) Map_hasx(con, 2, key1, key2)
#define Map_has3(con, key1, key2, key3) Map_hasx(con, 3, key1, key2, key3)
 //CN_has(con, key)

#define Map_setx(con, x, y, ...) do {          \
    Tv __marco_tv[x];                                   \
    Entity __macro_entity;                             \
    Entity_temp(&__macro_entity, x, y, __marco_tv, __VA_ARGS__); \
    CN_set(con, p2t(&__macro_entity));                 \
} while(0)

#define Map_set(con, key, value) Map_setx(con, 2, 1, key, value)
#define Map_set2(con, key1, key2, value) Map_setx(con, 3, 2, key1, key2, value)
#define Map_set3(con, key1, key2, key3, value) Map_setx(con, 4, 3, key1, key2, key3, value)

#define Map_getx(con, value, x, ...)   \
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
            value = ent->tv[ent->value_index];      \
            ret = 0;                                \
        }                                           \
        ret;                                        \
    })
#define Map_get(con, key, value) Map_getx(con, value, 1, key)
#define Map_get2(con, key1, key2, value) Map_getx(con, value, 2, key1, key2)
#define Map_get3(con, key1, key2, key3, value) Map_getx(con, value, 3, key1, key2, key3)

#define Map_delx(con, prdata, x, ...)        \
    ({                                      \
        int ret = -1;                       \
        Tv __marco_tv[x];                            \
        Entity __marco_entity;                      \
        Entity_temp(&__marco_entity, x, x, __marco_tv, __VA_ARGS__);        \
        Tv __marco_rentity;                                       \
        ret = CN_rm_target(con, p2t(&__marco_entity), &__marco_rentity);  \
        if (ret == 0) {                                   \
            Entity* pentity = t2p(__marco_rentity);       \
            Tv* ttprdata = prdata;                        \
            if (ttprdata)                                   \
                *ttprdata = pentity->tv[pentity->value_index]; \
            free(pentity);                              \
        }                                               \ 
        ret;                                            \
    })

#define Map_del(con, key, prdata) Map_delx(con, prdata, 1, key)
#define Map_del2(con, key1, key2, prdata) Map_delx(con, prdata, 2, key1, key2)
#define Map_del3(con, key1, key2, key3, prdata) Map_delx(con, prdata, 3, key1, key2, key3)

#define Map_get_entity(con,it) \ 
    ({ \
        Expose_Entity exposer = (Expose_Entity) c_extra_func(con); \
        Entity* ent = exposer(It_dref(it)); \
        ent; \
    })

static inline 
int Map_setup (Tv* v1, Tv v2) 
{
    Entity* temp = t2p(v2);
    Entity* lentity = Entity_malloc_copy_entity(temp);
    //printf("make a lentity %p \n", lentity);
    *v1 = p2t(lentity);
}

static inline 
int Map_conflict_fix(Tv* v1, Tv v2) 
{
    // 算出来了 key 相同。
    Entity* temp = t2p(v2);
    Entity* lentity = t2p((*v1));
    
    if (Entity_is_value_equal(lentity, temp) == 0) return 0;

    // 然后把 value 弄过去。
    if (temp->number == lentity->number 
    && temp->value_index == lentity->value_index) {
        Entity_copy_Value(lentity, temp);
    } else {
        // 直接把旧的弄掉，换新的上去。
        free(lentity);
        Entity* nentity = Entity_malloc_copy_entity(temp);
        *v1 = p2t(nentity);
    } 
    return 0;
}

static inline
int Map_entity_key_equl (Tv v1, Tv v2) 
{
    Entity* e1 = t2p(v1);
    Entity* e2 = t2p(v2);
    return Entity_is_key_equal(e1, e2);
}
#endif