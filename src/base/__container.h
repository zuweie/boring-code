/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-07 23:21:46
 * @LastEditTime: 2025-06-02 13:46:32
 * @LastEditors: zuweie jojoe.wei@gmail.com
 */
#ifndef __CONTAINER_H__
#define __CONTAINER_H__

#include <stddef.h>

//#include "__type_value.h"
#include "type_value/__type_value_def.h"
#include "__iterator.h"
#include "mem_pool/__mem_pool.h"

typedef struct iterator iterator_t;
typedef struct container container_t;

#define container_create(container_label, ...) container_label##_create(__VA_ARGS__)
#define container_destroy(conatainer_label, ...) conatainer_label##_destroy(__VA_ARGS__)

// 容器的 first
#define container_first(container_ptr) (((container_t*)(container_ptr))->first((container_t*)(container_ptr)))
// 容器的 last
#define container_last(container_ptr) (((container_t*)(container_ptr))->last((container_t*)(container_ptr)))
// 容器的 head
//#define container_head(container_ptr) ({iterator_t first=container_first(container_ptr); iterator_move(&first, -1); first;})
#define container_head(container_ptr) ((container_t*)(container_ptr))->move(container_first(container_ptr), -1)
// 容器的 tail
// #define container_tail(container_ptr) ({iterator_t last=container_last(container_ptr); iterator_move(&last, 1); last;})
#define container_tail(container_ptr) ((container_t*)(container_ptr))->move(container_last(container_ptr), 1)
// 容器的
#define container_access(container_ptr, step) ((container_t*)(container_ptr))->move(container_first(container_ptr), step)//({iterator_t first=container_first(container_ptr); iterator_move(&first, step); first;})

// 容器搜索
#define container_search(container_ptr, offset, find, compare) \
    (((container_t*)(container_ptr))->search(((container_t*)(container_ptr)), offset, find, compare))

// 容器插入
#define container_insert(container_ptr, iter, data) \
    (((container_t*)(container_ptr))->insert(((container_t*)(container_ptr)), iter, data))

// 容器移除
#define container_remove(container_ptr, iter, rdata) \
    (((container_t*)(container_ptr))->remove(((container_t*)(container_ptr)), iter, rdata))

// // 容器排序
// #define container_sort(container_ptr, compare) \
//     ((container_t*)(container_ptr))->sort((container_t*)(container_ptr), compare)

// // 容器挤水
// #define container_wring(container_ptr, compare, clean) \
//     (((container_t*)(container_ptr))->wring(((container_t*)(container_ptr)), compare, clean))

// 容器大小
#define container_size(container_ptr) \
    (((container_t*)(container_ptr))->size((container_t*)container_ptr))

#define initialize_container(container_ptr, __first, __last, __move, __search, __insert, __remove, __size, __type_clazz, __mem_pool) \
do { \
    ((container_t*)(container_ptr))->first  = (__first);                                        \
    ((container_t*)(container_ptr))->last   = (__last);                                         \
    ((container_t*)(container_ptr))->move   = (__move);                                         \
    ((container_t*)(container_ptr))->search = (__search);                                       \
    ((container_t*)(container_ptr))->insert = (__insert);                                       \
    ((container_t*)(container_ptr))->remove = (__remove);                                       \
    ((container_t*)(container_ptr))->size   = (__size);                                         \
    ((container_t*)(container_ptr))->type_clazz = (__type_clazz);                               \
    ((container_t*)(container_ptr))->mem_pool = (__mem_pool);                                   \
}while (0)




struct container {
    iterator_t (*first) (container_t* container_ptr);   
    iterator_t (*last) (container_t * container_ptr); 
    iterator_t (*move) (iterator_t iter, int step);
    iterator_t (*search) (container_t* container_ptr, iterator_t offset, type_value_t* find, int (*compare)(type_value_t*, type_value_t*));
    int (*insert) (container_t* container_ptr, iterator_t iter, type_value_t* data); 
    int (*remove) (container_t* container_ptr, iterator_t iter, void* rdata);
    int (*size) (container_t* container_ptr);
    T_clazz *type_clazz;
    pool_t* mem_pool;
};

#endif