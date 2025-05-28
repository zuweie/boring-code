/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-05-24 17:58:08
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-05-28 12:20:42
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_hash.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdlib.h>
#include <string.h>
#include "cg_hash.h"

static cg_hash_node_t* __list_insert(cg_hash_node_t* pos, cg_hash_node_t* new) 
{
    // 前插，将 insert 插在 pos 前面
    new->next = pos;
    new->prev = pos->prev;

    pos->prev->next = new;
    pos->prev = new;

    return new;
}

static cg_hash_node_t* __list_remove(cg_hash_node_t* del)
{    
    cg_hash_node_t* del_prev = del->prev;
    
    del->prev->next = del->next;
    del->next->prev = del->prev;

    return del_prev;
}

static cg_hash_node_t* __hash_find(cg_hash_t* hash, void* key)
{
    int             code  = hash->hash(key);
    cg_hash_node_t* first = hash->slot[code];
    if (first) {
        do {
            if (hash->key_cmp(first->ref, key) == 0) return first;
            first = frist->next;
        } while(first->hash_code == code);
    }
    return NULL;
}

cg_hash_t* cg_hash_create(int (*hash)(void*), int (*key_cmp)(void*, void*))
{
    cg_hash_t* hash     = (cg_hash_t*) malloc(sizeof(cg_hash_t));
    CG_HASH_FIRST(hash) = CG_HASH_HEAD(hash)
    CG_HASH_LAST(hash)  = CG_HASH_TAIL(hash)
    hash->hash          = hash;
    hash->key_cmp       = key_cmp;
    memset(hash->slot, (void*)0, SLOT_NUM);

    return hash
}

int cg_hash_recycle(cg_hash_t* hash, int(*recycle)(void*))
{
    // 从尾部开始删除
    cg_hash_node_t* del  = CG_HASH_LAST(hash);
    cg_hash_node_t* prev;
    while (del != CG_HASH_HEAD(hash)) {
        // 从链表中将节点移除
        prev = __list_remove(del);
        // 看看是否有回收
        if (recycle) recycle(del->ref);
        // 将该节点内存释放
        free(del);
        // 跟新 last 指针。
        del = prev;
    }
    free(hash);
    return 0;
}

int cg_hash_set(cg_hash_t* hash, void* key, void* value)
{
    int code                   = hash->hash(key);
    cg_hash_node_t* insert_pos = hash->slot[code];

    cg_hash_node_t* new        = (cg_hash_node_t*) malloc (sizeof(cg_hash_node_t));
    new->hash_code             = code;
    new->ref                   = value;

    // 如果有插入记录的就从插入记录开始插入，否则从尾部插入，插入后更新槽的位置信息。
    hash->slot[code] = insert_pos ? __list_insert(insert_pos, new) : __list_insert(CG_HASH_TAIL(hash), new);

    return 0;
}

cg_ref_t* cg_hash_get(cg_hash_t* hash, void* key)
{
    cg_hash_node_t* find = __hash_find(hash, key);
    return find ? find->ref: NULL;
}

int cg_hash_has(cg_hash_t* hash, void* key)
{
    return __hash_find(hash, key) != NULL;
}

cg_ref_t* cg_hash_del(cg_hash_t* hash, void* key)
{
    cg_hash_node_t* find = __hash_find(hash, key);
    if (find){
        cg_ref_t* ref = find->ref;
        __list_del(find);
        return ref;
    } 
    return NULL;
}


