/*
 * @Description: 迭代器
 * @Author: zuweie
 * @Date: 2019-09-07 23:21:54
 * @LastEditTime: 2025-06-02 12:59:05
 * @LastEditors: zuweie jojoe.wei@gmail.com
 */
#ifndef __ITERATOR_H__
#define __ITERATOR_H__

//#include "__container.h"
#include "type_value/__type_value_def.h"
#include "type_value/__type_value.h"

typedef struct container container_t;
typedef struct iterator iterator_t;

#define iterator_assign(iter1, iter2) \
do{ \
    iterator_t t1 = (iter1); \
    iterator_t t2 = (iter2); \
    int ty_size = T_size(t1.container->type_clazz); \
    type_value_cpy(t1.reference, t2.reference, ty_size); \
} while(0)

#define iterator_exchange(iter1, iter2) \
do { \
    iterator_t t1 = (iter1); \
    iterator_t t2 = (iter2); \
    int ty_size = T_size(t1.container->type_clazz); \
    type_value_swap(t1.reference, t2.reference, ty_size); \
} while(0)

#define iterator_move(iter, step) ((iter).container->move(iter, step))
#define iterator_next(iter) iterator_move(iter, 1)
#define iterator_prev(iter) iterator_move(iter, -1)

#define iterator_equal(iter1, iter2) ((iter1).reference == (iter2).reference)
#define iterator_is_null(iter) ((iter).container == NULL || (iter).reference== NULL)

struct iterator {
    container_t* container;
    type_value_t* reference;
};

#define __iterator(__refer, __container) ((iterator_t){.container=(__container), .reference=(__refer)})
#define __null_iterator __iterator(NULL, NULL)

#endif