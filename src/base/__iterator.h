/*
 * @Description: 迭代器
 * @Author: zuweie
 * @Date: 2019-09-07 23:21:54
 * @LastEditTime: 2021-10-15 15:55:28
 * @LastEditors: Please set LastEditors
 */
#ifndef __ITERATOR_H__
#define __ITERATOR_H__

#include "__container.h"
#include "type_value\__type_value_def.h"
#include "type_value\__type_value.h"

#define iterator_reference(iter) ((iter).reference)
#define iterator_container(iter) ((iter).container)

#define iterator_exchange(itert1, itert2) \
({ \
    container_t* cn = iter1.container; \
    type_value_t* t1 = iter1.refer;    \
    type_value_t* t2 = iter2.refer;    \
    T_def* _def     = &cn->type_def;   \
    type_value_t tmp[_def->ty_size];    \
    _def->ty_adapter.bit_cpy(tmp, t1);  \
    _def->ty_adapter.bit_cpy(t1, t2);   \
    _def->ty_adapter.bit_cpy(t2, tmp);  \
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
    void* refer;
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