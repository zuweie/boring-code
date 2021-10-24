/*
 * @Author: your name
 * @Date: 2020-10-24 10:20:46
 * @LastEditTime: 2020-12-14 08:33:46
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/Entity.c
 */
#include <stdarg.h>
#include <string.h>
#include "Entity.h"
#include "Tv.h"

/**
 * 
 * 记录一下可变参数的原理：
 * 1 va_list 其实就是 void*
 * 2 va_start(va_list, last_arg_before_dot_dot_dot), 就是获取了 &last_arg_before_dot_dot_dot 的地址。
 * 3 va_arg(va_list, type_you_dig) , 根据 type_you_dig 的数据类型(大小,size)。往 &last_arg_before_dot_dot_dot 的地址后面挖数据然后返回。
 * 4 va_end(va_list) 把 va_list 置 0
 * */

Entity* Entity_temp(Entity* entity, int num, int value_index, Tv t[], ...) 
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
    return entity;
}

Entity* Entity_heap(int num, int value_index, ...) 
{
    Entity* entity = (Entity*) malloc (sizeof(Entity) + sizeof(Tv)*num);
    entity->number = num;
    entity->value_index = value_index;
    entity->tv = &entity[1];

    va_list valist;
    va_start(valist, value_index);
    for (int i=0; i<num; ++i) {
        Tv v = va_arg(valist, Tv);
        entity->tv[i] = v;
    }
    return entity;
}

void Entity_cypto_entity(Entity* from, Entity* to) {
    memcpy(to->tv, from->tv, sizeof(Tv)*(from->number));
    to->number      = from->number;
    to->value_index = from->value_index;
    return;
}

Entity* Entity_malloc_copy_entity(Entity* from) 
{
    Entity *to = (Entity*) malloc(sizeof(Entity) + sizeof(Tv)*(from->number));
    to->tv = &to[1];
    Entity_cypto_entity(from, to);
    return to;
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

