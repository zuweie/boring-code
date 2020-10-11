/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-07 23:21:54
 * @LastEditTime: 2020-09-22 14:28:03
 * @LastEditors: Please set LastEditors
 */
#ifndef _ITERATOR_H_
#define _ITERATOR_H_

#include "__container.h"
#include "__type_value.h"

#define iterator_reference(iter) (iter.reference)
#define iterator_set_reference(iter, refer) (((iter).reference=(refer))?(iter):(iter))
#define iterator_container(iter) (iter.container)
#define iterator_dereference(iter) (*((type_value_t*)iterator_reference(iter)))

#define iterator_assign(to, from) (iterator_dereference(to)=iterator_dereference(from))

#define iterator_exchange(t1, t2) do { \
    type_value_t t = iterator_dereference(t1); \
    iterator_assign(t1, t2);                   \
    iterator_dereference(t2) = t;              \
}while(0)

#define iterator_move(iter, step) iter.move(iter, step)
#define iterator_next(iter) iterator_move(iter, 1)
#define iterator_prev(iter) iterator_move(iter, -1)

#define iterator_equal(iter1, iter2) (iterator_reference(iter1) == iterator_reference(iter2))

#define iterator_is_head(iter) iterator_equal(iter, container_head(iter.container))
#define iterator_is_tail(iter) iterator_equal(iter, container_tail(iter.container))
#define iterator_is_boundary(iter) (iterator_is_tail(iter) || iterator_is_head(iter))
#define iterator_valid(iter) !iterator_is_boundary(iter)

typedef struct _iterator iterator_t;
struct _iterator {
    iterator_t (*move)(iterator_t, int step);
    void* reference;
    void* container;
};

static inline 
iterator_t get_iterator(void* __refer, void* __container, iterator_t (*__move)(iterator_t, int))
{
    iterator_t it = {
        .move      = (__move),
        .reference = (__refer),
        .container = (__container)
    };
    return it;
}

#endif