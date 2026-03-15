/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-02-19 15:01:32
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-03-15 16:21:52
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_node.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __CG_NODE_H__
#define __CG_NODE_H__

#include <string.h>
#include "cg_list.h"
#include "cg_vertex.h"

#define CG_NODE_ID(cg_node)   (((cg_node_t*)(cg_node))->vertex.id)
#define CG_NODE_TYPE(cg_node) (((cg_node_t*)(cg_node))->node_type)
#define CG_NODE_IS_OPERAND(cg_node)  (CG_NODE_TYPE(cg_node) == e_operand)
#define CG_NODE_IS_OPERATOR(cg_node) (CG_NODE_TYPE(cg_node) == e_operator)

typedef enum cg_node_type {
    e_operand = 1,
    e_operator,
} cg_node_type_t;

typedef struct cg_node {
    cg_vertex_t    vertex;
    cg_node_type_t node_type;
} cg_node_t;

static inline int cg_node_init(cg_node_t* thiz, const char* id, cg_node_type_t node_type) 
{
    thiz->node_type = node_type;
    return cg_vertex_init(&thiz->vertex, id);
}

static inline int cg_node_reset(cg_node_t* thiz) 
{
    return cg_vertex_reset(&thiz->vertex);
}

static inline int cg_node_is_respect(cg_node_t* thiz) 
{
    return cg_list_size(thiz->vertex.in) > 0;
}


#endif