/*
 * @Author: your name
 * @Date: 2020-10-14 21:35:27
 * @LastEditTime: 2020-10-27 01:05:51
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/Hashmap.h
 */
#ifndef _HASHMAP_2_H_
#define _HASHMAP_2_H_
#include <string.h>
#include "Cn.h"
#include "base/__hashmap.h"
#include "Entity.h"

typedef Container   Hashmap;
typedef hash_node_t HashNode;

#define HASHMAP_SLOT_SIZE 100

#define _Hashmap(key_hasher, key_cmp, setup, conflict_fix) \
    ({ \
        Hashmap hm; \
        CN_initialize(hm, hashmap, NULL, setup, conflict_fix, HASHMAP_SLOT_SIZE, key_hasher, key_cmp); \
        hm; \
    })

#define Hashmap_(hm, cleanup) CN_uninitialize(hm, hashmap, cleanup)

#define Hashmap_hasx(con, x, ...) \
    ({                            \
        Tv t[x];                  \
        Entity entity;            \
        TempEntity(&entity, x, x, t, __VA_ARGS__); \
        int ret = CN_has(con, p2t(&entity)); \
        ret; \
    })
#define Hashmap_has(con, key) Hashmap_hasx(con, 1, key)
#define Hashmap_has2(con, key1, key2) Hashmap_hasx(con, 2, key1, key2)
#define Hashmap_has3(con, key1, key2, key3) Hashmap_hasx(con, 3, key1, key2, key3)
 //CN_has(con, key)

#define Hashmap_setx(con, x, y, ...) do {      \
    Tv t[x];                                   \
    Entity entity;                             \
    TempEntity(&entity, x, y, t, __VA_ARGS__);  \
    CN_set(con, p2t(&entity));                 \
} while(0)

#define Hashmap_set(con, key, value) Hashmap_setx(con, 2, 1, key, value)
#define Hashmap_set2(con, key1, key2, value) Hashmap_setx(con, 3, 2, key1, key2, value)
#define Hashmap_set3(con, key1, key2, key3, value) Hashmap_setx(con, 4, 3, key1, key2, key3, value)

#define Hashmap_getx(con, value, x, ...)   \
    ({                                     \
        int ret = -1;                      \
        Tv t[x];                           \
        Entity entity;                     \
        TempEntity(&entity, x, x, t, __VA_ARGS__);  \
        It it = CN_find(con, p2t(&entity));         \
        if (It_valid(it)) {                         \
            Entity* ent = It_dref(it);              \
            value = ent->tv[ent->value_index];      \
            ret = 0;                                \
        }                                           \
        ret;                                        \
    })
#define Hashmap_get(con, key, value) Hashmap_getx(con, value, 1, key)
#define Hashmap_get2(con, key1, key2, value) Hashmap_getx(con, value, 2, key1, key2)
#define Hashmap_get3(con, key1, key2, key3, value) Hashmap_getx(con, value, 3, key1, key2, key3)

#define Hashmap_delx(con, /*rdata,*/ x, ...)    \
    ({                                      \
        int ret = -1;                       \
        Tv t[x];                            \
        Entity entity;                      \
        TempEntity(&entity, x, x, t, __VA_ARGS__); \
        Tv rentity;                                \
        ret = CN_rm_target(con, p2t(t), rentity);  \
        if (ret == 0) {                          \
            Entity* pentity = t2p(rentity);      \
            /*rdata = pentity->tv[pentity->value_index];*/ \
            free(pentity);                             \
        }                                              \
        ret;\
    })

#define Hashmap_del(con, key/*, rdata*/) Hashmap_delx(con, /*rdata,*/ 1, key)
#define Hashmap_del2(con, key1, key2/*, rdata*/) Hashmap_delx(con, /*rdata,*/ 2, key1, key2)
#define Hashmap_del3(con, key1, key2, key3/*, rdata*/) Hashmap_delx(con, /*rdata,*/ 3, key1, key2, key3)

static inline 
int Hashmap_setup (Tv* v1, Tv v2) 
{
    Entity* temp = t2p(v2);
    Entity* lentity = CopyALongTimeEntity(temp);
    *v1 = p2t(lentity);
}

static inline 
int Hashmap_conflict_fix(Tv* v1, Tv v2) 
{
    // 算出来了 key 相同。
    Entity* temp = t2p(v2);
    Entity* lentity = t2p((*v1));
    
    if (EntityValueEqual(lentity, temp) == 0) return 0;

    // 然后把 value 弄过去。
    if (temp->number == lentity->number 
    && temp->value_index == lentity->value_index) {
        CopyEntityValue(lentity, temp);
    } else {
        // 直接把旧的弄掉，换新的上去。
        free(lentity);
        Entity* nentity = CopyALongTimeEntity(temp);
        *v1 = p2t(nentity);
    } 
    return 0;
}
static inline
int Hashmap_cleanup (Tv v) 
{
    Entity* entity = t2p(v);
    free(entity);
    return 0;
}
static inline
int Entity_key_cmp (Tv v1, Tv v2) 
{
    Entity* e1 = t2p(v1);
    Entity* e2 = t2p(v2);
    return EntityValueEqual(e1, e2);
}

#endif