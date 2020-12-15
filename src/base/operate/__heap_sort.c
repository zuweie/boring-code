/*
 * @Author: your name
 * @Date: 2020-12-02 14:59:35
 * @LastEditTime: 2020-12-15 08:58:13
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/base/operate/__heap_sort.c
 */
#include "base/__container.h"
#include "__heap_sort.h"

int heap_max_heapify(container_t* container_ptr, size_t i, size_t heap_size, int (*compare_to)(type_value_t, type_value_t))
{
    size_t left  = heap_left(i);
    size_t right = heap_right(i);
    size_t largest = -1;

    type_value_t v_left = container_fetch(container_ptr, left);
    type_value_t v_i    = container_fetch(container_ptr, i);
    
    if ( left < heap_size && compare_to(v_left, v_i) == 1 ) {
        largest = left;
    } else {
        largest = i;
    } 
    type_value_t v_right   = container_fetch(container_ptr, right);
    type_value_t v_largest = container_fetch(container_ptr, largest);

    if ( right < heap_size &&  compare_to(v_right, v_largest) == 1) {
        largest = right;
    }

    if (largest != i) {
        iterator_t i_it       = container_access(container_ptr, i);
        iterator_t largest_it = container_access(container_ptr, largest);
        iterator_exchange(i_it, largest_it);
        return heap_max_heapify(container_ptr, largest, heap_size, compare_to);
    }
    return 0;
}

int heap_build_max_heap(container_t* container_ptr, int (*compare_to)(type_value_t, type_value_t))
{
    size_t heap_size = container_size(container_ptr);
    for (int i = heap_size / 2 - 1; i >= 0; --i) {
        heap_max_heapify(container_ptr, i, heap_size, compare_to);
    }
    return 0;
}

int heap_sort(container_t* container_ptr, int (*compare_to)(type_value_t, type_value_t))
{
    heap_build_max_heap(container_ptr, compare_to);
    size_t _size = container_size(container_ptr);
    size_t heap_size = _size;
    for (int i = _size-1; i >= 1; --i ) {
        iterator_t it1 = container_access(container_ptr, 0);
        iterator_t it2 = container_access(container_ptr, i);
        
        iterator_exchange(it1, it2);

        heap_max_heapify(container_ptr, 0, --heap_size, compare_to);
    }
    return 0;
}