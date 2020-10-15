/*
 * @Author: your name
 * @Date: 2020-09-22 15:01:45
 * @LastEditTime: 2020-10-15 07:59:24
 * @LastEditors: your name
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/it.h
 */
#ifndef _IT_H_
#define _IT_H_
#include "base/__iterator.h"
#include "Tv.h"
typedef iterator_t It;
/* iterator_t function */
#define It_ref(iter) iterator_reference(iter)
#define It_dref(iter) iterator_dereference(iter)
#define It_move(it) iterator_move(iter, step)
#define It_next(iter) iterator_next(iter)
#define It_prev(iter) iterator_prev(iter)
#define It_equal(iter1, iter2) iterator_equal(iter1, iter2)
#define It_assign(from, to) iterator_assign(from, to)
#define It_exchange(iter1, iter2) iterator_exchange(iter1, itert2)

#define It_getV(it, vt) t2v(vt, It_dref(it))
#define It_getchar(it) It_getV(it, char)
#define It_getint(it) It_getV(it, int)
#define It_getfloat(it) It_getV(it, float)
#define It_getdouble(it) It_getV(it, double)
#define It_getptr(it) t2p(It_dref(it))

//#define ivalid(iter) iterator_valid(iter)
#define It_valid(it) _it_valid(it)
static inline 
int _it_valid(It iter) 
{
    It pos = iter;
    return iterator_valid(pos);
}

#endif