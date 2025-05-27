/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-05-24 17:58:08
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-05-27 14:10:43
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_hash.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdlib.h>
#include <string.h>
#include "cg_hash.h"

static int 
__hash(void* key) 
{
    const char* str = (const char*)key;
    unsigned int hash = 0;
    while(*str) {
        hash = (hash * 31 + *str) % SLOT_NUM;
        str++;
    }
    return hash;
}

static int 
__key_cmp(void* str1, void* str2)
{
    return strcmp(str1, str2);
}

cg_hash_t* cg_hash_create()
{
    cg_hash_t* hash = (cg_hash_t*) malloc(sizeof(cg_hash_t));
    hash->capacity  = cg_list_create();
    memset(hash->slot, (void*)0, SLOT_NUM);
    hash->hash    = &__hash;
    hash->key_cmp = &__key_cmp;
    return hash
}

int cg_hash_recycle(cg_hash_t* hash, int(*recycle)(void*))
{
    cg_list_recycle(hash->capacity, recycle);
    free(hash);
    return 0;
}

int cg_hash_set(cg_hash_t* hash, void* key, void* elem)
{
    int index         = hash->hash(key);
    cg_node_t* insert = hash->slot[index];
    cg_node_t* iter   = insert ? cg_list_insert(hash->capacity, insert, elem) : cg_list_push(hash->capacity, elem);
    hash->slot[index] = iter;
    return 0;
}

cg_ref_t* cg_hash_get(cg_hash_t* hash, void* key)
{
    int index         = hash->hash(key);
    cg_node_t* first  = hash->slot[index];

    if (first) {

        do{
            if (hash->key_cmp(first->ref, key) == 0) {
                return first->ref;
            } 
            first = first->prev;

        }while(hash->hash(first->ref) == hash->hash(key));
    }
    return NULL;
}