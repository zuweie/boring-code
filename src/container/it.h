/*
 * @Author: your name
 * @Date: 2020-05-30 08:54:21
 * @LastEditTime: 2020-06-14 08:40:36
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /kitc/src/container/iterator.h
 */ 

#ifndef _IT_H_
#define _IT_H_
#include "base/__iterator.h"
#include "tv.h"
typedef iterator_t it;
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
int _it_valid(it iter) 
{
    it pos = iter;
    return iterator_valid(pos);
}
#endif