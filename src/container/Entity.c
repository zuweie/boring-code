/*
 * @Author: your name
 * @Date: 2020-10-24 10:20:46
 * @LastEditTime: 2020-10-26 20:53:45
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/Entity.c
 */
#include <stdarg.h>
#include <string.h>
#include "Entity.h"
#include "Tv.h"

void TempEntity(Entity* entity, int num, int value_index, Tv t[], ...) 
{
    va_list valist;
    for (int i=0; i<num; ++i) {
        v_type v = va_arg(valist, v_type);
        t[i] = i2t(v);
    }
    entity->number = num;
    entity->value_index = value_index;
    entity->tv = t;
    va_end(valist);
}

Entity* CopyALongTimeEntity(Entity* temp) 
{
    Entity *lentity = (Entity*) malloc(sizeof(Entity) + sizeof(Tv)*(temp->number));
    memcpy(lentity->tv, temp->tv, sizeof(Tv)*(temp->number));
    lentity->number = temp->number;
    lentity->value_index = temp->value_index;
    return lentity;
}

int CopyEntityValue(Entity* e1, Entity* e2) 
{
    if (e1->number == e2->number 
        && e1->value_index == e2->value_index 
        && e2->value_index < e2->number) {
        memcpy(&e1->tv[e1->value_index], &e2->tv[e2->value_index], sizeof(Tv) * (e2->number-e2->value_index)); 
        return 0;
    }
    return -1;
}

int EntityValueEqual(Entity* e1, Entity* e2) 
{
    if ( e1->value_index == e2->value_index ){

        for (int i=e1->value_index;i<e1->number; ++i) {
            if ( Tv_equl((e1->tv[i]), (e2->tv[i])) != 0) {
                return 1;
            }
        }
        return 0;
    }
    return 1;
}

int EntityKeyEqual(Entity* e1, Entity* e2) 
{
    if (e1->number == e2->number 
    && e1->value_index == e2->value_index ) {
        for (int i=0; i<e1->value_index; ++i) {
            if (Tv_equl(e1->tv[i], e2->tv[i]) != 0) {
                return 1;
            }
        }
        return 0;
    }
    return 1;
}