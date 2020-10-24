/*
 * @Author: your name
 * @Date: 2020-10-24 10:20:46
 * @LastEditTime: 2020-10-24 10:58:47
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/Entity.c
 */
#include <stdarg.h>
#include "Entity.h"

Entity* _Entity(int num, ...) 
{
    Entity* entity = malloc (sizeof(Entity)*num);
    va_list valist;

    for (int i=0; i<num; ++i) {
        v_type v = va_arg(valist, v_type);
        entity->e[i] = i2t(v);
    }
    
    va_end(valist);

    return entity;
}