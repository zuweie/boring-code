/*
 * @Author: your name
 * @Date: 2021-10-21 13:39:44
 * @LastEditTime: 2025-06-09 12:07:08
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/It.h
 */
#ifndef __ITER_H__
#define __ITER_H__
// #include "cn.h"
#include "Typ.h"
#include "base/__iterator.h"
#include "base/__container.h"

typedef struct Cn Cn;
typedef struct Iter Iter;

#define It(iter, cn_ptr)  ((Iter){._iter=(iter), ._cn_ptr=(cn_ptr)}) 
#define It_move(it, step) It(iterator_move(it._iter, it._cn_ptr->is_forward? (step) : -(step)), it._cn_ptr) 
#define It_next(it)       It_move(it, 1)
#define It_prev(it)       It_move(it, -1)
#define It_refer(it)      (it._iter.reference)

#define It_char(it)   T_char(it._iter.reference)
#define It_uchar(it)  T_uchar(it._iter.reference)
#define It_int(it)    T_int(it._iter.reference)
#define It_uint(it)   T_uint(it._iter.reference)
#define It_long(it)   T_long(it._iter.reference)
#define It_ulong(it)  T_ulong(it._iter.reference)
#define It_float(it)  T_float(it._iter.reference)
#define It_double(it) T_double(it._iter.reference)
#define It_ptr(it)    T_ptr(it._iter.reference)
#define It_str(it)    T_str(it._iter.reference)

// #define It_is_tail(it) (it._cn_ptr->is_forward ? iterator_is_tail((it)._iter) : iterator_is_head((it)._iter))
// #define It_is_head(it) (it._cn_ptr->is_forward ? iterator_is_head((it)._iter) : iterator_is_tail((it)._iter))
#define It_equal(it1, it2) iterator_equal((it1)._iter, (it2)._iter)
#define It_is_tail(it) iterator_equal(it._iter, container_tail(it._iter.container))
#define It_is_head(it) iterator_equal(it._iter, container_head(it._iter.container))
#define It_null ((Iter){._iter=__null_iterator, ._cn_ptr = NULL})

struct Iter {
    iterator_t _iter;
    Cn*        _cn_ptr;
};


#endif
