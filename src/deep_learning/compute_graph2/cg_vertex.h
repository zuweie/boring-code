/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-06-17 11:14:44
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-03-15 16:45:00
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_vertex.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __CG_VERTEX_H__
#define __CG_VERTEX_H__

#include "cg_list.h"

typedef struct cg_list cg_list_t;
typedef struct cg_vertex
{
    const char id[64];
    cg_list_t*  in;
    cg_list_t*  out;
    
} cg_vertex_t;

static inline int cg_vertex_init(cg_vertex_t* vertex, const char* id) 
{
    vertex->in  = cg_list_create();
    vertex->out = cg_list_create();
    return 0;
}

static inline int cg_vertex_reset(cg_vertex_t* vertex) 
{
    cg_list_recycle(vertex->in, NULL);
    cg_list_recycle(vertex->out, NULL);
    
    return 0;
}

#endif