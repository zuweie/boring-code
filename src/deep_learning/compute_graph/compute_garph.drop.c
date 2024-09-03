#include <stdlib.h>
#include "matrix2/matrix2.h"
#include "compute_garph.h"
#include "grad_table.h"

static int __list_insert(compute_vertex_node_t* first, compute_vertex_t* vertex)
{
    compute_vertex_node_t* new_node = (compute_vertex_node_t*)malloc(sizeof(compute_vertex_node_t));
    new_node->vertex = vertex;
    new_node->next = first;
    first = new_node;
    return 0;
}

static int __list_recycle(compute_vertex_node_t* first)
{
    compute_vertex_node_t* curr_node;
    while (first) {
        curr_node = first;
        first = first->next;
        free(curr_node);
    }
    return 0;
}

static int __vertex_recycle(compute_vertex_t* vertex) 
{
    Mat2_destroy(vertex->payload);
    Mat2_destroy(vertex->grad);
    __list_recycle(vertex->incoming_vertexes):
    __list_recycle(vertex->outcoming_vertexes);
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
        if (!first->vertex->is_input_vertex) {
            __do_forward_propagation(first->vertex)
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
static int __do_build_grad(compute_vertex_t* z, compute_vertex_t* u, gard_table_t* g_table) 
{
    // 1 找到 u 往上到 z 的所有路径。
    // 2 然后从 z 开始计算 z 对往下一级的梯度。
    // 3 计算的结果全部放入 grad_table 中。


}

int cg_init(compute_garph_t* garph);
{
    garph->vertexes = NULL;
    return 0;
}

/**
 * @brief 输入的节点不需要 fp 操作，因为 fp 的操作是上一级，对下一级的操作，input 已经是最底层，所以不需要 fp 操作。
 * 这里 bp 操作需要的上级节点对本节点的一个偏导数。
 * 
 * @param id 节点的 ID
 * @param input_data 输入的数据，如果是神经网络，那么它可能是 X、W、b、y
 * @param row 输入数据的行数
 * @param col 输入数据的列数
 * @param bp 偏导的操作
 * @param recycle 回收函数，节点用于清除内存。
 * @return compute_vertex_t* 
 */
compute_vertex_t* cgh_create_input_vertex(const char* id, vfloat_t input_data[], int rows, int cols, int(*bp)(compute_vertex_t*))
{
    matrix2_t* payload = Mat2_create(1,1);
    Mat2_load_on_shape(payload, input_data, rows, cols);
    compute_vertex_t* v = (compute_vertex_t*) malloc (sizeof(compute_vertex_t));
    v->vertex_id = id;
    v->is_input_vertex = 1;
    v->incoming_vertexes = NULL;
    v->outcomint_vertexes = NULL;
    v->payload = payload;
    v->grad = Mat2_create(1,1);
    v->forward_op = NULL;
    v->backward_op = bp;
    v->update_grad = NULL;
    v->recycle = v->recycle;
    return v;
}

/**
 * @brief 中间节点并不需要任何输入数据，它的 payload 与 gard 都是经过 payload 或者 build_grad 后向形成的。
 * 
 * @param id 顶点的 ID
 * @param fp 中间顶点 fp 操作
 * @param bp 中间点的 bp 操作
 * @param recycle 中间顶点的回收
 * @return compute_vertex_t* 
 */
compute_vertex_t* cgh_create_medial_vertex(const char* id, int(*fp)(compute_vertex_t*), int (*bp)(compute_vertex_t*))
{

}

int cgh_vertex_add(compute_garph_t* garph, compute_vertex_t* vertex)
{
    // 插入 compute garph 的顶点列表中
    __list_insert(garph->vertexes, vertex);
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
    __list_insert(to->incoming_vertexes, from);
    __list_insert(from->outcomint_vertexes, to);
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
 * @brief 向前传播，最后从最顶点Z开始，一直从上往下递归到所有有关它的 input
 * 
 * @param z 
 * @return int 
 */
int cgh_forward_propagation(compute_vertex_t* z)
{
    
}


/**
 * @brief 
 * 
 * @param z 
 * @param u 
 * @param u_size 
 * @return int 
 */
int cgh_build_grad_table(compute_vertex_t* z, compute_vertex_t* u[], int u_size)
{

}