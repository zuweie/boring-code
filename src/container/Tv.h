/*
 * @Author: your name
 * @Date: 2020-06-02 06:50:22
 * @LastEditTime: 2021-06-29 13:39:26
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /kitc/src/container/tv.h
 */ 

#ifndef _TV_H_
#define _TV_H_

#include "base/__type_value.h"

typedef type_value_t Tv;

#define t2t(t) (t)

#define t2i(t) vtype_int(t)
#define t2f(t) vtype_float(t)
#define t2d(t) vtype_float(t)
#define t2p(t) vtype_pointer(t)

#define i2t(v) int_vtype(v)
#define f2t(v) float_vtype(v)
#define d2t(v) float_vtype(v)
#define p2t(v) pointer_vtype(v)

#define Tv_equl(tv1, tv2) vtype_equl(tv1, tv2)
#define Tv_cmpi(tv1, tv2) vtype_cmp_on_int(tv1, tv2)
#define Tv_cmpf(tv1, tv2) vtype_cmp_on_flt(tv1, tv2)
#define Tv_cmpd(tv1, tv2) vtype_cmp_on_flt(tv1, tv2)
#define Tv_cmpptr(tv1, tv2) vtype_cmp_on_ptr(tv1, tv2)

#endif