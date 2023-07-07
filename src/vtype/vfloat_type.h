/*
 * @Author: your name
 * @Date: 2021-04-08 14:08:25
 * @LastEditTime: 2023-07-07 16:50:26
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/type/float_type.h
 */
#ifndef _FLOAT_TYPE_H_
#define _FLOAT_TYPE_H_

#define __LONG_FLOAT__ 1

#if __LONG_FLOAT__ 
    typedef double vfloat_t;
#else
    typedef float vfloat_t;
#endif

#endif