/*
 * @Author: your name
 * @Date: 2020-12-26 09:33:18
 * @LastEditTime: 2021-04-08 15:07:18
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/complex/complex.h
 */
#ifndef _COMPLEX_H_
#define _COMPLEX_H_
#include <math.h>
#include "boring_type/vfloat_type.h"
typedef struct  _complex
{
    vfloat_t real;
    vfloat_t image;
} complex_t;

#define _complex(r, i) \
    ({ \
        complex_t complex = {.real = (r), .image = (i)}; \
        complex; \
    })

#define complex_w(N, k) _complex(cos(2*3.1415926535898/N*k), sin(2*3.1415926535898/N*k))

#define complex_rw(N, k) _complex(cos(2*3.1415926535898/N*k), -1 * sin(2*3.1415926535898/N*k))

#define complex_sum(c1, c2) _complex(c1.real+c2.real, c1.image+c2.image)

#define complex_sub(c1, c2) _complex(c1.real-c2.real, c1.image-c2.image)

#define complex_multiply(c1, c2) _complex(c1.real*c2.real-c1.image*c2.image, c1.image*c2.real+c1.real*c2.image)

#define complex_div(c1, c2) _complex((c1.real*c2.real+c1.image*c2.image)/(c2.real*c2.real+c2.image*c2.image)), (c1.image*c2.real-c1.real*c2.image)/(c2.real*c2.real+c2.image*c2.image))

#define complex_pow(c) (c.real*c.real + c.image*c.image)

#define complex_absolute(c) (sqrt(complex_pow(c)))

#endif   