/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-03-14 11:35:50
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-08-23 00:08:48
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
#include "deep_learning/compute_graph2/cg_calculate_flow.h"
#include "deep_learning/compute_graph2/cg_operand.h"
#include "deep_learning/compute_graph2/cg_operator.h"

#include "deep_learning/nn_library/nn_operator_linear.h"
#include "deep_learning/nn_library/nn_operator_relu.h"
#include "deep_learning/nn_library/nn_operator_softmax.h"
#include "deep_learning/nn_library/nn_operator_mse.h"
#include "deep_learning/nn_library/nn_operator_crossentropy.h"
#include "deep_learning/nn_library/nn_utility.h"

#include "dnn.h"

static int __recycle_node(cg_node_t* node)
{
    if (CG_NODE_IS_OPERAND(node)) {
        cg_operand_reset(node);
    } else if (CG_NODE_IS_OPERATOR(node)) {
        cg_operator_reset(node);
    } else {
        CG_DEBUG("Error <%d@%s>: unknown CG NODE TYPE\n", __LINE__, __FILE__);
    }
    free(node);
    return 0;
}

int dnn_init(dnn_t* nn)
{
    *ann = (ann_t) {
        .node_count   = 0,
        .recycle_list = cg_list_create(),
        .build_stack  = cg_list_create()
    };
    cg_allocator_init(&ann->alloc);
    
    return 0;
}

int dnn_reset(dnn_t* nn)
{
    cg_allocator_reset(&nn->alloc);
    cg_list_recycle(nn->recycle_list, __recycle_node);
    cg_list_recycle(nn->build_stack, NULL);
    return 0;
}

int dnn_linear(dnn_t* nn, int in_dimens, int out_dimens)
{
    linear_opt_t* l = linear(&nn->alloc, nn->build_stack, &nn->node_count, in_dimens, out_dimens, nn->recycle_list);
    if (l && !nn->_Input) {
        nn->_Input = l->_Input;
    }
    // 这个有可能是输出层,先把它的输出 markdown
    if (l) nn->_Output = (nn_operand_t*)(CG_LIST_TOP(nn->build_stack)->ref);
    return !l;
}

int dnn_relu(dnn_t* nn)
{
    void* ret = relu(&nn->alloc, nn->build_stack, &nn->node_count, nn->recyc_list);
    return !ret;
}

int dnn_mse(dnn_t* nn)
{
    void* ret = mse(&nn->alloc, nn->build_stack, &nn->node_count, nn->recycle_list);
    if (ret) {
        if (!nn->_Loss) {
            nn->_Loss = (nn_operand_t*) (CG_LIST_TOP(nn->build_stack)->ref);
        } else {
            CG_DEBUG("Error <%d@%s>: loss has been set, SHOULD be set again \n", __LINE__, __FILE__);
            return -1;
        }
    }
    return !ret;
}

int dnn_crx(dnn_t* nn)
{
   void* ret = crossentropy(&nn->alloc, nn->build_stack, nn->node_count, nn->recycle_list);
   if (ret) {
        if (!nn->_Loss) {
            nn->_Loss = (nn_operand_t*) (CG_LIST_TOP(nn->build_stack)->ref);
        } else {
            CG_DEBUG("Error <%d@%s>: loss has been set, SHOULD be set again \n", __LINE__, __FILE__);
            return 0;
        }
   }
   return !ret;
}

int dnn_softmax(dnn_t* nn)
{
    void* ret = softmax(&nn->alloc, nn->build_stack, &nn->node_count, nn->recycle_list)
    return !ret;
}


int dnn_fit(dnn_t* nn, dnn_optimizer_t* optimizer)
{

    int term = 1;
    do {
        optimizer->prepare(nn);
        cg_calculate(nn->_Loss);
        if (optimizer->term(nn)) {
            // do backward propagetion continue to fit
            cg_derivative(nn->_Loss);
        }
        optimizer->processing(nn);
    } while (!term);
    return 0;
}

cg_tensor_t* dnn_predict(dnn_t* nn, cg_tensor_t* feature)
{

    return 0;
}