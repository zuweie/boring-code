/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-05-24 09:57:25
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-06-15 08:17:58
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_znode.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __CG_ZNODE_BASE_H__
#define __CG_ZNODE_BASE_H__

#include "cg_flow.h"
#include "cg_graph.h"

typedef struct cg_opt_base cg_opt_base_t;
typedef struct cg_base cg_base_t;

// 这个是一个继承 cg_flow_elemt_t 的 struct
typedef struct cg_znode_base {
    cg_flow_elem_t flow_elem;
    cg_vertex_t    vertex;
    cg_list_t*     gradient_paths;
    cg_opt_base_t* opt;
    int            gradient_version;
} cg_znode_base_t;

// 基类的构造函数
int cg_znode_base_construct(cg_base_t*, cg_znode_base_t*);
// 基类的析构函数
int cg_znode_base_deconstruct(cg_znode_base_t*);
#endif