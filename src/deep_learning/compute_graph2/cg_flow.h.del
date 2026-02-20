/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-06-09 12:52:24
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-06-15 09:29:29
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_flow.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __CG_FLOW_H__
#define __CG_FLOW_H__
#include <stdlib.h>
typedef enum {

    e_beg = 1,
    e_end,
    e_znode,
    e_opt,

} flow_elem_type_t;

typedef union cg_flow_elem {
    flow_elem_type_t elem_type;
} cg_flow_elem_t;

static inline cg_flow_elem_t* flow_beg()
{
    cg_flow_elem_t* elem = (cg_flow_elem_t*) malloc (sizeof (cg_flow_elem_t));
    *elem = (cg_flow_elem_t) {.elem_type = e_beg};
    return elem;
}

static inline cg_flow_elem_t* flow_end()
{
    cg_flow_elem_t* elem = (cg_flow_elem_t*) malloc (sizeof (cg_flow_elem_t));
    *elem = (cg_flow_elem_t) {.elem_type = e_end};
    return elem;
}

#endif