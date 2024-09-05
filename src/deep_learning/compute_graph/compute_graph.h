/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2024-09-03 11:49:24
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2024-09-05 16:47:01
 * @FilePath: /boring-code/src/deep_learning/compute_graph/compute_graph.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __COMPUTE_GRAPH_H__
#define __COMPUTE_GRAPH_H__
#include "matrix2/matrix2.h"
#include "cg_graph.h"

typedef enum {
    e_mediate = 1,
    e_input,
    e_training_params
} node_type_t;

typedef struct _compute_node {

    /// @brief 每个节点的唯一 ID
    char        str_node_id[128];
    /// @brief 如果是终端节点，payload 是输入的数据，若是中间节点则是下层节点往上计算的结果，此 payload 也作为上一层的输入。
    matrix2_t*   p_payload;
    /// @brief 最终节点关于本节点的梯度。
    matrix2_t*   p_gradient;
    /// @brief 梯度数据版本，在每次前向传播后，总图会更新版本号，在做链式求导时会对比总图的版本，根据版本的新旧来确认是否要更新本节点的梯度。
    int          gradient_version;
    /// @brief 制定 note 的类型。1、为中间节点。2、为训练输入节点，例如 X，y。 3、模型参数节点，例如 W，b.
    node_type_t  node_type;
    /// @brief 保存着本节点到最终输出节点的路径。
    cg_list_t*   p_gradient_paths;
    /// @brief 这玩意的生命周期由 cg_graph 来管理，不要在 compute_graph 层面进行生命周期管理
    cg_vertex_t* p_vertex;
    /// @brief 节点自己的内存回收函数
    int (*recycle) (compute_node_t* p_node);
    /// @brief  前向传播的函数
    int (*forward_op) (compute_node_t* p_node);
    /// @brief 后向传播求梯度的函数
    int (*backward_op) (compute_node_t* p_node);
    /// @brief 根据梯度更新 payload 的函数, 例如参数 W = W - \lamada \delta W 或者是 b = b - \delat b
    int (*update_payload) (compute_node_t* p_node);
    /// @brief 总图的指针
    compute_graph_t* p_compute_graph;
    /// @brief  保留字段，可以放入指针或者整型
    cg_ref_t reserved[8];
} compute_node_t;


typedef struct _compute_graph {
    /// @brief 训练的时候用的
    void* p_compute_params;
    int update_version;
    cg_graph_t graph;
    cg_list_t* p_nodes;
    int (*build_graph)(compute_graph_t*, void*)
} compute_graph_t;

int compute_graph_init(compute_graph_t* p_thiz, void* p_compute_params, int (*build_graph)(compute_graph_t*, void* build_params));
int compute_graph_building(compute_graph_t* p_thiz, void* p_build_params);
int compute_graph_recycle(compute_graph_t* p_thiz);

int compute_graph_add_note(compute_graph_t* p_thiz, compute_node_t* p_node);
int compute_graph_link_note(compute_graph_t* p_thiz, const char* p_from_id, const char* p_to_id);

// 最重要这三个函数。
int compute_graph_forward_propagation(compute_graph_t* p_thiz, const char* str_zid);
int compute_graph_build_gradient(compute_graph_t* p_thiz, const char* str_zid, const char* str_uid);

compute_node_t* compute_create_mediate_node(const char* id,  int (*recycle)(compute_node_t*), int (*fp)(compute_node_t*, void*), int (*bp)(compute_node_t*, void*));
compute_node_t* compute_create_input_node(const char* id,  int(*recycle)(compute_node_t*));
compute_node_t* compute_create_training_params_node(const char* id, matrix2_t* p_initialization, int(*recycle)(compute_node_t*), int (*bp)(compute_node_t*, void*), int(*update_payload)(compute_node_t*, void*));

int compute_set_input(compute_graph_t* p_thiz, const char* str_id, matrix2_t* p_input);
int compute_update_training_params(compute_graph_t* p_thiz, const char* str_id);

#endif