/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2024-08-26 17:05:19
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2024-09-01 15:32:04
 * @FilePath: /boring-code/src/deep_learning/compute_garph.h
 * @Description: 计算图的实现。
 */
#ifndef __COMPUTE_GARPH_H__
#define __COMPUTE_GARPH_H__

#include <stdarg.h>
#include "matrix2/matrix2.h"
typedef struct _compute_vertex compute_vertex_t;
typedef struct _compute_vertex_node_t compute_vertex_node_t;

/// @brief 
struct _compute_vertex_node_t {
    compute_vertex_t* vertex;
    compute_vertex_node_t* next;
}

struct _compute_vertex {

    // 每个节点唯一的 ID
    const char* vertex_id;
    // 链表中元素是指向自己的顶点。
    compute_vertex_node_t* incoming_vertexes;
    // 链表中的元素是自己指向的顶点。
    compute_vertex_node_t* outcomint_vertexes;

    // 这是存储计算结果的指针，对于本节点他是一个由下层节点提供数据计算结果得到的结果，而对于往上的节点提供计算材料。
    matrix2_t* payload;
    // 最高点对本顶点的梯度。
    matrix2_t* grad;
    // 向前传播时 对 incoming 顶点的操作。
    int (forward_op*) (compute_vertex_t* thiz);
    // 向后传播时 对下一级的 顶点进行操作。
    int (backward_op*) (compute_vertex_t* thiz);
    // 当神经网络训练的时候，使用 grad 对 payload 进行更新参数
    int (update_payload)(compute_vertex_t* thiz);
    // 顶点自己的 recycle 
    int (recycle*)(compute_vertex_t* thiz);
}

typedef struct _compute_garph {
    // 计算图维护一个存储所有节点的链表。
    compute_vertex_node_t* vertexes;
} compute_garph_t;

int cgh_init(compute_garph_t* garph);
int cgh_vertex_add(compute_garph_t* garph, compute_vertex_node_t* (vertex_create*)(va_list), const char*, ...);
int cgh_vertex_link(compute_vertex_t* from, compute_vertex_t* to);
int cgh_recycle(compute_garph_t* garph);

int cgh_forward_propagation(compute_vertex_t* z);
int cgh_backward_propagation(compute_vertex_t* z, compute_vertex_t* u);

compute_vertex_t* cgh_vertex_create_with_null_payload(const char* id, va_list valist);
compute_vertex_t* cgh_vertex_create_with_matrix_payload(const char* id, va_list valist);
#endif 