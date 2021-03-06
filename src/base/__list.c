/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-03 15:07:45
 * @LastEditTime: 2020-11-09 14:41:27
 * @LastEditors: Please set LastEditors
 */

#include <stdlib.h>
#include "__list.h"
#include "__type_value.h"
#include "__iterator.h"
#include "__container.h"
#include "mem_pool/__mem_pool.h"
#include "base/operate/__sort.h"
#include "base/operate/__wring.h"
/** iter function **/
//static iterator_t _get_iter (void* refer, void* list);

static iterator_t _move(iterator_t it, int step)
{
    list_node_t* pnode = iterator_reference(it);
   
    for(int next = step; next; next = step > 0? --step:++step) {
        if (next > 0) pnode = pnode->next;
        else if (next < 0) pnode = pnode->prev;
    }
    return iterator_set_reference(it, pnode);
}


// static iterator_t _get_iter(void *refer, void* list) 
// {
//     return __iterator(refer, list, _move);
// }
/** iter function **/

/** container function **/

static iterator_t _list_first (container_t* plist)
{
    return __iterator(list_first(plist), plist, _move);
}

static iterator_t _list_last (container_t* plist)
{
    return __iterator(list_last(plist), plist, _move);
}

static iterator_t _list_search (container_t* container, iterator_t offset, type_value_t find, int(compare)(type_value_t data1, type_value_t data2))
{
    iterator_t first = offset;
    iterator_t tail  = container_tail(container);
    for(;!iterator_equal(first, tail); first = iterator_next(first)) {
        if (compare(iterator_dereference(first), find) == 0) {
            return first;
        }
    }
    // 返回边界的指针
    return first;
}
static int _list_set(container_t* container, type_value_t data, int(*setup)(type_value_t*, type_value_t), int(*confilct_fix)(type_value_t*, type_value_t))
{
    return -1;
}

static int _list_insert(container_t* container, iterator_t pos, type_value_t data)
{

    list_node_t *pnode = iterator_reference(pos);
    list_node_t *pnew = allocate(container_mem_pool(container), sizeof(list_node_t));
    // 赋值 和 插入

    pnew->data = data;
    pnew->prev = pnode->prev;
    pnew->next = pnode;

    pnode->prev->next = pnew;
    pnode->prev = pnew;

    list_t *plist = container;
    plist->_size++;
    return 0;
}

static int _list_remove(container_t* container, iterator_t pos, void* rdata)
{
    // 删除
    // 边界的东西不能移除
    // if (!iterator_is_boundary(pos)){

    //     list_t* list = container;
    //     list_node_t* pnode = iterator_reference(pos);
        
    //     pnode->prev->next = pnode->next;
    //     pnode->next->prev = pnode->prev;

    //     // 将要删除的值返回出去。
    //     if (rdata) *((type_value_t*)rdata) = iterator_dereference(pos);
    //     // 回收
    //     deallocate(container_mem_pool(container), pnode);
    //     list->_size--;
    //     return 0; 
    // }

    list_t* list = container;
    list_node_t* pnode = iterator_reference(pos);
        
    pnode->prev->next = pnode->next;
    pnode->next->prev = pnode->prev;

    // 将要删除的值返回出去。
    if (rdata) *((type_value_t*)rdata) = iterator_dereference(pos);
    // 回收
    deallocate(container_mem_pool(container), pnode);
    list->_size--;
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
