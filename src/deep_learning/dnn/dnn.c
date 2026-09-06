/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-03-14 11:35:50
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-09-05 17:46:14
 * @FilePath: /boring-code/src/deep_learning/nn/nn.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdlib.h>
#include <stdio.h>
#include <float.h>

#include "deep_learning/compute_graph2/cg_allocator.h"
#include "deep_learning/compute_graph2/cg_node.h"
#include "deep_learning/compute_graph2/cg_list.h"
#include "deep_learning/compute_graph2/cg_tensor.h"
#include "deep_learning/compute_graph2/cg_graph.h"
#include "deep_learning/compute_graph2/cg_tensor_shape.h"
#include "deep_learning/compute_graph2/cg_debug.h"
#include "deep_learning/compute_graph2/cg_flow.h"
#include "deep_learning/compute_graph2/cg_operand.h"
#include "deep_learning/compute_graph2/cg_operator.h"

#include "deep_learning/nn_library/nn_operator_linear.h"
#include "deep_learning/nn_library/nn_operator_relu.h"
#include "deep_learning/nn_library/nn_operator_softmax.h"
#include "deep_learning/nn_library/nn_operator_mse.h"
#include "deep_learning/nn_library/nn_operator_crossentropy.h"
#include "deep_learning/nn_library/nn_utility.h"

#include "dnn_optimizer.h"
#include "dnn.h"


static int __recycle_node(cg_node_t* node)
{
    if (CG_NODE_IS_OPERAND(node)) {
        cg_operand_reset(node);
        CG_DEBUG("INFO: released operand(%s)\n", CG_NODE_ID(node));
    } else if (CG_NODE_IS_OPERATOR(node)) {
        cg_operator_reset(node);
        CG_DEBUG("INFO: released operator(%s)\n", CG_NODE_ID(node));
    } else {
        CG_DEBUG("Error <%d@%s>: unknown CG NODE TYPE\n", __LINE__, __FILE__);
    }
    free(node);
    return 0;
}

int dnn_init(dnn_t* nn)
{
    *nn = (dnn_t) {
        .node_count  = 0,
        .nodes_list  = cg_list_create(),
        .build_stack = cg_list_create()
    };
    cg_allocator_init(&nn->alloc);
    
    return 0;
}

int dnn_reset(dnn_t* nn)
{
    cg_allocator_reset(&nn->alloc);
    cg_list_recycle(nn->nodes_list, __recycle_node);
    cg_list_recycle(nn->build_stack, NULL);
    return 0;
}

int dnn_linear(dnn_t* nn, int in_dimens, int out_dimens)
{
    linear_opt_t* l = nn_linear(&nn->alloc, nn->build_stack, &nn->node_count, in_dimens, out_dimens, nn->nodes_list);
    if (l && !nn->_Input) {
        nn->_Input = l->x;
    }
    // 这个也有可能是输出层,先把它的输出 markdown
    if (l) nn->_Output = (nn_operand_t*)(CG_LIST_TOP(nn->build_stack)->ref);
    return !l;
}

int dnn_relu(dnn_t* nn)
{
    void* ret = nn_relu(&nn->alloc, nn->build_stack, &nn->node_count, nn->nodes_list);
    if (!ret) CG_DEBUG("Error <%d@%s>: relu faild\n", __LINE__, __FILE__);
    return !ret;
}

int dnn_mse(dnn_t* nn)
{
    void* ret = nn_mse(&nn->alloc, nn->build_stack, &nn->node_count, nn->nodes_list);
    if (ret) {
        if (!nn->_Loss) {
            nn->_Loss = (nn_operand_t*)cg_list_pop(nn->build_stack);
        } else {
            CG_DEBUG("Error <%d@%s>: loss has been set, SHOULD be set again \n", __LINE__, __FILE__);
            return -1;
        }
    } else {
        CG_DEBUG("Error <%d@%s>: mse faild\n", __LINE__, __FILE__);
    }
    return !ret;
}

int dnn_softcrx(dnn_t* nn)
{
    void* top_operand = CG_LIST_TOP(nn->build_stack)->ref;
    if (node_is(top_operand, "x") && nn->_Loss == NULL) {

        // 这里需要做双头，一个头是 softmax 给 output， 一个头是 crxentropy 给 Loss, 所以这里多复制一个 x 节点给两个头用
        cg_list_push(nn->build_stack, top_operand);

        // TODO 1: softmax
        void* soft = nn_softmax(&nn->alloc, nn->build_stack, &nn->node_count, nn->nodes_list);
        if (soft) {
            nn->_Output = (nn_operand_t*) cg_list_pop(nn->build_stack);
        } else {
            CG_DEBUG("Error <%d@%s>: softmax faild\n", __LINE__, __FILE__);
            return !soft;
        }
        void* crx = nn_crossentropy(&nn->alloc, nn->build_stack, &nn->node_count, nn->nodes_list);
        if (crx) {
            nn->_Loss = (nn_operand_t*) cg_list_pop(nn->build_stack);
            return !crx;
        } else {
            CG_DEBUG("Error <%d@%s>: crx faild\n", __LINE__, __FILE__);
            return !crx;
        }
    } else if (nn->_Loss){
        CG_DEBUG("Error <%d@%s>: nn`s loss has been set, SHOULD be set again \n", __LINE__, __FILE__);
        return -1;
    }
    CG_DEBUG("Error <%d@%s>: Input(%s) is no a \'x\'\n", __LINE__, __FILE__, CG_NODE_ID(top_operand));
    return -1;
}

int dnn_softmax(dnn_t* nn)
{
    void* ret = nn_softmax(&nn->alloc, nn->build_stack, &nn->node_count, nn->nodes_list);
    if (ret) nn->_Output = (nn_operand_t*)cg_list_pop(nn->build_stack);
    else CG_DEBUG("Error <%d@%s>: softmax faild!\n", __LINE__, __FILE__);
    return !ret;
}


int dnn_fit(dnn_optimizer_t* optimizer)
{

    int term = 1;
    do {
        optimizer->prepare(optimizer);
        cg_calculate(optimizer->model->_Loss);
        if (!(term = optimizer->term(optimizer))) {
            // do backward propagetion continue to fit
            cg_derivative(optimizer->model->_Loss);
            optimizer->step(optimizer);
        }
        optimizer->processing(optimizer);
    } while (!term);
    return 0;
}

cg_tensor_t* dnn_predict(dnn_t* nn, cg_tensor_t* feature)
{
    // 输入特征
    cg_tensor_cpy_to(nn->_Input->x, feature);
    // 计算预计
    cg_calculate(nn->_Output);
    // 返回结果
    return nn->_Output->x;
}