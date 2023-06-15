/*
 * @Author: your name
 * @Date: 2020-10-11 19:54:38
<<<<<<< HEAD
 * @LastEditTime: 2021-11-10 16:59:57
 * @LastEditors: Please set LastEditors
=======
 * @LastEditTime: 2023-06-14 12:09:03
 * @LastEditors: zuweie jojoe.wei@gmail.com
>>>>>>> origin/main
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/base/__hashmap.c
 */
#include "__hash.h"
#include "type_value/__built_in_type_adapters.h"
#include "container_of.h"
#include "mem_pool/__mem_pool.h"

static hash_inner_list_node_t* __get_slot_node_by_key(hash_t* hash, type_value_t* key) 
{
    int hash_index = T_hash(hash->container.type_clazz)(key, hash->_slot_size);
    return hash->_slot[hash_index];
}

static hash_inner_list_node_t* __search_in_inner_list(hash_t* hash, hash_inner_list_node_t* from, type_value_t* find) 
{
    int hash_index = T_hash(hash->container.type_clazz)(find, hash->_slot_size);
    hash_inner_list_node_t* tail  = hash_table_tail(hash);
    
    for(;from != tail;from = from->next) {        
        if (T_cmp(hash->container.type_clazz)(from->w, find) == 0) {
            return from;
        }else if (from->slot_index != hash_index) {
            return tail;
        }
    }    
    return from;
}

static iterator_t __hash_first(container_t* container)
{
    hash_t* hash = (hash_t*) container;
    return __iterator(hash_table_head(hash)->next->w, container);
}

static iterator_t __hash_last(container_t* container)
{
    hash_t* hash = (hash_t*) container;
    return __iterator(hash_table_tail(hash)->prev->w, container);
}
static int __hash_move(iterator_t* iter, int step) 
{
    hash_t* hash = iter->container;
    hash_inner_list_node_t* node = container_of(iter->reference, hash_inner_list_node_t, w);
    for (int next = step ; next; step > 0 ? next-- : next++) {
        if (step > 0) node = node->next;
        else if (step < 0) node = node->prev;
    }
    iter->reference = node->w;
}

static iterator_t __hash_search (container_t* container, iterator_t offset, type_value_t* find, int(compare)(type_value_t, type_value_t))
{
    hash_t* hash = (hash_t*)container;
    hash_inner_list_node_t* target; 
    
    if (iterator_is_null(offset)) {
        hash_inner_list_node_t* slot_from = __get_slot_node_by_key(container, find);
        target = (slot_from == hash_table_tail(hash)) ? slot_from : __search_in_inner_list(container, slot_from, find);
    } else {
        hash_inner_list_node_t* from = container_of(offset.reference, hash_inner_list_node_t, w);
        target =  __search_in_inner_list(container, from, find);
    }
    return __iterator(target->w, container);    
}

// 这个是超级有难度的。
static int __hash_insert(container_t* container, iterator_t pos, type_value_t* en)
{
    int ret = -1;
    hash_t* hash  = (hash_t*) container;

    hash_inner_list_node_t* slot_from  = __get_slot_node_by_key(hash, en);
    hash_inner_list_node_t* target     = (slot_from == hash_table_tail(hash)) ? slot_from : __search_in_inner_list(container, slot_from, en);

    if (slot_from == hash_table_tail(hash) || target == hash_table_tail(hash) || hash->_multi) {
        // 插入新元素
        hash_inner_list_node_t* insert = hash->_multi ? (target == hash_table_tail(hash) ? slot_from : target->next): slot_from;
<<<<<<< HEAD
        // 申请内存。
        hash_inner_list_node_t* inner_list_node = allocate(container->mem_pool, sizeof(hash_inner_list_node_t) + T_size(container->type_clazz));
        // 初始化。
        inner_list_node->slot_index = T_hash(container->type_clazz)(en, hash->_slot_size);
        
=======
        // 申请内存. 
        hash_inner_list_node_t* inner_list_node = allocate(container->mem_pool, sizeof(hash_inner_list_node_t) + T_size(container->type_clazz));
        // 初始化。
        inner_list_node->slot_index = T_hash(container->type_clazz)(en, hash->_slot_size);
        // 这里执行了，entity body 的内存申请。
>>>>>>> origin/main
        T_setup(container->type_clazz)(inner_list_node->w, en, 0);

        inner_list_node->prev = insert->prev;
        inner_list_node->next = insert;

        insert->prev->next = inner_list_node;
        insert->prev       = inner_list_node;

        // 这里有一种情况不用更新 slot index 的。
        if ( slot_from == hash_table_tail(hash) || target == hash_table_tail(hash)) {
            hash->_slot[inner_list_node->slot_index] = inner_list_node;
        }
        hash->_size ++;
    } else {
 
        T_setup(container->type_clazz)(target->w, en, 1);

        ret = 1;
    }
    return ret;
}


static int __hash_remove(container_t* container, iterator_t pos, void* rdata)
{
    
    hash_t* hash = (hash_t*)container;
    hash_inner_list_node_t* remove = container_of(pos.reference, hash_inner_list_node_t, w);
    hash_inner_list_node_t* slot_from = hash->_slot[remove->slot_index];
    
    if (slot_from == remove) {
        if (remove->next->slot_index == remove->slot_index) {
            hash->_slot[remove->slot_index] = remove->next;
        } else {
            hash->_slot[remove->slot_index] = hash_table_tail(hash);
        }
    }

    remove->prev->next = remove->next;
    remove->next->prev = remove->prev;

    if (rdata) type_value_cpy(rdata, remove->w, T_size(container->type_clazz));

    deallocate(container->mem_pool, remove);
    hash->_size --;
    return 0;
}



static size_t __hash_size(container_t* container) 
{
    return ((hash_t*)container)->_size;
}

container_t* hash_create(T_clazz* __type_clazz, int slot_size, unsigned char multi) 
{
    hash_t* hash = (hash_t*) malloc (sizeof(hash_t) + sizeof(hash_inner_list_node_t*)*slot_size);
    pool_t* __mem_pool = alloc_create(0);
    hash->_slot_size = slot_size;
    hash->_multi = multi;
    hash->_size = 0;
    // init the inner double direct linklist.
    hash->_hash_table._sentinel.prev = &(hash->_hash_table._sentinel);
    hash->_hash_table._sentinel.next = &(hash->_hash_table._sentinel);

    for(int i=0; i<hash->_slot_size; ++i) {
        hash->_slot[i] = hash_table_tail(hash);
    }
    
    initialize_container(
        hash, 
        __hash_first,
        __hash_last,
        __hash_move, 
        __hash_search, 
        __hash_insert, 
        __hash_remove, 
        __hash_size,
        __type_clazz, 
        __mem_pool
    );
    return hash;
}

int hash_destroy(container_t* container)
{
    hash_t* hash = (hash_t*)container;
    //container_destroy(list, hashmap->_hash_table);
    // TODO : 内部的 list 销毁。我操他妈的个 B。
    alloc_destroy(container->mem_pool);
    free(hash);
    return 0;
}
