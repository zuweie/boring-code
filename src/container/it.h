/*
 * @Author: your name
 * @Date: 2020-09-22 15:01:45
 * @LastEditTime: 2020-10-15 15:22:23
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/it.h
 */
#ifndef _IT_H_
#define _IT_H_
#include "base/__iterator.h"
#include "Tv.h"
typedef iterator_t It;
/* iterator_t interface */
#define It_ref(iter) iterator_reference(iter)
#define It_dref(iter) iterator_dereference(iter)
#define It_move(it) iterator_move(iter, step)
#define It_next(iter) iterator_next(iter)
#define It_prev(iter) iterator_prev(iter)
#define It_equal(iter1, iter2) iterator_equal(iter1, iter2)
#define It_assign(from, to) iterator_assign(from, to)
#define It_exchange(iter1, iter2) iterator_exchange(iter1, itert2)

#define It_getchar(it) t2i(It_dref(it))
#define It_getint(it) t2i(It_dref(it))
#define It_getfloat(it) t2f(It_dref(it))
#define It_getdouble(it) t2f(It_dref(it))
#define It_getptr(it) t2p(It_dref(it))

#define It_valid(it)                  \
    ({                                \
        int ret = iterator_valid(it); \
        ret;                          \
    })

#endif