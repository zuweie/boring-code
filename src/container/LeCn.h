/*
 * @Author: your name
 * @Date: 2020-12-11 08:25:16
 * @LastEditTime: 2021-07-07 10:38:53
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/MtCn.h
 */

/* 线性的 entity 容器集合 */

#ifndef _LE_CN_H_
#define _LE_CN_H_
#include "Cn.h"
#include "Entity.h"

#define LeCN_init(lecon_ptr, con_clazz, entity_search_cmp, ...) CN_initialize(lecon_ptr, con_clazz, entity_search_cmp, NULL, NULL, __VA_ARGS__)

#define LeCN_uninit(lecon_ptr, con_clazz) CN_uninitialize(lecon_ptr, con_clazz, CLEANUP_ENTITY)

#define LeCN_addx(lecon_ptr, x, y, ...) do { \
    Entity* __p_marco_entity = Entity_heap(x, y, __VA_ARGS__); \
    CN_add(lecon_ptr, p2t(__p_marco_entity)); \
} while (0)

#define LeCN_add(lecon_ptr, key) LeCN_addx(lecon_ptr, 1, 0, key)
#define LeCN_add2(lecon_ptr, key, value) LeCN_addx(lecon_ptr, 2, 1, key, value)

#define LeCN_findx(lecon_ptr, num, ...) \
    ({ \
        Tv __marco_tv[num]; \
        Entity __marco_entity; \
        Entity_temp(&__marco_entity, num, num, __marco_tv, __VA_ARGS__); \
        It find_it = CN_find(lecon_ptr, p2t(&__marco_entity)); \
        find_it; \
    })

#define LeCN_find(lecon_ptr, tv) LeCN_findx(lecon_ptr, 1, tv)
#define LeCN_find2(lecon_ptr, tv1, tv2) LeCN_findx(lecon_ptr, 2, tv1, tv2)
#define LeCN_find3(lecon_ptr, tv1, tv2, tv3) LeCN_findx(lecon_ptr, 3, tv1, tv2, tv3)

#define LeCN_remove(lecon_ptr, it, prentity) \
    ({  \
        int ret = -1; \
        Tv __marco_tv; \
        if ( (ret = CN_remove(lecon_ptr, it, &__marco_tv)) != -1) { \
            Entity* __p_marco_rentity = prentity; \
            Entity* __p_marco_del_entity = p2t(__marco_tv); \
            if (__p_marco_rentity) { \
                Entity_cypto_entity(__p_marco_del_entity, __p_marco_rentity); \
            } \
            free(entity); \
        } \
        ret; \
    })

#define LeCN_get_entity(lecon, it) \
    ({ \
        Entity* __p_marco_entity = NULL; \
        if (It_valid(it)) { \
            __p_marco_entity = It_getptr(it); \
        } \
        __p_marco_entity; \
    })

static void
CLEANUP_ENTITY(Tv v) 
{
    Entity* entity = t2p(v);
    free(entity);
}
#endif