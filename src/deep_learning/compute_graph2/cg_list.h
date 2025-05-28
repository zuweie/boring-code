/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-05-24 17:57:47
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-05-28 13:39:07
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_list.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __CG_LIST_H__
#define __CG_LIST_H__
#include "cg_node.h"

#define CG_LIST_HEAD(p_list) (&((p_list)->base))
#define CG_LIST_TOP(p_list)  (CG_LIST_HEAD(p_list)->prev)

/// @brief 这个玩意虽然叫 list，它其实是一个盏。
typedef struct cg_list {
    cg_node_t base;
} cg_list_t;

cg_list_t* cg_list_create();
cg_list_t* cg_list_create_cpy(cg_list_t* p_list);
cg_node_t* cg_list_push(cg_list_t* p_list, cg_ref_t ref);
cg_node_t* cg_list_insert(cg_list_t* p_list, cg_node_t* insert_before, cg_ref_t ref);
int cg_list_is_empty(cg_list_t* p_list);
int cg_list_revert(cg_list_t* p_list);
cg_ref_t cg_list_pop(cg_list_t* p_list);
int cg_list_recycle(cg_list_t* p_list, int(*recycle)(cg_ref_t));

#endif