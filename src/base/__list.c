/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-03 15:07:45
 * @LastEditTime: 2021-10-15 16:01:19
 * @LastEditors: Please set LastEditors
 */

#include <stdlib.h>
#include "container_of.h"
#include "type_value/__type_value.h"
#include "__iterator.h"
#include "__container.h"
#include "mem_pool/__mem_pool.h"
#include "base/operate/__sort.h"
#include "base/operate/__wring.h"
#include "__list.h"

/** iter function **/
//static iterator_t _get_iter (void* refer, void* list);

static int _list_move(iterator_t* it, int step)
{
    list_node_t* pnode = container_of(it->refer, list_node_t, w);
   
    for(int next = step; next; next = step > 0? --step:++step) {
        if (next > 0) pnode = pnode->next;
        else if (next < 0) pnode = pnode->prev;
    }
    it->refer = node->w;
    return 0;
}

static iterator_t _list_first (container_t* plist)
{
    return __iterator(list_first(plist)->w, plist);
}

static iterator_t _list_last (container_t* plist)
{
    return __iterator(list_last(plist)->w, plist);
}

static iterator_t _list_search (container_t* container, iterator_t offset, type_value_t* find, int(compare)(type_value_t* t1, type_value_t* t2))
{
    for(iterator_t first = offset;
        !iterator_equal(first, container_tail(container)); 
        iterator_next(first)) {
        
            if (compare(iterator_reference(first), find) == 0) return first;
    }
    // 返回边界的指针
    return first;
}

static int _list_insert(container_t* container, iterator_t pos, type_value_t* data)
{

    list_node_t *insert = iterator_reference(pos);
    list_node_t *pnew = allocate(container->mem_pool, sizeof(list_node_t)+container->type_def.T_size);
    // 赋值
    container->type_def.T_adapter.bit_cpy(pnew->w, data);
    // 插入
    pnew->prev = insert->prev;
    pnew->next = insert;

    insert->prev->next = pnew;
    insert->prev = pnew;

    list_t *plist = container;
    plist->_size++;
    return 0;
}

static int _list_remove(container_t* container, iterator_t pos, void* rdata)
{
    // 删除
    list_node_t* remove = container_of(iterator_reference(pos), list_node_t, w);
        
    remove->prev->next = remove->next;
    remove->next->prev = remove->prev;

    // 将要删除的值返回出去。
    if (rdata) container->type_def.ty_adapter.bit_cpy(rdata, remove->w);
    // 回收
    deallocate(container_mem_pool(container), remove);
    ((list_t*)container)->_size--;
    return 0;
}

static size_t _list_size(container_t* container) 
{
    return ((list_t*)container)->_size;
}

static int _list_sort(container_t* container, int(*compare)(type_value_t, type_value_t))
{
    return quick_sort(container_first(container), container_last(container), compare);
}

static int _list_wring(container_t* container, int(*compare)(type_value_t, type_value_t), int(*callback)(void*))
{
    return wring(container, compare, callback);
}

container_t* list_create() {
    
    list_t* list = (list_t*) malloc( sizeof(list_t));
    pool_t* _mem_pool = alloc_create(0);

    initialize_container(
        list, 
        _list_first, 
        _list_last, 
        _list_search, 
        _list_set,
        _list_insert, 
        _list_remove, 
        _list_sort, 
        _list_wring, 
        _list_size, 
        _mem_pool
    );

    list_first(list) = list_head(list);
    list_last(list) = list_tail(list);
    list->_size = 0;
    list->_sentinel.data = int_vtype(-1);      
    return list;
}

int list_destroy(container_t* list) {
    alloc_destroy(container_mem_pool(list));
    free(list);
    return 0;
}
