/*
 * @Author: your name
 * @Date: 2020-10-19 17:09:03
 * @LastEditTime: 2020-10-19 17:09:56
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/base/__entitiy.h
 */
#ifndef _ENTITY_H_
#define _ENTITY_H_
#include "__type_value.h"
typedef struct _entity 
{
    type_value_t key;
    type_value_t value;
} entity_t;

#endif