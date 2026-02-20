/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2024-09-02 14:07:42
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-02-19 22:22:10
 * @FilePath: /boring-code/src/deep_learning/compute_garph/cg_list.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdlib.h>
#include "cg_debug.h"
#include "cg_list.h"

cg_list_t* cg_list_create()
{
    cg_list_t* p_list = (cg_list_t*) malloc (sizeof(cg_list_t));
    CG_LIST_TOP(p_list) = CG_LIST_HEAD(p_list);
    CG_DEBUG("cg list(%p) created.\n", p_list);
    p_list->size = 0;
    return p_list;
}

cg_list_t* cg_list_cpy(cg_list_t* dest, cg_list_t* src)
{
    cg_list_node_t* p_src_fist = CG_LIST_TOP(src);
    
    while(p_src_fist != CG_LIST_HEAD(src)) {
        cg_list_push(dest, p_src_fist->ref);
        p_src_fist = p_src_fist->prev;
    }
    
    cg_list_revert(dest);
    return dest;
}

cg_list_t* cg_list_create_cpy(cg_list_t* p_list) 
{
    cg_list_t* p_list_cpy  = cg_list_create();
    return cg_list_cpy(p_list_cpy, p_list);
}


int cg_list_revert(cg_list_t* p_list) 
{
    // plist 不为空，并且有一个以上的元素，才会反转。
    if (!cg_list_is_empty(p_list) || CG_LIST_TOP(p_list)->prev != CG_LIST_HEAD(p_list)) {

        cg_list_node_t *p_revert_node = CG_LIST_TOP(p_list);
        cg_list_node_t *p_revert_prev = CG_LIST_TOP(p_list)->prev;
        p_revert_node->prev = CG_LIST_HEAD(p_list);

        do
        {
            cg_list_node_t *prev = p_revert_prev->prev;
            p_revert_prev->prev = p_revert_node;
            p_revert_node = p_revert_prev;
            p_revert_prev = prev;
        } while (p_revert_prev != CG_LIST_HEAD(p_list));

        CG_LIST_TOP(p_list) = p_revert_node;
    }
    return 0;
}

int cg_list_recycle(cg_list_t* p_list, int(*recycle)(cg_ref_t))
{
    CG_DEBUG("cg list(%p) recycle\n", p_list);
    cg_list_node_t* p_first;
    while(CG_LIST_TOP(p_list) != CG_LIST_HEAD(p_list)) {

        p_first             = CG_LIST_TOP(p_list);
        CG_LIST_TOP(p_list) = CG_LIST_TOP(p_list)->prev;

        if (recycle) recycle(p_first->ref);

        free(p_first);
    }
    free(p_list);
    return 0;
}

int cg_list_is_empty(cg_list_t* p_list)
{
    return CG_LIST_TOP(p_list) == CG_LIST_HEAD(p_list);
}

cg_list_node_t* cg_list_push(cg_list_t* p_list, cg_ref_t ref)
{
    return cg_list_insert(p_list, CG_LIST_HEAD(p_list), ref);
}

cg_list_node_t* cg_list_insert(cg_list_t* p_list, cg_list_node_t* insert_before, cg_ref_t ref)
{
    cg_list_node_t* p_node    = (cg_list_node_t*) malloc (sizeof(cg_list_node_t));
    p_node->ref          = ref;
    p_node->prev         = insert_before->prev;
    insert_before->prev  = p_node;
    p_list->size++;
    return p_node;
}

cg_ref_t cg_list_pop(cg_list_t* p_list)
{
    if (!cg_list_is_empty(p_list)) {
        cg_list_node_t* p_pop    = CG_LIST_TOP(p_list);
        CG_LIST_TOP(p_list) = CG_LIST_TOP(p_list)->prev;
        cg_ref_t ref     = p_pop->ref;
        free(p_pop);
        p_list->size--;
        return ref;
    } 
    return NULL;
}

int cg_list_size(cg_list_t* p_list) 
{
    return p_list->size;
}

cg_list_node_t* cg_list_get(cg_list_t* p_list, int index)
{
    int pos = cg_list_size(p_list) - index;
    cg_list_node_t* p_pop = pos >=0 ? CG_LIST_TOP(p_list) : NULL;

    while ( -- pos && pos >0 ) {
        p_pop = p_pop->prev;
    }
    return p_pop ? p_pop->ref : NULL;
}