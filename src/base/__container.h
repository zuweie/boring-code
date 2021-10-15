/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-07 23:21:46
 * @LastEditTime: 2021-10-15 15:04:23
 * @LastEditors: Please set LastEditors
 */
#ifndef _CONTAINER_H_
#define _CONTAINER_H_

#include <stddef.h>

//#include "__type_value.h"
#include "type_value/__type_value_def.h"
#include "__iterator.h"
#include "mem_pool/__mem_pool.h"
typedef struct _iterator iterator_t;

#define container_create(container_label, ...) container_label##_create(__VA_ARGS__)
#define container_destroy(conatainer_label, ...) conatainer_label##_destroy(__VA_ARGS__)

// 容器位置
#define container_first(container_ptr) (((container_t*)(container_ptr))->first((container_t*)(container_ptr)))
#define container_last(container_ptr) (((container_t*)(container_ptr))->last((container_t*)(container_ptr)))
#define container_head(container_ptr) ({iterator_t first=container_first(container_ptr); iterator_move(&first, -1); first;})
#define container_tail(container_ptr) ({iterator_t last=container_last(container_ptr); iterator_move(&last, 1); last})

// 容器搜索
#define container_search(container_ptr, offset, find, compare) \
    (((container_t*)(container_ptr))->search(((container_t*)(container_ptr)), offset, find, compare))

// 容器插入
#define container_insert(container_ptr, iter, data) \
    (((container_t*)(container_ptr))->insert(((container_t*)(container_ptr)), iter, data))

// 容器移除
#define container_remove(container_ptr, iter, rdata) \
    (((container_t*)(container_ptr))->remove(((container_t*)(container_ptr)), iter, rdata))

// 容器排序
#define container_sort(container_ptr, compare) \
    ((container_t*)(container_ptr))->sort((container_t*)(container_ptr), compare)

// 容器挤水
#define container_wring(container_ptr, compare, clean) \
    (((container_t*)(container_ptr))->wring(((container_t*)(container_ptr)), compare, clean))

// 容器大小
#define container_size(container_ptr) \
    (((container_t*)(container_ptr))->size((container_t*)container_ptr))

#define initialize_container(container_ptr, __first, __last, __search, __move, __insert, __remove, __sort, __wring, __size, __type_def, __mem_pool) \
({ \
    ((container_t*)(container_ptr))->first  = (__first);                                        \
    ((container_t*)(container_ptr))->last   = (__last);                                         \
    ((container_t*)(container_ptr))->search = (__search);                                       \
    ((container_t*)(container_ptr))->set    = (__set);                                          \
    ((container_t*)(container_ptr))->insert = (__insert);                                       \
    ((container_t*)(container_ptr))->remove = (__remove);                                       \
    ((container_t*)(container_ptr))->sort   = (__sort);                                         \
    ((container_t*)(container_ptr))->wring  = (__wring);                                        \
    ((container_t*)(container_ptr))->size   = (__size);                                         \
    ((container_t*)(container_ptr))->type_def = (__type_def)                                    \
    ((container_t*)(container_ptr))->mem_pool = (__mem_pool);                                   \
})

typedef struct _container container_t;

struct _container {
    iterator_t (*first) (container_t* container_ptr);   
    iterator_t (*last) (container_t * container_ptr); 
    iterator_t (*search) (container_t* container_ptr, iterator_t offset, type_value_t* find, int (*compare)(type_value_t*, type_value_t*));
    int (*move) (iterator_t* iter, int step);
    int (*insert) (container_t* container_ptr, iterator_t iter, type_value_t* data); 
    int (*remove) (container_t* container_ptr, iterator_t iter, void* rdata);
    int (*sort) (container_t* container_ptr, int(*compare)(type_value_t*, type_value_t*));
    int (*wring) (container_t* container_ptr, int(*compare)(type_value_t*, type_value_t*), int(*clean)(void*));
    int (*size) (container_t* container_ptr);
    T_def type_def;
    pool_t* mem_pool;
};

#endif