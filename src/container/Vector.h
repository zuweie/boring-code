/*
 * @Author: your name
 * @Date: 2020-10-13 07:09:37
 * @LastEditTime: 2021-06-29 13:23:39
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/LinearArray.h
 */
#ifndef _VECTOR_2_H_
#define _VECTOR_2_H_

#include "Cn.h"
#include "base/__vector.h"

typedef Container Vector;

#define _Vector(search_cmp) \
    ({                   \
        Vector vet;      \
        CN_initialize(&vet, vector, search_cmp, NULL, NULL); \
        vet; \
    })

#define Vector_(vet_ptr, cleanup) CN_uninitialize(vet_ptr, vector, cleanup)

#endif
