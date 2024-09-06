#include <stdio.h>
#include "compute_graph/cg_node.h"
#include "compute_graph/cg_list.h"
#include "compute_graph/compute_graph.h"
#include "ann_cg_simulation.h"

static int __bp_W_node(compute_node_t* p_thiz)
{
    /**
     * @brief 如果上级节点是 W dot X， 那么这个 W 节点的梯度便是 X^T.
     * 
     */
    printf("build gradient of %s:W.\n", p_thiz->str_node_id);
    return 0;
}

static int __update_payload_W_node(compute_node_t* p_thiz)
{
    /**
     * @brief 在更新完 W 节点的梯度矩阵，那么 W 的更新味 W = W - \delta GofW.
     * 
     */
    printf("update %s:W by %s:W`s gradient with learning rate.\n", p_thiz->str_node_id, p_thiz->str_node_id);
    return 0;
}


static int __bp_b_node(compute_node_t* p_thiz) 
{
    /**
     * @brief 如果上级节点是 (W dot X + b), 那么上级节点对 b 节点的梯度为 1.
     * 
     */
    printf("build gradient of %s:b.\n", p_thiz->str_node_id);
    return 0;
}

static int __update_payload_b_node(compute_node_t* p_thiz) 
{

    printf("update %s:b by %s:b`s gradient with learning rate.\n", p_thiz->str_node_id, p_thiz->str_node_id);
    return 0;
}

static int __fp_u_x_dot_w_node(compute_node_t* p_thiz)
{
    printf("doing fp on %s:( W dot X ).\n", p_thiz->str_node_id);
    return 0;

}
static int __bp_u_x_dot_w_node(compute_node_t* p_thiz) 
{
    printf("build gradient of %s:( W dot X).\n", p_thiz->str_node_id);
    return 0;
}

static int __fp_u_plus_b_node(compute_node_t* p_thiz)
{
    printf("doing fp on %s:((W dot X) + b).\n", p_thiz->str_node_id);
    return 0;
}
static int __bp_u_plus_b_node(compute_node_t* p_thiz)
{
    printf("build gradient of %s:((W dot X) + b).\n", p_thiz->str_node_id);
    return 0;
}

static int __fp_u_act_input_node(compute_node_t* p_thiz) 
{
    printf("doing fp on %s:act((W dot x) + b).\n", p_thiz->str_node_id);
    return 0;
}

static int __bp_u_act_input_node(compute_node_t* p_thiz) 
{
    /**
     * @brief 上级节点问 (act(u)), 那么上级节点节点对本节点的梯度问 act`(u)
     * 
     */
    printf("build gradient of %s:act( (W dot x) + b).\n", p_thiz->str_node_id);
    return 0;
}

static int __fp_u_act_y_hat_node(compute_node_t* p_thiz)
{
    printf("doing fp on %s:act(y-hat).\n", p_thiz->str_node_id);
    return 0;
}

static int __bp_u_act_y_hat_node(compute_node_t* p_thiz)
{
    /**
     * @brief 如果上级节点使用的 mse( 1/2 ||y^-y||_2 )， 那么上级节点对本节点的梯度为 y^ - y.
     * 
     */
    printf("build gradient of %s:act(y-hat).\n", p_thiz->str_node_id);
    return 0;
}
static int __fp_output_node(compute_node_t* p_thiz)
{
    /**
     * @brief 如果 loss 使用 mse 那么公式为 1/2 || y^-y||_2
     * 
     */
    printf("doing fp on %s:z.\n", p_thiz->str_node_id);
    return 0;
}  

static int __bp_output_node(compute_node_t* p_thiz) 
{
    /**
     * @brief loss节点对loss节点永远为 1
     * 
     */
    printf("build gradient of %s:z, it away is 1.\n ", p_thiz->str_node_id);
    return 0;
}

static int __recycle_note(compute_node_t* p_thiz) 
{
    // 释放主要的矩阵
    if (p_thiz->p_payload)  Mat2_destroy(p_thiz->p_payload);
    if (p_thiz->p_gradient) Mat2_destroy(p_thiz->p_gradient);
    // 把导数链条释放。
    if (p_thiz->p_gradient_paths) {
        cg_ref_t gradient_path;
        while ( gradient_path = cg_list_pop(p_thiz->p_gradient_paths) ) {
            cg_list_recycle(gradient_path, NULL);
        }
        cg_list_recycle(p_thiz->p_gradient_paths, NULL);
    }

    return 0;
}
/**
 * @brief 根据神经网络的每一层的维度，和总共的层数进行扑街的计算图的建立，真他妈的复杂到嘿咁，不单单是连图复杂，每个节点梯度的求法也是复杂到嗨咁。丢距老味嘿。
 * 
 * @param p_thiz 总图
 * @param params 一些可能用到的额外参数。
 * @return int 
 */
static int __build_compute_graph(compute_graph_t* p_thiz, void* params)
 {
    // 根据神经网络的层数来生成计算图三个节点是确定的，X y 和 output
    cg_list_t* p_node_stack = cg_list_create();
    char str_node_id_buff[128];
    int u_i = 1;
    int W_i = 1;
    int b_i = 1;
    size_t rows_W, cols_W;
    size_t rows_b, cols_b = 1;
    compute_node_t* p_W;
    compute_node_t* p_b;
    compute_node_t* p_u;
    compute_node_t* p_X; 
    compute_node_t* p_z;
    compute_node_t* p_y;
    ann_cg_params_t* cg_params = (ann_cg_params_t*) p_thiz->p_compute_params;
    matrix2_t* mat_W = Mat2_create(1,1);
    matrix2_t* mat_b = Mat2_create(1,1);
    compute_node_t* pop;
    // 开始建图
    p_X = compute_create_input_node("X", &__recycle_note);
    compute_graph_add_note(p_thiz, p_X);
    cg_list_push(p_node_stack, p_X);

    for(int i=1; i<cg_params->layers_size-1; ++i) {

        rows_W = cg_params->layers_dimension[i];
        cols_W = cg_params->layers_dimension[i-1];
        rows_b = cg_params->layers_dimension[i];

        Mat2_reshape(mat_W, rows_W, cols_W);
        Mat2_fill_random(mat_W, 0, 1);

        Mat2_reshape(mat_b, rows_b, cols_b);
        Mat2_fill_random(mat_b, 0, 1);
        // build fuck node
        sprintf(str_node_id_buff, "W_%d", W_i++);
        p_W = compute_create_training_params_node(str_node_id_buff, mat_W, &__recycle_note, &__bp_W_node, &__update_payload_W_node);
        
        // 将 W 节点加到计算图中去
        compute_graph_add_note(p_thiz, p_W);
        cg_list_push(p_node_stack, p_W);

        // 生成 u = W dot x 的中间节点
        sprintf(str_node_id_buff, "u_%d", u_i++);
        p_u = compute_create_mediate_node(str_node_id_buff, &__recycle_note, &__fp_u_x_dot_w_node, &__bp_u_x_dot_w_node);
        // 将 u 加入到 计算图中去。
        compute_graph_add_note(p_thiz, p_u);

        while (pop = cg_list_pop(p_node_stack))
        {
           // 把 X 节点和 W 节点连到 u 节点上去。
           compute_graph_link_note(p_thiz, pop->str_node_id, p_u->str_node_id);
        }
        cg_list_push(p_node_stack, p_u);
        
        // 生成 b 节点
        sprintf(str_node_id_buff, "b_%d", b_i++);
        p_b = compute_create_training_params_node(str_node_id_buff, mat_b, &__recycle_note, &__bp_b_node, &__update_payload_b_node);
        // 将节点加载到计算图中
        compute_graph_add_note(p_thiz, p_b);
        cg_list_push(p_node_stack, p_b);

        // 生成 u = W dot x + b
        sprintf(str_node_id_buff, "u_%d", u_i++);
        p_u = compute_create_mediate_node(str_node_id_buff, &__recycle_note, &__fp_u_plus_b_node, &__bp_u_plus_b_node);
        compute_graph_add_note(p_thiz, p_u);
        
        while (pop = cg_list_pop(p_node_stack))
        {
            // 将加 b 节点连到 u 上面去。
            compute_graph_link_note(p_thiz, pop->str_node_id, p_u->str_node_id);
        }
        cg_list_push(p_node_stack, p_u);
        
        
        // 倒数第二层
        if (i == cg_params->layers_size-2) {
            // 如果是最后一层那么，将来这里就就是从这个节点开始前向传播，得到的结果便是
            sprintf(str_node_id_buff, "y_hat");
            p_u = compute_create_mediate_node(str_node_id_buff, &__recycle_note, &__fp_u_act_y_hat_node, & __bp_u_act_y_hat_node);
        } else {
            sprintf(str_node_id_buff, "u_%d", u_i++);
            p_u = compute_create_mediate_node(str_node_id_buff, &__recycle_note, &__fp_u_act_input_node, &__bp_u_act_input_node);

        }
        compute_graph_add_note(p_thiz, p_u);
        
        while(pop = cg_list_pop(p_node_stack)) {
            // 把上面的 u = W dot x + b 节点连到这个激活节点上去
            compute_graph_link_note(p_thiz, pop->str_node_id, p_u->str_node_id);
        }

        cg_list_push(p_node_stack, p_u);
    }

    sprintf(str_node_id_buff, "y");
    p_y = compute_create_input_node(str_node_id_buff, &__recycle_note);
    compute_graph_add_note(p_thiz, p_y);
    cg_list_push(p_node_stack, p_y);

    sprintf(str_node_id_buff, "z");
    p_z = compute_create_mediate_node(str_node_id_buff, &__recycle_note, &__fp_output_node, &__bp_output_node);
    compute_graph_add_note(p_thiz, p_z);
    
    while ( pop = cg_list_pop(p_node_stack))
    {
        compute_graph_link_note(p_thiz, pop->str_node_id, p_z->str_node_id);
    }
    
    cg_list_recycle(p_node_stack, NULL);
    Mat2_destroy(mat_W);
    Mat2_destroy(mat_b);
    return 0;
}

int ann_cg_train(matrix2_t* p_data, matrix2_t* p_label, ann_cg_params_t* p_params, compute_graph_t* p_compute_graph)
{
    // 初始化这个计算图
    compute_graph_init(p_compute_graph, p_params, &__build_compute_graph);
    // 根据神经网络建立图的连接
    compute_graph_building(p_compute_graph, NULL);
    matrix2_t* mat_fake_input = Mat2_create(5, 1);
    matrix2_t* mat_fake_lable = Mat2_create(3, 1);

    // 做两轮的模拟训练
    for (int i=0; i<2; ++i) {
        compute_set_input(p_compute_graph, "X", mat_fake_input);
        compute_set_input(p_compute_graph, "y", mat_fake_lable);

        compute_graph_forward_propagation(p_compute_graph, "z");
        cg_node_t* p_first = p_compute_graph->p_nodes->p_top;

        // 将所有的训练参数全部都建立完梯度
        while (p_first != LIST_HEAD(p_compute_graph->p_nodes))
        {
            compute_node_t* p_node = p_first->ref;
            if (p_node->node_type == e_training_params) {
                compute_graph_build_gradient(p_compute_graph, "z", p_node->str_node_id);
            }
            p_first = p_first->prev;
        }

        // 将所有的训练参数使用梯度表更新一次。
        p_first = p_compute_graph->p_nodes->p_top;
        while(p_first != LIST_HEAD(p_compute_graph->p_nodes)) {
            compute_node_t* p_node = p_first->ref;
            if (p_node->node_type == e_training_params) {
                compute_update_training_params(p_compute_graph, p_node->str_node_id);
            }

            p_first = p_first->prev;
        }
    }
    Mat2_destroy(mat_fake_input);
    Mat2_destroy(mat_fake_lable);
    return 0;
}
int ann_cg_predict(matrix2_t* p_Input, compute_graph_t* p_compute_graph, matrix2_t* predict)
{
    printf("run compute_graph_forpropagation whit with node with id y_hat\n");
    return 0;
}
int ann_cg_recycle(compute_graph_t* p_graph)
{
   return compute_graph_recycle(p_graph);
}