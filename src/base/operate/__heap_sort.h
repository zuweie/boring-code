/*
 * @Author: your name
 * @Date: 2020-12-02 12:13:47
 * @LastEditTime: 2020-12-02 23:28:30
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/base/operate/__head_sort.h
 */

#ifndef _HEAP_SORT_H_
#define _HEAP_SORT_H_

#define heap_parent(k) (k/2)
#define heap_left(k) (2*k)
#define heap_right(k) (2*k+1)

int heap_max_heapify(container_t*, size_t, size_t, int (*)(type_value_t, type_value_t));
int heap_build_max_heap(container_t*, int (*)(type_value_t, type_value_t));
int heap_sort(container_t*, int(*)(type_value_t, type_value_t));

#endif