#ifndef __CG_LIST_H__
#define __CG_LIST_H__
#include "cg_node.h"

#define LIST_HEAD(p_list) &((p_list)->base)
#define LIST_TAIL(p_list) &((p_list)->base)

/// @brief 这个玩意虽然叫 list，它其实是一个盏。
typedef struct _cg_list {
    cg_node_t base;
    cg_node_t* p_top;
} cg_list_t;

cg_list_t* cg_list_create();
cg_list_t* cg_list_create_cpy(cg_list_t* p_list);
int cg_list_push(cg_list_t* p_list, cg_ref_t ref);
int cg_list_is_empty(cg_list_t* p_list);
int cg_list_revert(cg_list_t* p_list);
cg_ref_t cg_list_pop(cg_list_t* p_list);
int cg_list_recycle(cg_list_t* p_list, int(*recycle)(cg_ref_t));

#endif