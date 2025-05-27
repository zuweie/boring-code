/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2024-09-02 14:07:42
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-05-27 14:16:25
 * @FilePath: /boring-code/src/deep_learning/compute_garph/cg_list.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdlib.h>
#include "cg_node.h"
#include "cg_list.h"

cg_list_t* cg_list_create()
{
    cg_list_t* p_list = (cg_list_t*) malloc (sizeof(cg_list_t));
    p_list->base.prev = LIST_HEAD(p_list);
    p_list->base.ref  = (void*)NULL;
    p_list->p_top     = LIST_HEAD(p_list);
    return p_list;
}
cg_list_t* cg_list_create_cpy(cg_list_t* p_list) 
{
    cg_list_t* p_list_cpy  = cg_list_create();
    cg_node_t* p_list_fist = p_list->p_top;
    
    while(p_list_fist != LIST_HEAD(p_list)) {
        cg_list_push(p_list_cpy, p_list_fist->ref);
        p_list_fist = p_list_fist->prev;
    }
    
    cg_list_revert(p_list_cpy);
    return p_list_cpy;
}

int cg_list_revert(cg_list_t* p_list) 
{
    cg_node_t* p_revert_node = p_list->p_top;
    cg_node_t* p_revert_prev = p_list->p_top->prev;
    p_revert_node->prev      = LIST_HEAD(p_list);

    do {
        cg_node_t* prev     = p_revert_prev->prev;
        p_revert_prev->prev = p_revert_node;
        p_revert_node       = p_revert_prev;
        p_revert_prev       = prev;
    }while (p_revert_prev != LIST_HEAD(p_list));

    p_list->p_top = p_revert_node;
    
    return 0;
}

int cg_list_recycle(cg_list_t* p_list, int(*recycle)(cg_ref_t))
{
    cg_node_t* p_curr;
    while(p_list->p_top != LIST_HEAD(p_list)) {
        p_curr        = p_list->p_top;
        p_list->p_top = p_list->p_top->prev;
        if (recycle) recycle(p_curr->ref);
        free(p_curr);
    }
    free(p_list);
    return 0;
}

int cg_list_is_empty(cg_list_t* p_list)
{
    return p_list->p_top == LIST_HEAD(p_list);
}

cg_node_t* cg_list_push(cg_list_t* p_list, cg_ref_t ref)
{
    cg_node_t* p_node = (cg_node_t*) malloc (sizeof(cg_node_t));
    p_node->ref       = ref;
    p_node->prev      = p_list->p_top;
    p_list->p_top     = p_node;
    return p_node;
}

cg_node_t* cg_list_insert(cg_list* p_list, cg_node_t* insert_before, cg_ref_t ref)
{
    cg_node_t* p_node    = (cg_node_t*) malloc (sizeof(cg_node_t));
    p_node->ref          = ref;
    p_node->prev         = insert_before->prev;
    insert_before->prev  = p_node;
    return p_node;
}

cg_ref_t cg_list_pop(cg_list_t* p_list)
{
    if (!cg_list_is_empty(p_list)) {
        cg_node_t* p_pop = p_list->p_top;
        p_list->p_top    = p_list->p_top->prev;
        cg_ref_t ref     = p_pop->ref;
        free(p_pop);
        return ref;
    } 
    return NULL;
}
