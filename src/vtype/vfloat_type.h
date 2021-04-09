/*
 * @Author: your name
 * @Date: 2021-04-08 14:08:25
 * @LastEditTime: 2021-04-08 16:57:50
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/type/float_type.h
 */
#ifndef _FLOAT_TYPE_H_
#define _FLOAT_TYPE_H_

#define __LONG_FLOAT__ 0

#if __LONG_FLOAT__ 
    typedef double vfloat_t;
#else
    typedef float vfloat_t;
#endif

#endif