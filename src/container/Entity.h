/*
 * @Author: your name
 * @Date: 2020-10-23 13:08:02
 * @LastEditTime: 2020-10-27 21:59:33
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/Entity.h
 */
#ifndef _ENTITY_H_
#define _ENTITY_H_
#include <stdlib.h>
#include "Tv.h"

typedef struct _entity {
    size_t value_index;
    size_t number;
    Tv* tv;
} Entity;

void TempEntity(Entity*, int num, int value_index, Tv t[],  ...);

// 这个需要手动释放内存。
Entity* CopyALongTimeEntity(Entity*);

int CopyEntityValue(Entity*, Entity*);
int EntityValueEqual(Entity* e1, Entity* e2);
int EntityKeyEqual(Entity* e1, Entity* e2);
#endif