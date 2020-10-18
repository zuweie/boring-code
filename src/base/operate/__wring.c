/*
 * @Author: your name
 * @Date: 2020-10-17 18:50:36
 * @LastEditTime: 2020-10-18 10:22:41
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/base/operate/__wring.c
 */
#include "base/__container.h"
#include "__wring.h"

int wring(container_t* container, int (*compare)(type_value_t, type_value_t), int (*callback)(void* vtype)) 
{
    iterator_t first = container_first(container);
    iterator_t next = iterator_next(first);
    while ( !iterator_is_tail(first) && !iterator_is_tail(next))
    {
        type_value_t v1 = iterator_dereference(first);
        type_value_t v2 = iterator_dereference(next);
        if (compare(v1, v2) == 0) {
            type_value_t rdata;
            if (container_remove(container, next, &rdata) == 0 && callback) {
                callback(&rdata);
            }
        } else {
            first = iterator_next(first);
        }
        next = iterator_next(first);
    }
    return 0;
}
