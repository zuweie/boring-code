/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-05-24 17:58:14
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-06-07 13:14:04
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_hash.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __CG_HASH_H__
#define __CG_HASH_H__
#include "cg_ref.h"

#define SLOT_NUM 1024

#define CG_HASH_HEAD(hash) (&((hash)->capacity))
#define CG_HASH_TAIL(hash) (&((hash)->capacity))
#define CG_HASH_FIRST(hash) (CG_HASH_HEAD(hash)->next)
#define CG_HASH_LAST(hash)  (CG_HASH_TAIL(hash)->prev)

typedef struct cg_hash_node
{
    unsigned int hash_code;
    cg_ref_t ref;
    void*    key;
    struct cg_hash_node* prev;
    struct cg_hash_node* next;

} cg_hash_node_t;

typedef struct cg_hash
{
    cg_hash_node_t  capacity;
    cg_hash_node_t* slot[SLOT_NUM];
    
    int        (*hash)(void*);
    int        (*key_cmp)(void*, void*);

} cg_hash_t;

cg_hash_t* cg_hash_create(int (*hash)(void*), int (*key_cmp)(void*, void*));
int        cg_hash_recycle(cg_hash_t*, int(*recycle)(void*));
int        cg_hash_set(cg_hash_t*, void*, void*);
cg_ref_t   cg_hash_get(cg_hash_t*, void*);
cg_ref_t   cg_hash_del(cg_hash_t*, void*);
int        cg_hash_has(cg_hash_t*, void*);


#endif