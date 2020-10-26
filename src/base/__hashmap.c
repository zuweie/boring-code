/*
 * @Author: your name
 * @Date: 2020-10-11 19:54:38
 * @LastEditTime: 2020-10-26 07:52:22
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/base/__hashmap.c
 */
#include "__hashmap.h"
#include "mem_pool/__mem_pool.h"

static hash_node_t* __create_hash_node (container_t* container, type_value_t en, int (*setup)(type_value_t*, type_value_t)) 
{
    hashmap_t* hashmap = (hashmap_t*) container;
    hash_node_t* node = allocate(container_mem_pool(container), sizeof(hash_node_t));
    node->slot_index = hashmap->key_hasher(en, hashmap->_slot_size);
    if (setup) {
        setup(&node->entity, en);
    }else{
        node->entity = en;
    }
    return node;
}
static iterator_t __get_iterator_by_key(container_t* container, type_value_t key) 
{
    hashmap_t* hashmap = (hashmap_t*) container;
    int hash_index = hashmap->key_hasher(key, hashmap->_slot_size);
    return hashmap->_slot[hash_index];
}

static iterator_t __search_in_table(container_t* container, iterator_t pos, type_value_t find) 
{
    hashmap_t* hashmap = (hashmap_t*) container;
    int hash_index = hashmap->key_hasher(find, hashmap->_slot_size);
    iterator_t table_tail = container_tail(hashmap->_hash_table);
    
    for(;!iterator_equal(pos, table_tail);pos=iterator_next(pos)) {
        
        hash_node_t* node = vtype_pointer(iterator_dereference(pos));
        if (hashmap->key_compare(node->entity, find) == 0) {
            return pos;
        }else if (node->slot_index != hash_index) {
            return table_tail;
        }
    }    
    return pos;
}

static iterator_t _hashmap_first(container_t* container)
{
    hashmap_t* hashmap = (hashmap_t*) container;
    return container_first(hashmap->_hash_table);
}

static iterator_t _hashmap_last(container_t* container)
{
    hashmap_t* hashmap = (hashmap_t*) container;
    return container_last(hashmap->_hash_table);
}

static iterator_t _hashmap_search (container_t* container, iterator_t offset, type_value_t find, int(compare)(type_value_t, type_value_t))
{
    iterator_t slot_it = __get_iterator_by_key(container, find);
    iterator_t iter    = iterator_is_tail(slot_it) ? slot_it : __search_in_table(container, slot_it, find);
    return iter;    
}

static int _hashmap_set(container_t* container, type_value_t en, int (*setup)(type_value_t*, type_value_t), int (*conflict_fix)(type_value_t, type_value_t))
{
    int ret = -1;
    hashmap_t* hashmap  = (hashmap_t*) container;

    iterator_t slot_it  = __get_iterator_by_key(container, en);
    iterator_t table_it = iterator_is_tail(slot_it)? slot_it : __search_in_table(container, slot_it, en);
    if (iterator_is_tail(slot_it) || iterator_is_tail(table_it)) {
        // 插入新元素
        hash_node_t* pnode = __create_hash_node(container, en, setup);
        ret = container_insert(hashmap->_hash_table, slot_it, pointer_vtype(pnode));
        hashmap->_slot[pnode->slot_index] = iterator_prev(slot_it);
    } else {
        // 更新元素的 value
        hash_node_t* pnode  = vtype_pointer(iterator_dereference(table_it));
        if (conflict_fix) {
            // 如果有冲突解决函数，则调用冲突解决函数
            conflict_fix(pnode->entity, en);
        } else {
            // 没有则直接把旧的 entity 换成新的 en;
            pnode->entity = en;
        }
        ret = 1;
    }
    return ret;
}

static int _hashmap_insert(container_t* container, iterator_t pos, type_value_t en) 
{
    return -1;
}
static int _hashmap_remove(container_t* container, iterator_t pos, void* rdata)
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

static int _hashmap_sort (container_t* container, int(*compare)(type_value_t, type_value_t)) 
{
    return -1;
}

static int _hashmap_wring (container_t* container, int(*compare)(type_value_t, type_value_t), int (*callback)(void*))
{
    return -1;
}

static size_t _hashmap_size(container_t* container) 
{
    hashmap_t* hashmap = (hashmap_t*) container;
    return container_size(hashmap->_hash_table);
}

container_t* hashmap_create(size_t slot_size, int (*key_hasher)(type_value_t, size_t), int (*key_compare) (type_value_t, type_value_t)) 
{
    hashmap_t* hashmap = (hashmap_t*) malloc (sizeof(hashmap_t) + sizeof(iterator_t)*slot_size);
    pool_t* _mem_pool = alloc_create(0);
    hashmap->key_hasher = key_hasher;
    hashmap->key_compare = key_compare;
    hashmap->_slot_size = slot_size;
    hashmap->_hash_table = container_create(list);

    iterator_t hash_table_tail = container_tail(hashmap->_hash_table);
    for(int i=0; i<hashmap->_slot_size; ++i) {
        hashmap->_slot[i] = hash_table_tail;
    }
    
    initialize_container(
        hashmap, 
        _hashmap_first,
        _hashmap_last, 
        _hashmap_search, 
        _hashmap_set,
        _hashmap_insert, 
        _hashmap_remove, 
        _hashmap_sort,
        _hashmap_wring, 
        _hashmap_size, 
        _mem_pool
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
