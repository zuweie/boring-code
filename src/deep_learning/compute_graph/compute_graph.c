#include "cg_node.h"
#include "cg_list.h"
#include "cg_graph.h"
#include "compute_graph.h"
#include "base/container_of.h"
#include "matrix2/matrix2.h"

static compute_node_t* __search_node_by_id(compute_graph_t* p_compute_graph, const char* id) 
{
    cg_node_t* p_first = p_compute_graph->graph.p_vertexes->p_top;
    while (p_first != LIST_HEAD(p_compute_graph->graph.p_vertexes))
    {
        cg_vertex_t* p_v = (cg_vertex_t*) p_first->ref;
        compute_node_t* p_compute_node = container_of(pv, compute_node_t, p_vertex);
        if (strcmp(p_compute_node->p_compute_node_id, id) == 0) return p_compute_node;
        p_first = p_first->prev;
    }
    return NULL;
}
int compute_graph_init(compute_graph_t* p_compute_graph, const char* z, void* p_op_params)
{
    p_compute_graph->z              = z;
    p_compute_graph->p_op_params    = p_op_params;
    p_compute_graph->update_version = 0;
    cg_graph_init(&p_compute_graph->graph);
    return 0;
}
int compute_graph_recycle(compute_graph_t* p_compute_graph)
{
    cg_node_t* p_first = p_compute_graph->graph.p_vertexes->p_top;
    while (p_first != LIST_HEAD(p_compute_graph->graph.p_vertexes))
    {
        cg_vertex_t* p_v = (cg_vertex_t*) p_first->ref;
        compute_node_t* p_compute_node = container_of(p_v, compute_node_t, p_vertex);
        p_compute_node->recycle(p_compute_node);
        free(p_compute_node);
        p_first = p_first->prev;
    }
    
    cg_graph_recycle(&p_compute_graph->graph);
    return 0;
}

int compute_graph_add_note(compute_graph_t* p_compute_graph, compute_node_t* p_node)
{
    cg_vertex_t* p_vertex    = cg_graph_add_vertex(&p_compute_graph->graph, p_node->p_compute_node_id);
    p_node->p_vertex         = p_vertex;
    p_node->p_compute_graph  = p_compute_graph;
    return 0;
}
int compute_graph_link_note(compute_graph_t* p_compute_graph, const char* p_from_id, const char* p_to_id)
{
    compute_node_t* p_from_node = __search_node_by_id(p_compute_graph, p_from_id);
    compute_node_t* p_to_node   = __search_node_by_id(p_compute_graph, p_to_id);
    if (p_from_node && p_to_node) {
        cg_graph_link(p_from_node->p_vertex, p_to_node->p_vertex);
        return 0;
    }
    return -1;
}

// 最重要这三个函数。
int compute_graph_forward_propagation(compute_graph_t* p_compute_graph)
{
    compute_node_t* p_z = __search_node_by_id(p_compute_graph->z);
    // 统计有多少个 income 节点。
    
}
int compute_graph_build_gradient(compute_graph_t* p_compute_graph)
{

}
int compute_graph_update_gradient(compute_graph_t* p_compute_graph)
{
}

compute_node_t* compute_node_create_type1(const char* id, int (*recycle)(compute_node_t*), int (*fp)(compute_node_t*), int (*bp)(compute_node_t*))
{
    compute_node_t* p_node_1     = (compute_node_t*) malloc (sizeof(compute_node_t));
    p_node_1->p_compute_node_id  = id;
    p_node_1->p_payload          = Mat2_create(1,1);
    p_node_1->p_gradient         = Mat2_create(1,1);
    p_node_1->gradient_version   = 0;
    p_node_1->node_type          = 1;
    p_node_1->p_paths_to_z       = cg_list_create();
    p_node_1->p_vertex           = NULL;
    p_node_1->recycle            = recycle;
    p_node_1->forward_op         = fp;
    p_node_1->backward_op        = bp;
    // 1 型节点是不需要 update payload，它们的 payload 靠它们的 fp 来update的。 
    p_node_1->update_payload     = NULL;
    p_node_1->p_compute_graph    = NULL;
    return p_node_1;
}
compute_node_t* compute_node_create_type2(const char* id, matrix2_t* input, int(*recycle)(compute_node_t*))
{
    compute_node_t* p_node_2    = (compute_node_t*) malloc (sizeof(compute_node_t));
    p_node_2->p_compute_node_id = id;
    p_node_2->p_payload         = Mat2_create_cpy(input);
    // 2 型节点是不需要 gradient 的, 因为它们是 X，y
    p_node_2->p_gradient        = NULL;
    // gradient_version 对于 2 型节点没有意义。
    p_node_2->gradient_version  = 0;
    p_node_2->node_type         = 2;
    p_node_2->p_paths_to_z      = cg_list_create();
    p_node_2->p_vertex          = NULL;
    p_node_2->recycle           = recycle;
    // 终端的节点不需要 fp 操作。
    p_node_2->forward_op        = NULL;
    // 2 型节点不需要梯度，不需要 bp 操作
    p_node_2->backward_op       = NULL;
    // 它们的 payload 更新是靠外部输入的，即每轮训练输入不同的记录。
    p_node_2->update_payload    = NULL;
    p_node_2->p_compute_graph   = NULL;
    return p_node_2;
}
compute_node_t* compute_node_create_type3(const char* id, matrix2_t* input, int(*recycle)(compute_node_t*), int (*bp)(compute_node_t*), int(*update_payload)(compute_node_t*))
{
    compute_node_t* p_node_3    = (compute_node_t*) malloc (sizeof(compute_node_t));
    p_node_3->p_compute_node_id = id;
    p_node_3->p_payload         = Mat2_create(1,1);
    p_node_3->p_gradient        = Mat2_create(1,1);
    p_node_3->gradient_version  = 0;
    p_node_3->node_type         = 3;
    p_node_3->p_paths_to_z      = cg_list_create();
    p_node_3->p_vertex          = NULL;
    p_node_3->recycle           = recycle;
    // 终端节点不需要 fp 操作
    p_node_3->forward_op        = NULL;
    p_node_3->backward_op       = bp;
    p_node_3->update_payload    = update_payload;
    p_node_3->p_compute_graph   = NULL;
    return p_node_3;
}
int compute_node_update_payload_type2(compute_graph_t* p_compute_graph, const char* id, matrix2_t* input)
{
    compute_node_t* p_node = __search_node_by_id(p_compute_graph, id);
    Mat2_cpy(p_node->payload, input);
    return 0;
}