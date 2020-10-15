/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-08 00:02:36
 * @LastEditTime: 2020-10-15 22:07:03
 * @LastEditors: Please set LastEditors
 */
//#include <stdio.h>
#include <stddef.h>
#include <string.h>
#include "__vector.h"
#include "__iterator.h"
#include "__type_value.h"
#include "mem_pool/__mem_pool.h"
#include "__sort.h"

/** iterator function **/
static iterator_t _get_iter (void* refer, void* container);

static iterator_t _move (iterator_t it, int step) 
{
    type_value_t* pv = iterator_reference(it);

    //return _get_iter((pv + step), iterator_container(it));
    return iterator_set_reference(it, (pv+step));
}

static iterator_t _get_iter (void* refer, void* vec) {
    return get_iterator(refer, vec, _move);
}
/** iterator function **/

/** container function **/
static iterator_t _vector_first (container_t* container) 
{
    vector_t* vec = container;
    return _get_iter(vec->_data, vec);
}

static iterator_t _vector_last (container_t* container) 
{
    vector_t* vec = container;
    return _get_iter((vec->_data + vec->_size -1), vec);
}

static iterator_t _vector_search (container_t* container, iterator_t offset, type_value_t find, int (*compare)(type_value_t, type_value_t)) 
{
    //printf("vector search here\n");
    iterator_t first = offset;
    iterator_t tail = container_tail(container);

    for(; !iterator_equal(first, tail); first=iterator_next(first)) {
        if (compare(iterator_dereference(first), find) == 0) return first;
    }
    // 返回边界的指针
    return first;
}

static int _vector_insert (container_t* container, iterator_t it, type_value_t data)
{
    // head 的位置不能前插
    if (!iterator_is_head(it)){
        
        vector_t* vec = container;
        // 检测一下是否满水？
        if (vec->_size >= vec->_capacity){
            // 注水
            unsigned int require_size = vec->_size + VEC_ALLOC_CHUNK_SIZE;
            type_value_t *new_block = allocate(container_mem_pool(container), require_size * sizeof(type_value_t));

            if (new_block == NULL){
                return -1;
            }

            // 如果整个块要是重新malloc的，那么要重新计算it的位置。
            // 隐藏的bug：地址的差值可能会超过 long 的最大值。
            ptrdiff_t offset = iterator_reference(it) - iterator_reference(container_head(vec));
            // copy 旧数据到新的内存
            memcpy(new_block, vec->_data, vec->_size * sizeof(type_value_t));
            // 释放旧的内存
            deallocate(container_mem_pool(container), vec->_data);
            // 把新内存挂上去
            vec->_data = new_block;
            // 容量值变大。
            vec->_capacity += VEC_ALLOC_CHUNK_SIZE;

            // 更新it的refer。
            void *new_refer = iterator_reference(container_head(vec)) + offset;
            iterator_set_reference(it, new_refer);
        }

        // 继续做插入动作。
        iterator_t last = container_last(container);
        iterator_t it_prev = iterator_prev(it);

        // 挪位
        for (; !iterator_equal(last, it_prev); last = iterator_prev(last)){
            iterator_t last_next = iterator_next(last);
            iterator_assign(last_next, last);
        }
        // 插入
        type_value_t *pt = iterator_reference(it);
        *pt = data;
        vec->_size++;
        return 0;
    }
    return -1;
}

static int _vector_remove (container_t* container, iterator_t it, void* rdata) 
{
    if (!iterator_is_boundary(it)){
        
        vector_t *vec = container;

        if (rdata){

            *((type_value_t*)rdata) = iterator_dereference(it);
        }

        // 擦除
        for (;!iterator_equal(it, container_last(vec));it = iterator_next(it)){
            iterator_t it_next = iterator_next(it);
            iterator_assign(it, it_next);
        }
        vec->_size--;
        return 0;
    }
    return -1;
}

static int _vector_sort(container_t* container, int(*compare)(type_value_t, type_value_t)) 
{
    return quick_sort(container_first(container), container_last(container), compare);
}

static size_t _vector_size (container_t* container) 
{
    return ((vector_t*)container)->_size;
}
/** container **/

container_t* vector_create() {
    vector_t* vector = (vector_t*) malloc (sizeof(vector_t));
    pool_t* _mem_pool = alloc_create(0);
    initialize_container(vector, _vector_first, _vector_last, _vector_search, _vector_insert, _vector_remove,_vector_sort, _vector_size, _mem_pool);
    vector->_size = 0;
    vector->_capacity = VEC_ALLOC_CHUNK_SIZE;
    // 先给水池注点水。
    vector->_data = allocate(container_mem_pool(vector), VEC_ALLOC_CHUNK_SIZE*sizeof(type_value_t));
    return vector;
}

int vector_destroy(container_t* vector) {
    alloc_destroy(container_mem_pool(vector));
    free(vector);
    return 0;
}