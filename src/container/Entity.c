/*
 * @Author: your name
 * @Date: 2020-10-24 10:20:46
 * @LastEditTime: 2020-11-22 22:53:31
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/Entity.c
 */
#include <stdarg.h>
#include <string.h>
#include "Entity.h"
#include "Tv.h"

void Entity_temp(Entity* entity, int num, int value_index, Tv t[], ...) 
{
    va_list valist;
    va_start(valist, t);
    
    for (int i=0; i<num; ++i) {
        Tv v = va_arg(valist, Tv);
        t[i] = v;
    }
    entity->number = num;
    entity->value_index = value_index;
    entity->tv = t;
    va_end(valist);
}

Entity* Entity_cpyto_heap_entity(Entity* temp) 
{
    Entity *lentity = (Entity*) malloc(sizeof(Entity) + sizeof(Tv)*(temp->number));
    memcpy(&lentity[1], temp->tv, sizeof(Tv)*(temp->number));
    lentity->number = temp->number;
    lentity->value_index = temp->value_index;
    lentity->tv = &lentity[1];
    return lentity;
}

int Entity_copy_Value(Entity* e1, Entity* e2) 
{
    if (e1->number == e2->number 
        && e1->value_index == e2->value_index 
        && e2->value_index < e2->number) {
        memcpy(&e1->tv[e1->value_index], &e2->tv[e2->value_index], sizeof(Tv) * (e2->number-e2->value_index)); 
        return 0;
    }
    return -1;
}

int Entity_is_value_equal(Entity* e1, Entity* e2) 
{
    if ( e1->value_index == e2->value_index && e1->number == e2->number){

        for (int i=e1->value_index;i<e1->number; ++i) {
            if ( Tv_equl((e1->tv[i]), (e2->tv[i])) != 0) {
                return 1;
            }
        }
        return 0;
    }
    return 1;
}

int Entity_is_key_equal(Entity* e1, Entity* e2) 
{
    if ( e1->value_index == e2->value_index ) {
        for (int i=0; i<e1->value_index; ++i) {
            if (Tv_equl(e1->tv[i], e2->tv[i]) != 0) {
                return 1;
            }
        }
        return 0;
    }
    return 1;
}

