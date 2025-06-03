/*
 * @Author: your name
 * @Date: 2020-10-17 18:50:36
 * @LastEditTime: 2025-06-03 11:20:46
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/base/operate/__wring.c
 */
#include "base/__container.h"
#include "__wring.h"

int wring(container_t* container, int (*compare)(type_value_t*, type_value_t*), int (*callback)(void* vtype)) 
{
    iterator_t first = container_first(container);
    iterator_t next = iterator_next(first);
    while ( !iterator_equal(first, container_tail(container)) && !iterator_equal(next, container_tail(container)) )
    {
        type_value_t* v1 = first.reference;
        type_value_t* v2 = next.reference;
        if (compare(v1, v2) == 0) {
            type_value_t rdata[T_size(container->type_clazz)];
            if (container_remove(container, next, rdata) == 0 && callback) {
                callback(rdata);
            }
            next = iterator_next(first);
        } else {
            next = iterator_next(next);
            first = iterator_next(first);
        }
    }
    return 0;
}
