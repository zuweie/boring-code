/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-06-17 11:14:44
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-02-19 15:57:00
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_vertex.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __CG_VERTEX_H__
#define __CG_VERTEX_H__

typedef struct cg_list cg_list_t;

typedef struct cg_vertex
{
    const char id[64];
    cg_list_t*  in;
    cg_list_t*  out;
} cg_vertex_t;

#endif