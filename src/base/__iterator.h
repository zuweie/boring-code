/*
 * @Description: 迭代器
 * @Author: zuweie
 * @Date: 2019-09-07 23:21:54
 * @LastEditTime: 2021-10-18 17:23:37
 * @LastEditors: Please set LastEditors
 */
#ifndef __ITERATOR_H__
#define __ITERATOR_H__

#include "__container.h"
#include "type_value\__type_value_def.h"
#include "type_value\__type_value.h"

#define iterator_reference(iter) ((iter).reference)
#define iterator_container(iter) ((iter).container)
#define iterator_assign(iter1, iter2) \
({ \
    container_t* cn = iter1.container; \
    T_def* _def = &cn->type_def;       \
    _def->ty_adapter.bit_cpy(iter1.refer, iter2.refer);\
})

#define iterator_exchange(itert1, itert2) \
({ \
    container_t* cn = iter1.container; \
    T_def* _def     = &cn->type_def;   \
    type_value_t tmp[_def->ty_size];   \
    _def->ty_adapter.bit_cpy(tmp, iter1.refer);         \
    _def->ty_adapter.bit_cpy(iter1.refer, iter2.refer); \
    _def->ty_adapter.bit_cpy(iter2.refer, tmp);         \
})

#define iterator_move(piter, step) ((piter)->container.move(piter, step))
#define iterator_next(iter) iterator_move(&iter, 1)
#define iterator_prev(iter) iterator_move(&iter, -1)

#define iterator_equal(iter1, iter2) (iterator_reference(iter1) == iterator_reference(iter2))

#define iterator_is_head(iter) iterator_equal(iter, container_head(iter.container))
#define iterator_is_tail(iter) iterator_equal(iter, container_tail(iter.container))
#define iterator_is_boundary(iter) (iterator_is_tail(iter) || iterator_is_head(iter))
#define iterator_valid(iter) !iterator_is_boundary(iter)

typedef struct _iterator iterator_t;
struct _iterator {
    container_t* container;
    type_value_t* refer;
};

#define __iterator(__refer, __container) \
    ({ \
        iterator_t it = { \
            .reference = (__refer), \
            .container = (__container), \
        }; \
        it; \
    })
#endif