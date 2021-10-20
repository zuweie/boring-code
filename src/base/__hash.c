/*
 * @Author: your name
 * @Date: 2020-10-11 19:54:38
 * @LastEditTime: 2021-10-20 17:08:18
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/base/__hashmap.c
 */
#include "__hash.h"
#include "type_value/__built_in_type.h"
#include "container_of.h"
#include "mem_pool/__mem_pool.h"

static hash_node_t* __create_hash_node (container_t* container, type_value_t* en) 
{
    hash_t* hash = (hashmap_t*) container;
    hash_node_t* node = allocate(container->mem_pool, sizeof(hash_node_t) + container->type_def.ty_size);
    node->slot_index = container->type_def.ty_hasher(en, hash->_slot_size); 
    
    if (hash->setup) {
        hash->setup(node->w, en);
    }else{
        container->type_def.ty_adapter.bit_cpy(node->w, en);
    }
    return node;
}
static hash_inner_list_node_t* __get_inner_list_node_by_key(hash_t* hash, type_value_t* key) 
{
    int hash_index = container->type_def.ty_hasher(key, hash->_slot_size); 
    return hash->_slot[hash_index];
}

static hash_inner_list_node_t* __search_in_inner_list(hash_t* hash, hash_inner_list_node_t* from, type_value_t* find) 
{
    int hash_index = container->type_def.ty_hasher(find, hash->_slot_size); 
    hash_inner_list_node_t* tail  = hash_table_tail(hash);
    
    for(;from != tail;from = from->next) {        
        if (hash->container.type_def.ty_cmp(from->w, find)== 0) {
            return from;
        }else if (first->slot_index != hash_index) {
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
    hash_inner_list_node_t* node = container_of(iter->reference, hash_inner_list_node_t, w)
    for (int next = step ; next; next = step > 0? next - 1: next + 1) {
        if (step > 0) node = node->next;
        else if (step < 0) node = node->prev;
    }
    iter->reference = node->w;
}
static iterator_t __hash_search (container_t* container, iterator_t offset, type_value_t* find, int(compare)(type_value_t, type_value_t))
{
    hash_t* hash = (hash_t*)container;
    hash_inner_list_node_t* slot_from   = __get_inner_list_node_by_key(container, find);
    hash_inner_list_node_t* target = (solt_from == hash_table_tail(hash)) ? slot_from : __search_in_inner_list(container, slot_from, find);
    return __iterator(target->w, container);    
}

// 这个是超级有难度的。
static int __hash_insert(container_t* container, iterator_t pos, type_value_t* en)
{
    int ret = -1;
    hash_t* hash  = (hash_t*) container;

    hash_inner_list_node_t* slot_from  = __get_inner_list_node_by_key(hash, en);
    hash_inner_list_node_t* target     = (slot_from == hash_table_tail(hash)) ? slot_from : __search_in_inner_list(container, slot_from, en);
    if (target == hash_table_tail(hash)) {
        // 插入新元素
        //hash_node_t* pnode = __create_hash_node(container, en, setup);
        int new_node_size = sizeof(hash_node_t) + hash->container.type_def.ty_size;
        type_value_t new_node[new_node_size];
        
        ret = container_insert(hashmap->_hash_table, slot_it, pnew);
        hashmap->_slot[pnode->slot_index] = iterator_prev(slot_it);
    } else {
        // 找到元素，跟新它
        // 更新元素的 value
        //hash_node_t* pnode  = vtype_pointer(iterator_dereference(table_it));
        hash_node_t* pnode = table_it.reference;
        if (hash->conflict_fix) {
            // 如果有冲突解决函数，则调用冲突解决函数
            hash->conflict_fix(pnode->w, en);
        } else {
            // 没有则直接把旧的 entity 换成新的 en;
            hash->container.type_def.ty_adapter.bit_cpy(pnode->w, en);
        }
        ret = 1;
    }
    return ret;
}

// static int __hash_insert(container_t* container, iterator_t pos, type_value_t en) 
// {
//     return -1;
// }
static int __hash_remove(container_t* container, iterator_t pos, void* rdata)
{
    if (!iterator_is_tail(pos)) {
        hashmap_t*   hashmap   = (hashmap_t*)container;
        hash_node_t* hash_node = vtype_pointer(iterator_dereference(pos));
        int slot_index         = hash_node->slot_index;
        iterator_t slot_it     = hashmap->_slot[slot_index];

        if (iterator_equal(slot_it, pos)) {
            // 如果找到的 pos 刚好是在 slot 中，则去除的时候需要更新
            // 获取当前 pos，查看其是否为尾部，且下一个是否同族
            iterator_t pos_next = iterator_next(pos);
            if (!iterator_is_tail(pos_next) 
            && ((hash_node_t*)(vtype_pointer(iterator_dereference(pos_next))))->slot_index == slot_index ) {
                // 把 slot 中的 It 置为下一个 It
                hashmap->_slot[slot_index] = pos_next;
            }else{
                // 把 slot 中的 It 置为尾部。
                hashmap->_slot[slot_index] = container_tail(hashmap->_hash_table);
            }
        }
        if (rdata) *((type_value_t*)rdata) = hash_node->entity;

        // 把这个节点干掉。
        type_value_t del;
        container_remove(hashmap->_hash_table, pos, &del);
        deallocate(container_mem_pool(hashmap), vtype_pointer(del));
        return 0;
    }
    return -1;
}



static size_t _hash_size(container_t* container) 
{
    hashmap_t* hash = (hash_t*) container;
    return container_size(hash->_hash_table);
}

container_t* hash_create(T_def* __ty_def, int slot_size, int (*setup)(type_value_t*, type_value_t*), int (*conflict_fix)(type_value_t*, type_value_t*)) 
{
    hash_t* hash = (hash_t*) malloc (sizeof(hash_t) + sizeof(hash_inner_list_node_t*)*slot_size);
    pool_t* __mem_pool = alloc_create(0);
    hash->_slot_size = slot_size;

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
        *__ty_def, 
        __mem_pool
    );
    return hashmap;
}

int hashmap_destroy(container_t* container)
{
    hashmap_t* hashmap = (hashmap_t*)container;
    container_destroy(list, hashmap->_hash_table);
    alloc_destroy(container_mem_pool(hashmap));
    free(hashmap);
    return 0;
}
