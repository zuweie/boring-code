/*
 * @Description: 迭代器
 * @Author: zuweie
 * @Date: 2019-09-07 23:21:54
 * @LastEditTime: 2021-10-25 14:34:25
 * @LastEditors: Please set LastEditors
 */
#ifndef __ITERATOR_H__
#define __ITERATOR_H__

#include "__container.h"
#include "type_value/__type_value_def.h"
#include "type_value/__type_value.h"

#define iterator_assign(iter1, iter2) \
({ \
    iter1.container->type_def.ty_adapter.bit_cpy(iter1.refer, iter2.refer);\
})

#define iterator_exchange(itert1, itert2) \
({ \
    container_t* cn = iter1.container; \
    T_def* _def     = &cn->type_def;   \
    type_value_t tmp[_def->ty_size];   \
    _def->ty_adapter.bit_cpy(tmp, iter1.reference);         \
    _def->ty_adapter.bit_cpy(iter1.refer, iter2.reference); \
    _def->ty_adapter.bit_cpy(iter2.reference, tmp);         \
})

#define iterator_move(piter, step) ((piter)->container->move(piter, step))
#define iterator_next(iter) iterator_move(&iter, 1)
#define iterator_prev(iter) iterator_move(&iter, -1)

#define iterator_equal(iter1, iter2) ((iter1).reference == (iter2).reference)

#define iterator_is_head(iter) iterator_equal(iter, container_head(iter.container))
#define iterator_is_tail(iter) iterator_equal(iter, container_tail(iter.container))
#define iterator_is_boundary(iter) (iterator_is_tail(iter) || iterator_is_head(iter))
#define iterator_valid(iter) !iterator_is_boundary(iter)
#define iterator_is_null(iter) ((iter).container == NULL)

typedef struct _container container_t;
typedef struct _iterator iterator_t;

struct _iterator {
    container_t* container;
    type_value_t* reference;
};

#define __iterator(__refer, __container) ({ iterator_t it = { .reference = (__refer), .container = (__container),}; it;})
#define __null_iterator ({iterator_t it = {.reference = NULL, .container = NULL}; it;})
#endif