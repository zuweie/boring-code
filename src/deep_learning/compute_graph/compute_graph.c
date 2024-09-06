#include <string.h>
#include "cg_node.h"
#include "cg_list.h"
#include "cg_graph.h"
#include "compute_graph.h"
#include "base/container_of.h"
#include "matrix2/matrix2.h"


static int __compute_node_recycle(compute_node_t* p_compute_node) 
{
    // 回收自己的资源
    p_compute_node->recycle(p_compute_node);
    // 启动自毁
    free(p_compute_node);
    return 0;
}
static compute_node_t* __search_node_by_id(compute_graph_t* p_compute_graph, const char* id) 
{
    cg_node_t* p_first = p_compute_graph->p_nodes->p_top;
    while (p_first != LIST_HEAD(p_compute_graph->p_nodes))
    {
        compute_node_t* p_node = (compute_node_t*) p_first->ref;
        if (strcmp(p_node->str_node_id, id) == 0) return p_node;
        p_first = p_first->prev;
    }
    
    return NULL;
}


static int __do_forward_op(compute_node_t* p_znode) 
{
    // 只有中间的节点才有 fp 操作
    if (p_znode->node_type == e_mediate) {
        
        // 检查一遍这个节点是否是还有其他的 e_mediate
        cg_list_t* p_income_list = p_znode->p_vertex->p_income_vertexes;
        cg_node_t* p_first = p_income_list->p_top;
        while (p_first != LIST_HEAD(p_income_list))
        {
            compute_node_t* p_income_node = CONVERT_TO_COM_NODE(p_first->ref);
            if (p_income_node->node_type == e_mediate) {
                __do_forward_op(p_income_node);
            }
        }
        // 这里的 op 操作会
        return p_znode->forward_op(p_znode);
    }

    return 0;
}

static int __do_build_gradient(compute_node_t* p_znode, compute_node_t* p_unode)
{   
    
    compute_graph_t* p_graph = p_unode->p_compute_graph;

    // 比对自己梯度版本号是否ok。
    if (p_unode->gradient_version != p_graph->update_version) {

        if (p_znode == p_unode) {
            // 最外层的自己导自己，那就是 1，调用 z_node 的导数，将自己的 gradient 矩阵变成1
            p_unode->backward_op(p_unode);
            p_unode->gradient_version = p_graph->update_version;
            return 0;
        }

        // 这个节点的梯度是旧的，重新更新它的梯度。
        if (!p_unode->p_gradient_paths) {
            // 到 z 的导数路径为空，则说明未曾开始求导数路径。
            cg_list_t* gradient_paths = cg_list_create();
            cg_graph_search_paths(p_unode->p_vertex, p_znode->p_vertex, gradient_paths);
            p_unode->gradient_paths= gradient_paths;
        }

        // 
        cg_node_t* p_paths_first = p_unode->p_gradient_paths->p_top;
        while(p_paths_first != LIST_HEAD(p_unode->p_gradient_paths)) {
            
            // 更新每一条导数链的节点的 gradient 矩阵。
            
            cg_list_t* gradient_path = (cg_list_t*) p_paths_first->ref;

            // 从第二个节点开始就是它的上一级节点，让他的上一层的节点做梯度更新。
            cg_node_t* p_superior = gradient_path->p_top->prev;
            while(p_superior != LIST_HEAD(gradient_path)) {
                compute_node_t* p_superior_node = CONVERT_TO_COM_NODE(p_superior->ref);
                __do_build_gradient(p_znode, p_superior);
                p_superior = p_superior->prev;
            }
            
            p_paths_first = p_paths_first->prev;
        }

        // 让所有导数链上的节点更新完 gradient 后，让自己更新
        p_unode->backward_op(p_unode);
        // 更新梯度版本，使得和总图一样, 总图的 update version 会在 一轮 forward propagation 后加 1；
        p_unode->gradient_version = p_graph->update_version;
    }
    // 否则什么都不用干，因为这个节点的梯度是最新的。
    return 0;
}

int compute_graph_init(compute_graph_t* p_thiz, void* p_compute_params, int (*build_graph)(compute_graph_t*, void*))
{
    p_thiz->p_compute_params    = p_compute_params;
    p_thiz->update_version      = 0;
    p_thiz->nodes               = cg_list_create();
    p_thiz->build_graph         = build_graph;
    cg_graph_init(&p_thiz->graph);
    return 0;
}
int compute_graph_building(compute_graph_t* p_thiz, void* p_build_params) 
{
    return p_thiz->build_graph(p_thiz, p_build_params);
}
int compute_graph_recycle(compute_graph_t* p_compute_graph)
{
    cg_list_recycle(p_compute_graph->p_nodes, __compute_node_recycle);
    cg_graph_recycle(&p_compute_graph->graph);
    return 0;
}

int compute_graph_add_note(compute_graph_t* p_compute_graph, compute_node_t* p_node)
{
    // 
    cg_vertex_t* p_vertex    = cg_graph_add_vertex(&p_compute_graph->graph, p_node->str_node_id);
    p_node->p_vertex         = p_vertex;
    p_node->p_compute_graph  = p_compute_graph;
    cg_list_push(p_compute_graph->p_nodes, p_node);
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
int compute_graph_forward_propagation(compute_graph_t* p_compute_graph, const char* z)
{
    compute_node_t* p_znode = __search_node_by_id(p_compute_graph, z);
    __do_forward_op(p_znode);
    p_compute_graph->update_version++;
    
}

int compute_graph_build_gradient(compute_graph_t* p_compute_graph, const char* z, const char* u)
{
    compute_node_t* p_znode = __search_node_by_id(p_compute_graph, z);
    compute_node_t* p_unode = __search_node_by_id(p_compute_graph, u);

    return __do_build_gradient(p_znode, p_unode);
}


compute_node_t* compute_create_mediate_node(const char* id, int (*init)(compute_node_t*), int (*recycle)(compute_node_t*), int (*fp)(compute_node_t*), int (*bp)(compute_node_t*))
{
    compute_node_t* p_node_1     = (compute_node_t*) malloc (sizeof(compute_node_t));
    strcpy(p_node_1->str_node_id, id);
    p_node_1->p_payload          = Mat2_create(1,1);
    p_node_1->p_gradient         = Mat2_create(1,1);
    p_node_1->gradient_version   = 0;
    p_node_1->node_type          = e_mediate;
    p_node_1->p_gradient_paths   = NULL;
    p_node_1->p_vertex           = NULL;
    p_node_1->recycle            = recycle;
    p_node_1->forward_op         = fp;
    p_node_1->backward_op        = bp;
    // 1 型节点是不需要 update payload，它们的 payload 靠它们的 fp 来update的。 
    p_node_1->update_payload     = NULL;
    p_node_1->p_compute_graph    = NULL;
    if (init) init(p_node_1);
    return p_node_1;
}

compute_node_t* compute_create_input_node(const char* id, int (*init)(compute_node_t*), int(*recycle)(compute_node_t*))
{
    compute_node_t* p_node_2    = (compute_node_t*) malloc (sizeof(compute_node_t));
    //p_node_2->str_node_id       = id;
    strcpy(p_node_2->str_node_id, id);
    p_node_2->p_payload         = Mat2_create(1,1);
    // 2 型节点是不需要 gradient 的, 因为它们是 X，y
    p_node_2->p_gradient        = NULL;
    // gradient_version 对于 2 型节点没有意义。
    p_node_2->gradient_version  = 0;
    p_node_2->node_type         = e_input;
    p_node_2->p_gradient_paths  = NULL;
    p_node_2->p_vertex          = NULL;
    p_node_2->recycle           = recycle;
    // 终端的节点不需要 fp 操作。
    p_node_2->forward_op        = NULL;
    // 2 型节点不需要梯度，不需要 bp 操作
    p_node_2->backward_op       = NULL;
    // 它们的 payload 更新是靠外部输入的，即每轮训练输入不同的记录。
    p_node_2->update_payload    = NULL;
    p_node_2->p_compute_graph   = NULL;
    if (init) init(p_node_2);
    return p_node_2;
}

compute_node_t* compute_create_training_params_node(const char* id, matrix2_t* p_initialization, int (*init)(compute_node_t*),int(*recycle)(compute_node_t*), int (*bp)(compute_node_t*), int(*update_payload)(compute_node_t*))
{
    compute_node_t* p_node_3    = (compute_node_t*) malloc (sizeof(compute_node_t));
    //p_node_3->str_node_id       = id;
    strcpy(p_node_3->str_node_id, id);
    p_node_3->p_payload         = Mat2_create_cpy(p_initialization);
    p_node_3->p_gradient        = Mat2_create_cpy(p_initialization);
    p_node_3->gradient_version  = 0;
    p_node_3->node_type         = e_training_params;
    p_node_3->p_gradient_paths  = NULL;
    p_node_3->p_vertex          = NULL;
    p_node_3->recycle           = recycle;
    // 终端节点不需要 fp 操作
    p_node_3->forward_op        = NULL;
    p_node_3->backward_op       = bp;
    // update_payload 就是更新训练参数。
    p_node_3->update_payload    = update_payload;
    p_node_3->p_compute_graph   = NULL;
    if (init) init(p_node_3);
    return p_node_3;
}

int compute_set_input(compute_graph_t* p_compute_graph, const char* id, matrix2_t* input)
{
    compute_node_t* p_node = __search_node_by_id(p_compute_graph, id);
    if (p_node->node_type == e_input) {
        Mat2_cpy(p_node->p_payload, input);
        return 0;
    }
    return -1;
}

int compute_update_training_params(compute_garph_t* p_compute_graph, const char* id)
{
    compute_node_t* p_node = __search_node_by_id(p_compute_graph, id);
    p_node->update_payload(p_node);
}