/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-05-24 09:57:19
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-06-15 21:23:48
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_opt_base.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __CG_OPT_H__
#define __CG_OPT_H__
#include "cg_flow.h"

typedef struct cg_znode_base cg_znode_base_t;

// 这个是一个继承 flow_elem_t 的 struct
typedef struct cg_opt_base {
    cg_flow_elem_t flow_elem;
    int (*fp)(cg_znode_base_t*, void* );
    int (*bp)(cg_znode_base_t*, cg_znode_base_t*, void* );
    int (*recycle) (struct cg_opt_base * thiz);
    struct cg_opt_base* next;
} cg_opt_base_t;



#endif