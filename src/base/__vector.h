/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-07 23:28:17
 * @LastEditTime: 2020-10-11 18:00:10
 * @LastEditors: Please set LastEditors
 */
#ifndef _VECTOR_H_
#define _VECTOR_H_

#include "__container.h"
#include "__type_value.h"

#define VEC_ALLOC_CHUNK_SIZE 128

typedef struct _vector
{
   container_t container;
   type_value_t* _data;
   size_t _size;
   size_t _capacity;
   
} vector_t;

container_t* vector_create();
int vector_destroy(container_t*);
#endif