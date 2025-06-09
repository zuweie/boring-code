/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-05-24 09:57:25
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-06-09 17:34:09
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_znode.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __CG_ZNODE_H__
#define __CG_ZNODE_H__

#include "cg_graph.h"
#include "cg_tensor.h"
#include "cg_opt_base.h"
#include "cg_flow.h"
typedef struct cg_znode {
    cg_vertex_t   vertex;
    cg_tensor_t*  payload;
    cg_tensor_t*  gradient;
    cg_opt_base_t* opt;
    cg_flow_elem_t flow_elem;     
} cg_znode_t;
#endif