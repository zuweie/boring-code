/*
 * @Author: your name
 * @Date: 2020-10-23 13:08:02
 * @LastEditTime: 2020-10-24 10:58:06
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/Entity.h
 */
#ifndef _ENTITY_H_
#define _ENTITY_H_
#include "Tv.h"

typedef struct {
    Tv e[0];
} Entity;

Entity* _Entity(int num, ...);
#endif