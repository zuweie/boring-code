/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-08 00:02:36
 * @LastEditTime: 2025-06-03 10:51:34
 * @LastEditors: zuweie jojoe.wei@gmail.com
 */
//#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "__vector.h"
#include "__iterator.h"
#include "base/type_value/__type_value.h"
#include "mem_pool/__mem_pool.h"
#include "base/operate/__heap_sort.h"
#include "base/operate/__wring.h"
/** iterator function **/

static iterator_t __vector_move (iterator_t it, int step) 
{
    it.reference =  it.reference +  (step * T_size(it.container->type_clazz));
    //return (iterator_t){.container=it->container, .reference=it->reference};
    return it;
}
/** iterator function **/

/** container function **/
static iterator_t __vector_first (container_t* container) 
{
    vector_t* vec = (vector_t*)container;
    return __iterator(vec->_data, container);
}

static iterator_t __vector_last (container_t* container) 
{
    vector_t* vec = (vector_t*) container;
    return __iterator((vec->_data + (vec->_size -1) * T_size(container->type_clazz)), container);
}

static iterator_t __vector_search (container_t* container, iterator_t offset, type_value_t* find, int (*compare)(type_value_t, type_value_t)) 
{
    //printf("vector search here\n");
    iterator_t first = offset;
    iterator_t tail = container_tail(container);

    for(; !iterator_equal(first, tail); first=iterator_next(first)) {
        if ( (compare && compare(first.reference, find) == 0) 
        || (T_cmp(container->type_clazz)(first.reference, find)) == 0) 
        return first;
    }
    // 返回边界的指针
    return first;
}

static int __vector_insert (container_t* container, iterator_t it, type_value_t* data)
{
    vector_t *vec = container;
    // 检测一下是否满水？
    if (vec->_size >= vec->_capacity){
        // 注水
        int require_size = vec->_size + VEC_ALLOC_CHUNK_SIZE;
        type_value_t *new_block = allocate(container->mem_pool, require_size * T_size(container->type_clazz));

        if (new_block == NULL){
            return -1;
        }

        // 如果整个块要是重新malloc的，那么要重新计算it的位置。
        // 隐藏的bug：地址的差值可能会超过 long 的最大值。
        ptrdiff_t offset = it.reference - container_head(vec).reference;
        // copy 旧数据到新的内存
        memcpy(new_block, vec->_data, vec->_size * T_size(container->type_clazz));
        // 释放旧的内存
        deallocate(container->mem_pool, vec->_data);
        // 把新内存挂上去
        vec->_data = new_block;
        // 容量值变大。
        vec->_capacity += VEC_ALLOC_CHUNK_SIZE;

        // 更新it的refer。
        void *new_refer = container_head(vec).reference + offset;
        it.reference = new_refer;
    }

    // 继续做插入动作。
    iterator_t it_prev = iterator_prev(it);

    iterator_t last = container_last(container);
    iterator_t last_next = iterator_next(last);
    

    // 挪位
    for (; !iterator_equal(last, it_prev); iterator_prev(last_next), last=iterator_prev(last)){
        iterator_assign(last_next, last);
    }
    // 插入
    //container->type_def.ty_adapter.bit_cpy(it.refer, data);
    T_setup(container->type_clazz)(it.reference, data, 0);
    vec->_size++;
    return 0;
}

static int __vector_remove (container_t* container, iterator_t it, void* rdata) 
{
    vector_t *vec = container;
    if (rdata) type_value_cpy(rdata, it.reference, T_size(container->type_clazz));


    // 擦除
    iterator_t it_next = iterator_next(it);
    for (;!iterator_equal(it, container_last(vec));it=iterator_next(it),it_next=iterator_next(it_next)){
        iterator_assign(it, it_next);
    }
    vec->_size--;
        
    return 0;
}


static size_t __vector_size (container_t* container) 
{
    return ((vector_t*)container)->_size;
}
/** container **/

container_t* vector_create(T_clazz* __type_clazz) {
    vector_t* vector = (vector_t*) malloc (sizeof(vector_t));
    pool_t* __mem_pool = alloc_create(0);
    initialize_container(
        vector, 
        __vector_first, 
        __vector_last, 
        __vector_move,
        __vector_search,
        __vector_insert, 
        __vector_remove, 
        __vector_size,
        __type_clazz, 
        __mem_pool
    );
    vector->_size = 0;
    vector->_capacity = VEC_ALLOC_CHUNK_SIZE;
    // 先给水池注点水。
    vector->_data = allocate(vector->container.mem_pool, VEC_ALLOC_CHUNK_SIZE * T_size(vector->container.type_clazz));
    return vector;
}

int vector_destroy(container_t* vector) {
    alloc_destroy(vector->mem_pool);
    free(vector);
    return 0;
}