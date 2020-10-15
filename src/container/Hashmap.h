/*
 * @Author: your name
 * @Date: 2020-10-14 21:35:27
 * @LastEditTime: 2020-10-15 12:53:13
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/Hashmap.h
 */
#ifndef _HASHMAP_2_H_
#define _HASHMAP_2_H_

#include "Cn.h"
#include "base/__hashmap.h"
typedef Container Hashmap;
typedef entity_t Entity;

#define Hashmap_init(hm, key_hasher, key_cmp) CN_initialize(hm, hashmap, NULL, key_hasher, key_cmp)
#define Hashmap_uninit(hm, cleanup) CN_uninitialize(hm, hashmap, cleanup)

#define Hashmap_has(con, key) CN_has(con, key)

#define Hashmap_set(con, key, value) do { \
    Entity entity = {                     \
        .key = key,                       \
        .value = value                    \
    };                                    \
    CN_insert(con, __null_iterator, p2t(&entity)); \
} while(0)
#define Hashmap_get(con, key, value)                   \
    ({                                                 \
        int ret = -1;                                  \
        It it = CN_find(con, key);                     \
        if (It_valid(it)) {                            \
            Entity *entity = (Entity*) It_getptr(it);  \
            *value = entity->value;                    \
            ret = 0;                                   \
        }                                              \
        ret;                                           \
    })
#define Hashmap_del(con, key) CN_rm_target(con, key, NULL)

static inline 
int hashmap_ikey_hasher (Tv key) {
    int k = t2i(key);
    return (k % HASHCODE_INDEX_SIZE);
}
#endif