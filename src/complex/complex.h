/*
 * @Author: your name
 * @Date: 2020-12-26 09:33:18
 * @LastEditTime: 2020-12-29 08:44:28
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/complex/complex.h
 */
#ifndef _COMPLEX_H_
#define _COMPLEX_H_

typedef struct  _complex
{
    double real;
    double image;
} complex_t;

#define _complex(r, i) \
    ({ \
        complex_t complex = {.real = r, .image = i}; \
        complex; \
    })
#endif   