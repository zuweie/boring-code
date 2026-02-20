/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-02-19 14:20:30
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-02-20 11:00:40
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_operator.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __CG_OPERATOR_H__
#define __CG_OPERATOR_H__

#include "cg_node.h"

typedef struct cg_operator {

    cg_node_t _base;

    int (*calculate) (cg_node_t*, cg_list_t*, cg_ref_t);
    int (*derivative)(cg_list_t*, cg_ref_t*, cg_ref_t);

} cg_operator_t;

static inline int cg_operator_init(
    cg_operator_t* thiz, 
    const char* id, 
    int (*calculate) (cg_node_t*, cg_list_t*, cg_ref_t), 
    int (*derivative)(cg_list_t*, cg_ref_t, cg_ref_t)
) {
    cg_node_init(&thiz->_base, e_operator);
    *thiz = (cg_operator_t) {
        .calculate  = calculate,
        .derivative = derivative
    };
    return 0;
};

static inline int cg_operator_reset(cg_operator_t* thiz) {
    cg_node_reset(&thiz->_base);
    return 0;
};
#endif