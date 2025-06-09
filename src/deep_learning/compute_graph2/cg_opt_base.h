/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-05-24 09:57:19
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-06-09 15:44:12
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_opt_base.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __CG_OPT_H__
#define __CG_OPT_H__
#include "cg_flow.h"

typedef struct cg_opt_base {
    int (*fp)(cg_tensor_t*, cg_tensor_t*, cg_tensor_t*, void* );
    int (*bp)(cg_tensor_t*, cg_tensor_t*, cg_tensor_t*, void* );
    cg_flow_elem_t flow_elem;
} cg_opt_base_t;



#endif