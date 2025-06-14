/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-06-11 11:11:57
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-06-14 10:15:01
 * @FilePath: /boring-code/src/deep_learning/cg_ann/cg_ann.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdlib.h>
#include <time.h>
#include "deep_learning/compute_graph2/cg_debug.h"
#include "deep_learning/compute_graph2/cg_tensor.h"
#include "deep_learning/compute_graph2/cg_znode.h"
#include "cg_ann_znode.h"
#include "cg_ann_opt.h"
#include "cg_ann.h"

static int __recycle_znode(cg_ref_t ref) 
{
    ann_znode_t* znode = (ann_znode_t*) ref;
    cg_ann_znode_recycle(znode);
    return 0;
}

static cg_znode_t* __build_y_hat(cg_ann_t* ann, int in_dimens, int out_dimens, cg_opt_base_t* act_opt, int batch_size) 
{

}

static cg_znode_t* __build_linear(cg_ann_t* ann, int in_dimens, int out_dimens, cg_opt_base_t* act_opt)
{
    ann_znode_t* ann_znode;
    cg_flow_push(ann, dot_opt(NULL));
    cg_flow_push(ann, cg_ann_znode_create(ann, out_dimens, in_dimens , e_weight));
    ann_znode = cg_flow_end(ann, cg_ann_znode_create(ann, 0, 0, e_auto));

    cg_flow_beg(ann);
    cg_flow_push(ann, ann_znode);
    cg_flow_push(ann, plus_opt(NULL));

    cg_flow_push(ann, cg_ann_znode_create(ann, out_dimens, 1, e_bais));
    if (act_opt)
        ann_znode = cg_flow_end(ann, cg_ann_znode_create(ann, 0, 0, e_auto));
    else 
        ann_znode = cg_flow_end(ann, cg_ann_znode_create(ann, batch_size,out_dimens, znode_type))
    
    if (act_opt) {
        // 如果有激活函数，则再多加一个计算节点。
        cg_flow_beg(ann);
        cg_flow_push(ann, ann_znode);
        cg_flow_push(act_opt);
        ann_znode = cg_flow_end(ann, cg_ann_znode_create(ann, 0, 0, e_auto));

        cg_flow_push(ann, flow_beg());
        cg_flow_push(ann, ann_znode);

    } else {
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
    cg_init(&ann->compute_graph);
    ann->batch_size   = batch_size;
    ann->hidden_layer_size = hl_size;
    ann->x_dimens = x_dimens;
    ann->y_dimens = y_dimens;
    ann->loss_type = loss_type;
    ann->learning_rate     = learning_rate;
    ann->max_iter = max_iter;
    ann->epsilon  = espilon;
    ann->step     = 0;

    ann->hidden_layer = malloc(hl_size * sizeof(int));
    memcpy(ann->hidden_layer, hl, hl_size * sizeof(int));

    return 0;
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
int cg_ann_build_calculate_flow(cg_ann_t* ann)
{
    // push a start tag to start build compute flow
    cg_flow_beg(ann);
    cg_flow_push(ann, cg_ann_znode_create(ann, ann->batch_size, ann->x_dimens, e_x));

    int in_dimens = ann->x_dimens;

    for (int i=0; i<ann->hidden_layer_size; ++i) {
        __build_linear(ann, in_dimens, ann->hidden_layer[i], relu_opt);
        in_dimens = ann->hidden_layer[i];
    }

    if (ann->loss_type == e_cross_entroy)
        znode = __build_linear(ann, in_dimens, ann->y_dimens, softmax_opt, e_y_hat)
    else 
        znode = __build_linear(ann, in_dimens, ann->y_dimens, NULL, e_y_hat);
    
    znode->payload = cg_tensor_create(ann->compute_graph.znode_alloc, 2, ann->batch_size, ann->y_dimens);
    znode->gradient = cg_tensor_create(ann->compute_graph.znode_alloc, 2, ann->batch_size, ann->y_dimens);
    ann->y_hat_node = znode;


    if (ann->loss_type == e_cross_entroy) {

        cg_flow_push(ann, cross_entroy_opt(NULL));
        znode = cg_flow_push(ann, cg_znode_create(ann, e_y));
       
    } else  {
        cg_flow_push(ann, mse_opt(NULL));
        znode = cg_flow_push(ann, cg_znode_create(ann, e_y));
    }
    znode->payload = cg_tensor_create(ann->compute_graph.znode_alloc, 2, ann->batch_size, ann->y_dimens);
    ann->y_node = znode;

    cg_flow_push(ann, flow_end());

    znode = cg_comb_zonde(ann, e_loss);
    ann->loss_node = znode;

    return 0;
}

int cg_ann_train(cg_ann_t* ann, cg_tensor_t* X_data, cg_tensor_t* Y_label) 
{
    if (TENSOR_DIMEN(X_data, 0) > ann->batch_size) {

        // load batch size 大小的数据到 e_x 节点中。
        srand(time(0));
        int coord[1];
        int epoch = 0;
        while (epoch < ann->max_epoch)
        {
            // 1. 加载一个 batch 的数据进入 Xinput。
            for (int i=0; i<ann->batch_size; ++i) {
                coord[0] = rand() % (TENSOR_DIMEN(X_data, 0) + 1);
                __sub_tensor_t sub_X_data  = cg_tensor_get_sub(X_data, 1, coord);
                __sub_tensor_t sub_Y_label = cg_tensor_get_sub(Y_label, 1, coord);
                coord[0] = i;
                __sub_tensor_t sub_X_input = cg_tensor_get_sub(ann->x_node->payload, 1, coord);
                __sub_tensor_t sub_Y       = cg_tensor_get_sub(ann->y_node->payload, 1, coord);
                

                if (cg_tensor_sub_to_sub(&sub_X_input, &sub_X_data) != 0) {
                    CG_DEBUG("load data from X_data to X_input fail at %d\n", i);
                    exit(1);
                }

                if (cg_tensor_sub_to_sub(&sub_y_label, &sub_y) != 0) {
                    CG_DEBUG("load data from Y_label to Y fail at %d\n", i);
                    exit(1);
                }
            }

            // 将
            for (int i=0; ann->step=0; i<ann->batch_size; ++i, ++ann->setp) {
                cg_forward_propagation(&ann->compute_graph);
            }

            
            
        }

    } else {
        CG_DEBUG("size of X_data %d is less than ann batch %d\n", TENSOR_DIMEN(X_data,i), ann->batch_size);
        exit(1);
    }
}

int cg_ann_predict(cg_ann_t* ann, cg_tensor_t* input, cg_tensor_t* predict)
{

}

int cg_ann_reset(cg_ann_t* ann) 
{
    cg_base_reset(&ann->cg_base);
    // 
    cg_list_recycle(ann->znode_list, __recycle_znode);
    ann->znode_list = NULL;

    free(ann->hidden_layer);
    ann->hidden_layer = NULL;
   
    return 0;
}