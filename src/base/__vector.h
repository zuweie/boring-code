/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-07 23:28:17
 * @LastEditTime: 2021-11-01 11:03:38
 * @LastEditors: Please set LastEditors
 */
#ifndef __VECTOR_H__
#define __VECTOR_H__

#include "__container.h"
#include "type_value/__type_value.h"
#include "type_value/__type_value_def.h"

#define VEC_ALLOC_CHUNK_SIZE 128

typedef struct _vector
{
   container_t container;
   type_value_t* _data;
   int _size;
   int _capacity;
   
} vector_t;

container_t* vector_create(T_clazz* __type_clazz);
int vector_destroy(container_t*);
#endif