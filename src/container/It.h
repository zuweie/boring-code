/*
 * @Author: your name
 * @Date: 2021-10-21 13:39:44
 * @LastEditTime: 2021-10-25 14:58:19
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/It.h
 */
#ifndef __IT_H__
#define __IT_H__
#include "cn.h"
#include "ty.h"

#include "base/__iterator.h"
#define It(iter, cn) ({It marco_##iter = {._iter = iter, ._cn = cn}; marco_##iter;})
#define It_move(it, step) iterator_move(&it._iter, step)
#define It_next(it) It_move(it, 1)
#define It_prev(it) It_move(it, -1)
#define It_refer(it) (it._iter.reference)

#define It_char(it) T_char(it._iter.reference)
#define It_uchar(it) T_uchar(it._iter.reference)
#define It_short(it) T_short(it._iter.reference)
#define It_ushort(it) T_ushort(it._iter.reference)
#define It_int(it) T_int(it._iter.reference)
#define It_uint(it) T_uint(it._iter.reference)
#define It_long(it) T_long(it._iter.reference)
#define It_ulong(it) T_ulong(it._iter.reference)
#define It_ptr(it) T_ptr(it._iter.reference)
#define It_str(it) T_str(it._iter.reference)
#define It_is_tail(it) iterator_is_tail((it)._iter)
#define It_is_head(it) iterator_is_head((it)._iter)
#define It_null ({It it={._iter=__null_iterator(), ._cn = 0}; it;})
typedef int CN;
typedef struct _It {
    iterator_t _iter;
    CN _cn;
} It;


#endif
