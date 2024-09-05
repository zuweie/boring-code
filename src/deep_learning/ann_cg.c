#include "compute_graph/cg_list.h"
#include "ann_cg.h"


static int __bp_W_node(compute_node_t* p_thiz)
{
    // W 参数可能有多路向前传播的贡献，妈的一路的前向传播都复杂到嗨咁。
    // 找到每一路的前向传播，然后找到关于每一路前向传播的梯度值。
    

}

static int __update_payload_W_node(compute_node_t* p_thiz)
{

}


static int __bp_b_node(compute_node_t* p_thiz) 
{
    // 对于 b 型节点的求梯度，便是将上上一级节点的梯度拉回来，然后什么都不用做了。
}

static int __update_payload_b_node(compute_node_t* p_thiz) 
{

}

static int __fp_u_x_dot_w_node(compute_node_t* p_thiz)
{
    // 对于 X dot W 的中间节点的 fp 操作就是 X dot b
}
static int __bp_u_x_dot_w_node(compute_node_t* p_thiz) 
{

}

static int __fp_u_plus_b_node(compute_node_t* p_thiz)
{

}
static int __bp_u_plus_b_node(compute_node_t* p_thiz)
{

}

static int __fp_u_act_input_node(comptue_node_t* p_thiz) 
{

}

static int __bp_u_act_input_node(compute_node_t* p_thiz) 
{

}

static int __fp_u_act_y_hat_node(compute_node_t* p_thiz)
{

}

static int __bp_u_act_y_hat_node(compute_node_t* p_thiz)
{

}
static int __fp_output_node(compute_node_t* p_thiz)
{

}

static int __bp_output_node(compute_node_t* p_thiz) 
{

}

static int __recycle_note(compute_node_t* p_thiz) 
{
    Mat2_destroy(p_thiz->p_payload);
    Mat2_destroy(p_thiz->p_gradient);
    while ((cg_ref_t* p = cg_list_pop(p_thiz->p_gradient_paths)) != NULL) {
        cg_list_recycle(p, NULL);
    }
    cg_list_recycle(p_thiz->p_gradient_paths, NULL);
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
    matrix2_t* mat_W = Mat2_create();
    matrix2_t* mat_b = Mat2_create();
    // 开始建图
    p_X = compute_create_input_node("X", &__recycle_note);
    compute_graph_add_note(p_thiz, p_X);
    cg_list_push(p_node_stack, p_X);

    for(int i=1; i<cg_params->layers_size; ++i) {

        rows_W = cg_params->layers_dimension[i];
        cols_W = cg_params->layers_dimension[i-1];
        rows_b = cg_params->layers_dimension[i];

        Mat2_reshape(mat_W, rows_W, cols_b);
        Mat2_fill_random(mat_W, 0, 1);

        Mat2_reshape(mat_b, rows_b, cols_b);
        Mat2_fill_random(mat_b, 0, 1);
        // build fuck node
        sprintf(str_node_id_buff, "W_%d", W_i);
        p_W = compute_create_training_params_node(str_node_id_buff, mat_initialization, &__recycle_note, &__bp_W_node, &__update_payload_W_node);
        
        // 将 W 节点加到计算图中去
        compute_graph_add_note(p_thiz, p_W);
        cg_list_push(p_node_stack, p_W);

        // 生成 u = W dot x 的中间节点
        sprintf(str_node_id_buff, "U_%d", u_i++);
        p_u = compute_create_mediate_node(str_node_id_buff, &__recycle_note, &__fp_u_x_dot_w_node, &__bp_u_x_dot_w_node);
        // 将 u 加入到 计算图中去。
        compute_graph_add_note(p_thiz, p_u);

        while ((compute_node_t* p = cg_list_pop(p_node_stack)) != NULL)
        {
           // 把 X 节点和 W 节点连到 u 节点上去。
           compute_graph_link_note(p_thiz, p, p_u);
        }
        cg_list_push(p_u);
        
        // 生成 b 节点
        sptrinf(str_node_id_buff, "b_%d", b_i++);
        p_b = compute_create_training_params_node(str_node_id_buff, mat_b, &__recycle_note, &__bp_b_node, &__update_payload_b_node);
        // 将节点加载到计算图中
        compute_graph_add_note(p_thiz, p_b);
        cg_list_push(p_node_stack, p_b);

        // 生成 u = W dot x + b
        sptrinf(str_node_id_buff, "u_%d", u_i++);
        p_u = compute_create_mediate_node(str_node_id_buff, &__recycle_note, &__fp_u_plus_b_node, &__bp_u_plus_b_node);
        compute_graph_add_node(p_thiz, p_u);

        while ((compute_node_t* p = cg_list_pop(p_node_stack)) != NULL)
        {
            // 将加 b 节点连到 u 上面去。
            compute_graph_link_note(p_thiz, p, p_u);
        }
        cg_list_push(p_node_stack, p_u);
        
        // 
        sprintf(str_node_id_buff, "u_%d", u_i++);
        if (i == layers_size-1) {
            // 
            p_u = compute_create_mediate_node(str_node_id_buff, &__recycle_note, &__fp_u_act_y_hat_node, & __bp_u_act_y_hat_node);
        } else {
            p_u = compute_create_mediate_node(str_node_id_buff, &__recycle_note, &__fp_u_act_input_node, &__bp_u_act_input_node);

        }
        compute_graph_add_note(p_u);
        
        while((compute_node_t* p = cg_list_pop(p_node_stack) != NULL) {
            // 把上面的 u = W dot x + b 节点连到这个激活节点上去
            compute_graph_link_note(p_thiz, p, p_u);
        }

        cg_list_push(p_node_stack, p_u);
    }

    sprintf(str_node_id_buff, "y");
    p_y = compute_create_input_node(str_node_id_buff, &__recycle_note);
    compute_graph_add_note(p_y);

    sprintf(string_node_id_buff, "z");
    p_z = compute_create_mediate_node(str_node_id_buff, &__recycle_note, &__fp_output_node, &__bp_output_node);
    compute_graph_add_note(p_z);

    while ((compute_node_t *p = cg_list_pop(p_node_stack)) != NULL)
    {
        // 把 u = act(W dot x + b) 和 y 连接到这个输出节点上去。
        compute_graph_link_note(p_thiz, p, p_z);
    }
    
    cg_list_recycle(p_node_stack, NULL);
    return 0;
}

int ann_cg_train(matrix2_t* p_data, matrix2_t* p_label, ann_cg_params_t* p_params, compute_graph_t* p_compute_graph_t)
{

}
int ann_cg_predict(matrix2_t* p_Input, compute_graph_t* p_compute_graph, matrix2_t* predict);
int ann_recycle();