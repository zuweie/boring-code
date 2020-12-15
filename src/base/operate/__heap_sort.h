/*
 * @Author: your name
 * @Date: 2020-12-02 12:13:47
 * @LastEditTime: 2020-12-15 09:03:36
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/base/operate/__head_sort.h
 */

#ifndef _HEAP_SORT_H_
#define _HEAP_SORT_H_

#define heap_parent(k) ((k-1)>>1)
#define heap_left(k)   ((k<<1)+1)
#define heap_right(k)  ((k<<1)+2)

int heap_max_heapify(container_t*, size_t, size_t, int (*)(type_value_t, type_value_t));
int heap_build_max_heap(container_t*, int (*)(type_value_t, type_value_t));
int heap_sort(container_t*, int(*)(type_value_t, type_value_t));

#endif