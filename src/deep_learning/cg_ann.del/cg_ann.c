/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-06-11 11:11:57
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-06-17 14:50:28
 * @FilePath: /boring-code/src/deep_learning/cg_ann/cg_ann.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "deep_learning/compute_graph2/cg_list.h"
#include "deep_learning/compute_graph2/cg_debug.h"
#include "deep_learning/compute_graph2/cg_tensor.h"
#include "cg_ann_znode.h"
#include "cg_ann_opt.h"
#include "cg_ann.h"

static int __recycle_znode(cg_ref_t ref) 
{
    ann_znode_t* znode = (ann_znode_t*) ref;
    cg_ann_znode_recycle(znode);
    return 0;
}

static ann_znode_t* __do_build_linear_act(cg_ann_t* ann, int in_dimens, int out_dimens, cg_opt_base_t* act_opt, ann_znode_type_t out_type)
{
    ann_znode_t* ann_znode;
    cg_flow_push(ann, dot_opt(NULL));
    cg_flow_push(ann, empty_opt(NULL));
    cg_flow_push(ann, empty_opt(NULL));
    cg_flow_push(ann, cg_ann_znode_create(ann, out_dimens, in_dimens , e_weight));
    ann_znode = cg_flow_end(ann, cg_ann_znode_create(ann, 0, 0, e_auto));

    cg_flow_beg(ann);
    cg_flow_push(ann, ann_znode);
    cg_flow_push(ann, plus_opt(NULL));

    cg_flow_push(ann, cg_ann_znode_create(ann, out_dimens, 1, e_bais));
    
    if (act_opt) {
        ann_znode = cg_flow_end(ann, cg_ann_znode_create(ann, 0, 0, e_auto));
        cg_flow_beg(ann);
        cg_flow_push(ann, ann_znode);
        cg_flow_push(ann, act_opt);
    
        ann_znode = cg_flow_end(ann, cg_ann_znode_create(ann, 0, 0, out_type));

        cg_flow_beg(ann);
        cg_flow_push(ann, ann_znode);
    } else {
        ann_znode = cg_flow_end(ann, cg_ann_znode_create(ann, 0, 0, out_type));
        cg_flow_beg(ann);
        cg_flow_push(ann, ann_znode);
    }
    return ann_znode;
}

int cg_ann_init(
    cg_ann_t* ann, 
    int hl_size, 
    int* hl, 
    int batch_size, 
    int max_iter, 
    int x_dimens, 
    int y_dimens, 
    loss_type_t loss_type, 
    float learning_rate, 
    float espilon)
{
    cg_base_init(&ann->cg_base);
    ann->batch_size        = batch_size;
    ann->hidden_layer_size = hl_size;
    ann->x_dimens          = x_dimens;
    ann->y_dimens          = y_dimens;
    ann->loss_type     = loss_type;
    ann->learning_rate = learning_rate;
    ann->max_iter = max_iter;
    ann->epsilon  = espilon;
    ann->step     = 0;
    ann->hidden_layer = malloc(hl_size * sizeof(int));
    memcpy(ann->hidden_layer, hl, hl_size * sizeof(int));
    ann->znode_list = cg_list_create();
    cg_allocator_init(&ann->alloc);
    return 0;
}

int cg_ann_reset(cg_ann_t* ann) 
{
    cg_base_reset(&ann->cg_base);
    // 
    cg_list_recycle(ann->znode_list, __recycle_znode);
    ann->znode_list = NULL;

    free(ann->hidden_layer);
    ann->hidden_layer = NULL;
    cg_allocator_reset(&ann->alloc);
    return 0;
}

int cg_ann_reset_step(cg_ann_t* ann)
{
    ann->step = 0;
}

/**
 * @brief build compute graph
 * 
 * @param ann 
 * @param x_dimens 每条 x 数据的维度，例如 人（身高，体重，年龄，喜欢看动作片...)
 * @param y_dimens 每条 y 数据的维度，例如 男人（1， 0，0）、女人 （0，1，0）、人妖（0，0，1）
 * @param loss_type loss function 有 mse、corss entroy
 * @return int 
 */
int cg_ann_build_flow(cg_ann_t* ann)
{
    // push a start tag to start build compute flow
    ann_znode_t* znode;
    
    cg_flow_beg(ann);
    znode = (ann_znode_t*)cg_flow_push(ann, cg_ann_znode_create(ann, ann->batch_size, ann->x_dimens, e_x));
    ann->x_node = znode;
    
    int in_dimens = ann->x_dimens;

    for (int i=0; i<ann->hidden_layer_size; ++i) {
        __do_build_linear_act(ann, in_dimens, ann->hidden_layer[i], relu_opt(NULL), e_auto);
        in_dimens = ann->hidden_layer[i];
    }

    if (ann->loss_type == e_cross_entroy)
        znode = __do_build_linear_act(ann, in_dimens, ann->y_dimens, softmax_opt(NULL), e_y_hat);
    else 
        znode = __do_build_linear_act(ann, in_dimens, ann->y_dimens, NULL, e_y_hat);

    ann->y_hat_node = znode;

    if (ann->loss_type == e_cross_entroy) {

        cg_flow_push(ann, cross_entroy_opt(NULL));
        znode = (ann_znode_t*)cg_flow_push(ann, cg_ann_znode_create(ann, ann->batch_size, ann->y_dimens, e_y));
       
    } else  {
        cg_flow_push(ann, mse_opt(NULL));
        znode = (ann_znode_t*)cg_flow_push(ann, cg_ann_znode_create(ann, ann->batch_size, ann->y_dimens, e_y));
    }
    
    ann->y_node = znode;

    znode = (ann_znode_t*)cg_flow_end(ann, cg_ann_znode_create(ann, 0, 0, e_loss));

    ann->loss_node = znode;
    
    return 0;
}

int cg_ann_train(cg_ann_t* ann, cg_tensor_t* X_data, cg_tensor_t* Y_label) 
{
    if (TENSOR_DIMEN(X_data, 0) > ann->batch_size) {

        // load batch size 大小的数据到 e_x 节点中。
        srand(time(0));
        int iter = 0;
        //const int TRAIN_ITER = TENSOR_DIMEN(X_data, 0) / ann->batch_size; 
        
        while (iter++ < ann->max_iter)
        {
            // 1. 随机选从 X_data 中选取数据，存入 ann->x_node 和 ann->y_node 中，然后开始训练。
            for (int i=0; i<ann->batch_size; ++i) {

                int index = rand() % (TENSOR_DIMEN(X_data, 0) + 1);

                __sub_tensor_t sub_X_data  = cg_tensor_get_sub(X_data, 1, index);
                __sub_tensor_t sub_Y_label = cg_tensor_get_sub(Y_label, 1, index);

                __sub_tensor_t sub_X_input = cg_tensor_get_sub(ann->x_node->payload, 1, i);
                __sub_tensor_t sub_Y       = cg_tensor_get_sub(ann->y_node->payload, 1, i);
                

                if (cg_tensor_sub_to_sub(sub_X_input, sub_X_data) != 0) {
                    CG_DEBUG("load data from X_data to X_input FAIL at %d\n", i);
                    exit(1);
                }

                if (cg_tensor_sub_to_sub(sub_Y, sub_Y_label) != 0) {
                    CG_DEBUG("load data from Y_label to Y FAIL at %d\n", i);
                    exit(1);
                }
            }

            // do forward batch_size time 将 y_hat 的数据填满，最后计算 loss。丢复杂到嗨咁
            cg_base_reset_marker(ann);
            
            int i;
            for (i=0, ann->step=0; i<ann->batch_size; ++i, ++(ann->step)) {
                if (cg_do_forward((cg_base_t*)ann, (cg_znode_base_t*)ann->loss_node) != 0) {
                    CG_DEBUG("do forward propagation Error!");
                    exit(1);
                }
            }

            // printf the loss
            if (0 && ann->loss_node->payload) {
                float err = *((float*)ann->loss_node->payload->elems);
                if (err<ann->epsilon) break;
            }

            
            // 做一个变态的 
            cg_node_t* frist = CG_LIST_TOP(ann->znode_list);
            while (frist != CG_LIST_HEAD(ann->znode_list))
            {
                ann_znode_t* znode = (ann_znode_t*) frist->ref;

                if (znode->znode_type == e_weight || znode->znode_type == e_bais || znode->znode_type == e_auto) 
                {
                    // 需要对 weight、bais、中间节点做梯度计算。
                    if (cg_do_gradient((cg_base_t*)ann, (cg_znode_base_t*)ann->loss_node, (cg_znode_base_t*)znode) != 0) {
                        CG_DEBUG("do backward propagation Error!\n");
                        exit(1);
                    }

                    // 做完梯度更新，随便 update 了那个 payload 为下次 forwad 做准备
                    if (cg_tensor_scale(znode->gradient, ann->learning_rate) != 0) {
                        CG_DEBUG("scale gradient Error\n");
                        exit(1);
                    }

                    if (cg_tensor_sum(znode->payload, znode->payload, znode->gradient) != 0) {
                        CG_DEBUG("update payload Error\n");
                        exit(1);
                    }
                    
                }
                frist = frist->prev;
            }
        }

    } else {
        CG_DEBUG("size of X_data %d is less than ann batch %d\n", TENSOR_DIMEN(X_data,0), ann->batch_size);
        exit(1);
    }
    return 0;
}

int cg_ann_predict(cg_ann_t* ann, cg_tensor_t* input, cg_tensor_t* predict)
{
    if (cg_tensor_to_tensor(ann->x_node->payload, input) != 0) {
        CG_DEBUG("Error to load data!");
        exit(1);
    }
    
    cg_ann_reset_step(ann);
    
    if (cg_do_forward((cg_base_t*)ann, (cg_znode_base_t*)ann->y_hat_node) !=0) {
        CG_DEBUG("Error to do forward!!");
        exit(1);
    }

    cg_tensor_sub_to_tensor(predict, cg_tensor_get_sub(ann->y_hat_node->payload, 1, 0));

    return 0;
}

