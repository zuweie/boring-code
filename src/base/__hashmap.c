/*
 * @Author: your name
 * @Date: 2020-10-11 19:54:38
 * @LastEditTime: 2020-10-15 06:03:44
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/base/__hashmap.c
 */
#include "__hashmap.h"
#include "__container.h"
#include "__iterator.h"
#include "mem_pool/__mem_pool.h"


static hash_node_t* _create_hash_node (container_t* container, type_value_t key, type_value_t value) 
{
    hashmap_t* hashmap = (hashmap_t*) container;
    hash_node_t* node = allocate(container_mem_pool(container), sizeof(hash_node_t));
    node->entity.key = key;
    node->entity.value = value;
    node->slot_index = hashmap->key_hasher(key);
    return node;
}
static iterator_t _get_iterator_by_key(container_t* container, type_value_t key) 
{
    hashmap_t* hashmap = (hashmap_t*) container;
    int hash_index = hashmap->key_hasher(key);
    return hashmap->_slot[hash_index];
}

static iterator_t _search_in_table(container_t* container, iterator_t pos, type_value_t key) 
{
    hashmap_t* hashmap = (hashmap_t*) container;
    int hash_index = hashmap->key_hasher(key);

    for(;!iterator_equal(pos, container_tail(hashmap->_hash_table));pos=iterator_next(pos)) {
        
        hash_node_t* node = type_pointer(iterator_dereference(pos));
        
        if (hashmap->key_compare(node->entity.key, key) == 0) {
            return pos;
        }else if (node->slot_index != hash_index) {
            return container_tail(hashmap->_hash_table);
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
    iterator_t slot_it = _get_iterator_by_key(container, find);
    iterator_t iter    = _search_in_table(container, slot_it, find);
    return iter;    
}

static int _hashmap_insert(container_t* container, iterator_t pos, type_value_t en)
{
    int ret = -1;
    hashmap_t* hashmap  = (hashmap_t*) container;
    entity_t* pentity   = type_pointer(en);
    iterator_t slot_it  = _get_iterator_by_key(container, pentity->key);
    iterator_t table_it = _search_in_table(container, slot_it, pentity->key);

    if (iterator_is_tail(slot_it) || iterator_is_tail(table_it)) {
        // 插入新元素
        iterator_t insert_it = iterator_is_tail(slot_it) ? slot_it : table_it;
        hash_node_t* pnode = _create_hash_node(container, pentity->key, pentity->value);
        ret = container_insert(hashmap->_hash_table, insert_it, pointer_type(pnode));
        hashmap->_slot[hashmap->key_hasher(pentity->key)] = iterator_prev(insert_it);
        return ret;
    } else {
        // 更新元素的 value
        hash_node_t* pnode  = type_pointer(iterator_dereference(table_it));
        pnode->entity.value = pentity->key;
        ret = 0;
    }
    return ret;
}

static int _hashmap_remove(container_t* container, iterator_t pos, void* rdata)
{
    if (!iterator_is_tail(pos)) {
        hashmap_t*   hashmap   = (hashmap_t*)container;
        hash_node_t* hash_node = type_pointer(iterator_dereference(pos));
        int slot_index         = hash_node->slot_index;
        iterator_t slot_it     = hashmap->_slot[slot_index];

        if (iterator_equal(slot_it, pos)) {
            // 如果找到的 pos 刚好是在 slot 中，则去除的时候需要更新
            // 获取当前 pos，查看其是否为尾部，且下一个是否同族

            iterator_t pos_next = iterator_next(pos);
            if (!iterator_is_tail(pos_next) 
            && ((hash_node_t*)(type_pointer(iterator_dereference(pos_next))))->slot_index == slot_index ) {
                // 把 slot 中的 It 置为下一个 It
                hashmap->_slot[slot_index] = pos_next;
            }else{
                // 把 slot 中的 It 置为尾部。
                hashmap->_slot[slot_index] = container_tail(hashmap->_slot);
            }
        }
        if (rdata) {
            // 把键值返回回去。
            *((entity_t*)rdata) = hash_node->entity;
        }

        container_remove(hashmap->_hash_table, pos, NULL);
        
        return 0;
    }
    return -1;
}

static int _hashmap_sort (container_t* container, int(*compare)(type_value_t, type_value_t)) 
{
    return -1;
}

static size_t _hashmap_size(container_t* container) 
{
    hashmap_t* hashmap = (hashmap_t*) container;
    return container_size(hashmap->_hash_table);
}

container_t* hashmap_create(int (*key_hasher)(type_value_t), int (*key_compare) (type_value_t, type_value_t)) 
{
    hashmap_t* hashmap = (hashmap_t*) malloc (sizeof (hashmap_t));
    pool_t* _mem_pool = alloc_create(0);
    hashmap->key_hasher = key_hasher;
    hashmap->key_compare = key_compare;
    hashmap->_hash_table = container_create(list);
    initialize_container(
        hashmap, 
        _hashmap_first,
        _hashmap_last, 
        _hashmap_search, 
        _hashmap_insert, 
        _hashmap_remove, 
        _hashmap_sort, 
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
