/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-07 23:21:46
 * @LastEditTime: 2020-11-19 12:52:28
 * @LastEditors: Please set LastEditors
 */
#ifndef _CONTAINER_H_
#define _CONTAINER_H_

#include <stddef.h>

#include "__type_value.h"
#include "__iterator.h"
#include "mem_pool/__mem_pool.h"
typedef struct _iterator iterator_t;

#define container_create(container_label, ...) container_label##_create(__VA_ARGS__)
#define container_destroy(conatainer_label, ...) conatainer_label##_destroy(__VA_ARGS__)

// 容器位置
#define container_first(container_ptr) (((container_t*)(container_ptr))->first((container_t*)(container_ptr)))
#define container_last(container_ptr) (((container_t*)(container_ptr))->last((container_t*)(container_ptr)))
#define container_head(container_ptr) iterator_prev(container_first(container_ptr))
#define container_tail(container_ptr) iterator_next(container_last(container_ptr))

// 容器搜索
#define container_search(container_ptr, offset, find, compare) \
    (((container_t*)(container_ptr))->search(((container_t*)(container_ptr)), offset, find, compare))

#define container_find(container_ptr, find, compare) \
    container_search(container_ptr, container_first(container_ptr), find, compare)

// 根据插入的东东计算插入的位置。不需要 iterator 来指明，有特殊的数据结构，例如红黑树，hashmap
#define container_set(container_ptr, data, setup, conflict_fix) \
    (((container_t*)(container_ptr))->set(((container_t*)(container_ptr)), data, setup, conflict_fix))

// 容器插入
#define container_insert(container_ptr, iter, data) \
    (((container_t*)(container_ptr))->insert(((container_t*)(container_ptr)), iter, data))

#define container_insert_find(container_ptr, find, data, compare) \
    container_insert(container_ptr, container_find(container_ptr, find, compare), data)

#define container_insert_first(container_ptr, data) \
    container_insert(container_ptr, container_first(container_ptr), data)

#define container_insert_tail(container_ptr, data) \
    container_insert(container_ptr, container_tail(container_ptr), data)

// 容器移除
#define container_remove(container_ptr, iter, rdata) \
    (((container_t*)(container_ptr))->remove(((container_t*)(container_ptr)), iter, rdata))

#define container_remove_find(container_ptr, find, compare, rdata) \
    container_remove(container_ptr, container_find(container_ptr, find, compare, rdata)) 

#define container_remove_first(container_ptr, rdata) \
    container_remove(container_ptr, container_first(container_ptr), rdata)

#define container_remove_last(container_ptr, rdata) \
    container_remove(container_ptr, container_last(container_ptr), rdata)

// // 容器测试
// #define container_has(container_ptr, find, compare) \
//     ({ \
//         itreator_t it = container_find(container_ptr, find, compare); \
//         int ret = !iterator_is_boundary(it); \
//         ret; \
//     })
   

// 容器排序
#define container_sort(container_ptr, compare) \
    ((container_t*)(container_ptr))->sort((container_t*)(container_ptr), compare)

// 容器挤水
#define container_wring(container_ptr, compare, callback) \
    (((container_t*)(container_ptr))->wring(((container_t*)(container_ptr)), compare, callback))

// 容器的内存池
#define container_mem_pool(container_ptr) (((container_t*)(container_ptr))->mem_pool)

#define container_size(container_ptr) (((container_t*)(container_ptr))->size((container_t*)container_ptr))

#define initialize_container(container_ptr, __first, __last, __search, __set, __insert, __remove, __sort, __wring, __size, __mem_pool) do { \
    ((container_t*)(container_ptr))->first  = (__first);                                        \
    ((container_t*)(container_ptr))->last   = (__last);                                         \
    ((container_t*)(container_ptr))->search = (__search);                                       \
    ((container_t*)(container_ptr))->set    = (__set);                                          \
    ((container_t*)(container_ptr))->insert = (__insert);                                       \
    ((container_t*)(container_ptr))->remove = (__remove);                                       \
    ((container_t*)(container_ptr))->sort   = (__sort);                                         \
    ((container_t*)(container_ptr))->wring  = (__wring);                                        \
    ((container_t*)(container_ptr))->size   = (__size);                                         \
    ((container_t*)(container_ptr))->mem_pool = (__mem_pool);                                   \
} while (0)

typedef struct _container container_t;

struct _container {
    iterator_t (*first) (container_t* container_ptr);   
    iterator_t (*last) (container_t * container_ptr);   
    iterator_t (*search) (container_t* container_ptr, iterator_t offset, type_value_t find, int (*compare)(type_value_t, type_value_t)); 
    int (*set) (container_t* container_ptr, type_value_t data, int(*setup)(type_value_t*, type_value_t), int (*conflict_fix) (type_value_t*, type_value_t));
    int (*insert) (container_t* container_ptr, iterator_t iter, type_value_t data); 
    int (*remove) (container_t* container_ptr, iterator_t iter, void* rdata);
    int (*sort) (container_t* container_ptr, int(*compare)(type_value_t, type_value_t));
    int (*wring) (container_t* container_ptr, int(*compare)(type_value_t, type_value_t), int(*callback)(void*));
    size_t (*size) (container_t* container_ptr);
    pool_t* mem_pool;
};

#endif