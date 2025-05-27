#ifndef __CG_HASH_H__
#define __CG_HASH_H__
#include "cg_list.h"

#define SLOT_NUM 1024

typedef struct cg_hash
{
    cg_list_t* capacity;
    cg_node_t* slot[SLOT_NUM];

    int        (*hash)(void*);
    int        (*key_cmp)(void*, void*);

} cg_hash_t;

cg_hash_t* cg_hash_create();
int        cg_hash_recycle(cg_hash_t*, int(*recycle)(void*));
int        cg_hash_set(cg_hash_t*, void*, void*);
cg_ref_t*  cg_hash_get(cg_hash_t*, void*);


#endif