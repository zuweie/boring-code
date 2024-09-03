/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2024-09-02 13:57:35
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2024-09-03 11:13:53
 * @FilePath: /boring-code/src/deep_learning/compute_garph/cg_list.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __CG_LIST_H__
#define __CG_LIST_H__
#include "cg_node.h"

#define LIST_HEAD(p_list) &((p_list)->head)
#define LIST_TAIL(p_list) &((p_list)->head)

typedef struct {
    cg_node_t base;
    cg_node_t* p_top;
} cg_list_t;

cg_list_t* cg_list_create();
cg_list_t* cg_list_create_cpy(cg_list_t* p_list);
int cg_list_push(cg_list_t* p_list, cg_ref_t ref);
int cg_list_is_empty(cg_list_t* p_list);
cg_ref_t cg_list_pop(cg_list_t* p_list);
int cg_list_recycle(cg_list_t* p_list, int(*recycle)(cg_ref_t));

#endif