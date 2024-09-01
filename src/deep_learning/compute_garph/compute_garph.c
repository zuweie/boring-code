#include <stdlib.h>
#include "compute_garph.h"



static int __vertex_list_insert(compute_vertex_node_t* first, compute_vertex_t* vertex)
{
    compute_vertex_node_t* new_node = (compute_vertex_node_t*)malloc(sizeof(compute_vertex_node_t));
    new_node->vertex = vertex;
    new_node->next = first;
    first = new_node;
    return 0;
}

static int __vertex_list_recycle(compute_vertex_node_t* first)
{
    compute_vertex_node_t* curr_node;
    while (first) {
        curr_node = first;
        first = first->next;
        free(curr_node);
    }
    return 0;
}

/**
 * @brief 从顶层节点开始做向前传播。使用递归方法，从上到下，再从下到上的向前传播。
 * 
 * @param z 
 * @return int 
 */
static int __do_forward_propagation(compute_vertex_t* z) 
{
    compute_vertex_node_t* first = z->incoming_vertexes;
    // 查看这个 incomping 顶点是否有 payloa，如果没有 payload， 证明它自己本身没有任何 op，让下层顶点做好 op 后让上层进行 op
    for (;first; first = first->next) {
        if(first->vertex->pay_load == NULL) {
            __do_backward_propagation(first);
        }
    }
    // 
    return z->forward_op(z);
}

/**
 * @brief 这个向后传播算法很难
 * 
 * @param z 
 * @param u 
 * @param grad_table
 * @return int 
 */
static int __do_backward_propagation(compute_vertex_t* z, compute_vertex_t* u, compute_grad_table_t* grad_table) 
{
    
}

int cg_init(compute_garph_t* garph);
{
    garph->vertexes = NULL;
    return 0;
}

int cgh_vertex_add(compute_garph_t* garph, compute_vertex_t* (vertex_create*)(const char*, va_list), const char* id, ...)
{
    va_list valist;
    va_start(valist, id);
    compute_vertex_t* vertex = vertex_create(id, valist);
    va_end(valist);
    // 插入 compute garph 的顶点列表中
    __vertex_list_insert(garph->vertexes, vertex);
    return 0;
}

/**
 * @brief 建立计算图的时候进行定点之间的连接。
 * 
 * @param from 出发点
 * @param to 目的点
 * @return int 返回值，一般为 0；
 */
int cg_vertex_link(compute_vertex_t* from, compute_vertex_t* to)
{
    __vertex_list_insert(to->incoming_vertexes, from);
    __vertex_list_insert(from->outcomint_vertexes, to);
    return 0;
}

int cg_recycle(compute_garph_t* garph) 
{
    compute_vertex_node_t* first = garph->vertexes;
    while(first) {
        compute_vertex_node_t* curr_node = first;
        first = first->next;
        curr_node->vertex->recycle(curr_node->vertex);
        free(curr_node->vertex);
        free(curr_node);
    }
    return 0;
}

/**
 * @brief 
 * 
 * @param z 
 * @return int 
 */
int cgh_forward_propagation(compute_vertex_t* z)
{
    
}
int cgh_backward_propagation(compute_vertex_t* z, compute_vertex_t* u)
{

}