/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-05-24 17:58:08
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-06-07 13:29:48
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_hash.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdlib.h>
#include <string.h>
#include "cg_debug.h"
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
            if (hash->key_cmp(first->key, key) == 0) return first;
            first = first->next;
        } while(first->hash_code == code);
    }
    return NULL;
}

cg_hash_t* cg_hash_create(int (*hash_func)(void*), int (*key_cmp)(void*, void*))
{
    cg_hash_t* hash     = (cg_hash_t*) malloc(sizeof(cg_hash_t));
    CG_HASH_FIRST(hash) = CG_HASH_HEAD(hash);
    CG_HASH_LAST(hash)  = CG_HASH_TAIL(hash);
    hash->hash          = hash_func;
    hash->key_cmp       = key_cmp;
    memset(hash->slot, (void*)0, sizeof(hash->slot));
    // 这里给capacity节点的 hash code 赋值非常重要。因为要将 list 的 tail 排除在正常节点之外。
    // 否则会出现将 tial 节点装入 slot 的情况
    hash->capacity.hash_code = SLOT_NUM + 1;
    hash->capacity.key       = NULL;
    hash->capacity.ref       = NULL;
    CG_DEBUG("cg hash(%p) created\n", hash);
    return hash;
}

int cg_hash_recycle(cg_hash_t* hash, int(*recycle)(void*))
{
    // 从尾部开始删除
    CG_DEBUG("cg hash(%p) recycle\n", hash);
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

int cg_hash_set(cg_hash_t* hash, void* key, void* val)
{
    int code                   = hash->hash(key);
    cg_hash_node_t* insert_pos = hash->slot[code];

    cg_hash_node_t* new        = (cg_hash_node_t*) malloc (sizeof(cg_hash_node_t));
    new->hash_code             = code;
    new->ref                   = val;
    new->key                   = key;
    // 如果有插入记录的就从插入记录开始插入，否则从尾部插入，插入后更新槽的位置信息。
    hash->slot[code] = insert_pos ? __list_insert(insert_pos, new) : __list_insert(CG_HASH_TAIL(hash), new);

    return 0;
}

cg_ref_t cg_hash_get(cg_hash_t* hash, void* key)
{
    cg_hash_node_t* find = __hash_find(hash, key);
    return find ? find->ref: NULL;
}

int cg_hash_has(cg_hash_t* hash, void* key)
{
    return __hash_find(hash, key) != NULL;
}

cg_ref_t cg_hash_del(cg_hash_t* hash, void* key)
{
    cg_hash_node_t* find = __hash_find(hash, key);
    if (find){
        // 如果找到了讲此节点从线性链表中去除。
        cg_ref_t ref = find->ref;
        
        int hash_code = hash->hash(key);
        // 若此节点是slot指向的首个节点，则将 slot 中元素置空。

        if (hash->slot[hash_code] == find) 
        {
            if (find->next->hash_code == hash_code)
                hash->slot[hash_code] = find->next;
            else 
                hash->slot[hash_code] = NULL;
        }

        __list_remove(find);
        free(find);
        return ref;
    } 
    return NULL;
}


