/*
 * @Author: your name
 * @Date: 2020-06-02 06:50:22
 * @LastEditTime: 2020-06-11 10:45:36
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /kitc/src/container/tv.h
 */ 

#ifndef _TV_H_
#define _TV_H_

#include "base/__type_value.h"

typedef type_value_t tv;

#define t2v(vt, t) type_v(vt, t)
#define t2i(t) type_int(t)
#define t2f(t) type_float(t)
#define t2d(t) type_double(t)
#define t2p(t) type_pointer(t)

#define i2t(v) int_type(v)
#define f2t(v) float_type(v)
#define d2t(v) double_type(v)
#define p2t(v) pointer_type(v)

#define cmpi(tv1, tv2) compare_int(tv1, tv2)
#define cmpf(tv1, tv2) compare_float(tv1, tv2)
#define cmpd(tv1, tv2) compare_double(tv1, tv2)
#define cmpptr(tv1, tv2) compare_pointer(tv1, tv2)

#endif