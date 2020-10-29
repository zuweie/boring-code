/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-07 23:21:46
 * @LastEditTime: 2020-10-29 10:22:28
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
#define container_first(container) (((container_t*)(container))->first((container_t*)(container)))
#define container_last(container) (((container_t*)(container))->last((container_t*)(container)))
#define container_head(container) iterator_prev(container_first(container))
#define container_tail(container) iterator_next(container_last(container))

// 容器搜索
#define container_search(container, offset, find, compare) (((container_t*)(container))->search(((container_t*)(container)), offset, find, compare))
#define container_find(container, find, compare) container_search(container, container_first(container), find, compare)

// 根据插入的东东计算插入的位置。不需要 iterator 来指明，有特殊的数据结构，例如红黑树，hashmap
#define container_set(container, data, setup, conflict_fix) (((container_t*)(container))->set(((container_t*)(container)), data, setup, conflict_fix))

// 容器插入
#define container_insert(container, iter, data) (((container_t*)(container))->insert(((container_t*)(container)), iter, data))
#define container_insert_find(container, find, data, compare) container_insert(container, container_find(container, find, compare), data)
#define container_insert_first(container, data) container_insert(container, container_first(container), data)
#define container_insert_tail(container, data) container_insert(container, container_tail(container), data)

// 容器移除
#define container_remove(container, iter, rdata) (((container_t*)(container))->remove(((container_t*)(container)), iter, rdata))
#define container_remove_find(container, find, compare, rdata) container_remove(container, container_find(container, find, compare, rdata)) 
#define container_remove_first(container, rdata) container_remove(container, container_first(container), rdata)
#define container_remove_last(container, rdata) container_remove(container, container_last(container, rdata))

// 容器测试
#define container_has(container, find, compare) (!iterator_is_boundary(container_find(container, find, compare)))

// 容器排序
#define container_sort(container, compare) ((container_t*)(container))->sort((container_t*)(container), compare)

// 容器挤水
#define container_wring(container, compare, callback) (((container_t*)(container))->wring(((container_t*)(container)), compare, callback))

// 容器的内存池
#define container_mem_pool(container) (((container_t*)(container))->mem_pool)

#define container_size(container) (((container_t*)(container))->size((container_t*)container))

#define initialize_container(container, __first, __last, __search, __set, __insert, __remove, __sort, __wring, __size, __mem_pool) do { \
    ((container_t*)(container))->first  = (__first);                                        \
    ((container_t*)(container))->last   = (__last);                                         \
    ((container_t*)(container))->search = (__search);                                       \
    ((container_t*)(container))->set    = (__set);                                          \
    ((container_t*)(container))->insert = (__insert);                                       \
    ((container_t*)(container))->remove = (__remove);                                       \
    ((container_t*)(container))->sort   = (__sort);                                         \
    ((container_t*)(container))->wring  = (__wring);                                        \
    ((container_t*)(container))->size   = (__size);                                         \
    ((container_t*)(container))->mem_pool = (__mem_pool);                                   \
} while (0)

typedef struct _container container_t;

struct _container {
    iterator_t (*first) (container_t* container);   
    iterator_t (*last) (container_t * container);   
    iterator_t (*search) (container_t* container, iterator_t offset, type_value_t find, int (*compare)(type_value_t, type_value_t)); 
    int (*set) (container_t* container, type_value_t data, int(*setup)(type_value_t*, type_value_t), int (*conflict_fix) (type_value_t*, type_value_t));
    int (*insert) (container_t* container, iterator_t iter, type_value_t data); 
    int (*remove) (container_t* container, iterator_t iter, void* rdata);
    int (*sort) (container_t* container, int(*compare)(type_value_t, type_value_t));
    int (*wring) (container_t* container, int(*compare)(type_value_t, type_value_t), int(*callback)(void*));
    size_t (*size) (container_t* container);
    pool_t* mem_pool;
};

#endif