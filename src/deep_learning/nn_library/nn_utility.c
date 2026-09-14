/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-08-22 10:01:59
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-09-13 12:37:05
 * @FilePath: /boring-code/src/deep_learning/nn_library/nn_utility.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>
#include "deep_learning/compute_graph2/cg_allocator.h"
#include "deep_learning/compute_graph2/cg_node.h"
#include "deep_learning/compute_graph2/cg_graph.h"
#include "deep_learning/compute_graph2/cg_tensor_shape.h"
#include "deep_learning/compute_graph2/cg_list.h"
#include "deep_learning/compute_graph2/cg_tensor.h"
#include "nn_operand.h"
#include "nn_operator_linear.h"
#include "nn_operator_relu.h"
#include "nn_operator_softmax.h"
#include "nn_operator_mse.h"
#include "nn_operator_crossentropy.h"
#include "nn_operator_sqrsum.h"
#include "nn_operator_sum.h"
#include "nn_utility.h"

static char cg_node_naming[64];
static char cg_node_type[64];

static inline char* __gen_node_id(int cg_node_count, const char* prefix)
{
    snprintf(cg_node_naming, 64, "%s@%d", prefix, cg_node_count);
    return cg_node_naming;
}


char* node_typeof(cg_node_t* node)
{
    if (sscanf(CG_NODE_ID(node), "%[^@]",  cg_node_type) == 1) {
        return cg_node_type;
    } else {
        CG_DEBUG("Error <%d@%s>: cg_node`s id(%s) format is wrong\n", __LINE__, __FILE__, CG_NODE_ID(node));
        return "undefined!";
    }
}

char* node_is(cg_node_t* node, const char* type)
{
    return strstr(CG_NODE_ID(node), type) != NULL;
}

nn_operand_t* node_group_find(nn_operand_t* z, const char* node_type)
{
    cg_operator_t* opt = cg_operand_get_producer(z);
    if (opt) {

        cg_list_node_t* first = CG_LIST_TOP( CG_NODE_IN(opt) );

        while (first != CG_LIST_HEAD( CG_NODE_IN(opt) ) ) {

            if (node_is(first->ref, node_type)) return first->ref;
            first = first->prev;
        }
    }
    return NULL;
}

// 线性运算
int nn_linear(cg_allocator_t* alloc, cg_list_t* build_stack, int *node_count, int in_dimens, int out_dimens, cg_list_t* nodes_list)
{
    
    nn_operand_t* _Input = cg_list_pop(build_stack);
    if (!_Input) {
        // 新建 _Input 默认是 in_dimens 个属性， 1 条记录。
        _Input = nn_operand_create(alloc, __gen_node_id(++(*node_count), "x"), in_dimens, 1);
        cg_list_push(nodes_list, _Input);
    } else if (!node_is(_Input, "x")) {
        CG_DEBUG("Error <%d@%s>: pop up node is a \'%s\', not a \'x\'\n", __LINE__, __FILE__, node_typeof(_Input));
        return NULL;
    } else if (SHAPE_DIMENS(_Input->x->shape, 0) != in_dimens) {
        CG_DEBUG("Error <%d@%s>: in_dimens(%d) does not match _Input rows(%d)\n", __LINE__, __FILE__, in_dimens,SHAPE_DIMENS(_Input->x->shape, 0));
        return NULL;
    }
    // every thing is OK keep going
    nn_operand_t* W = nn_operand_create(alloc, __gen_node_id(++(*node_count), "W"), out_dimens, in_dimens);
    nn_operand_t* b = nn_operand_create(alloc, __gen_node_id(++(*node_count), "b"), out_dimens, 1);
    nn_operand_t* z = nn_operand_create(alloc, __gen_node_id(++(*node_count), "x"), out_dimens, 1);
    linear_opt_t* linear_opt = linear_opt_create(__gen_node_id(++(*node_count), "linear_opt"),  _Input, W, b);

    // 做物理连接。
    cg_graph_link(_Input, linear_opt);
    cg_graph_link(W, linear_opt);
    cg_graph_link(b, linear_opt);
    cg_graph_link(linear_opt, z);

    cg_list_push(build_stack, W);
    cg_list_push(build_stack, z);

    cg_list_push(nodes_list, W);
    cg_list_push(nodes_list, b);
    cg_list_push(nodes_list, z);
    cg_list_push(nodes_list, linear_opt);
    
    return 0;
}

// relu 激活
int nn_relu(cg_allocator_t* alloc, cg_list_t* build_stack, int* node_count, cg_list_t* nodes_list)
{
    nn_operand_t* _Input = cg_list_pop(build_stack);

    if (_Input && node_is(_Input, "x")) {
        // ok create the relu
        // z 的大小与 _Input 一样大。
        nn_operand_t* z  = nn_operand_create(
            alloc, 
            __gen_node_id(++(*node_count), "x"), 
            SHAPE_DIMENS(_Input->x->shape, 0), 
            SHAPE_DIMENS(_Input->x->shape, 1)
        );

        relu_opt_t* relu = relu_opt_create(__gen_node_id(++(*node_count), "relu_opt"), _Input);
        
        cg_graph_link(_Input, relu);
        cg_graph_link(relu, z);

        cg_list_push(build_stack, z);

        cg_list_push(nodes_list, z);
        cg_list_push(nodes_list, relu);

        return 0;

    } else if (_Input) {
        // error _Input is not a "X"
        CG_DEBUG("Error: <%d@%s>: _Input(%s) is not a \'x\'\n", __LINE__, __FILE__, CG_NODE_ID(_Input));
        return -1;
    } else {
        // error _Input is NULL
        CG_DEBUG("Error: <%d@%s>: _Input is NULL\n", __LINE__, __FILE__);
        return -1;
    }
    return 0;
}

// softmax
int nn_softmax(cg_allocator_t* alloc, cg_list_t* build_stack, int* node_count, cg_list_t* nodes_list)
{
    nn_operand_t* _Input = cg_list_pop(build_stack);
    if (_Input && node_is(_Input, "x")) {
        nn_operand_t* z = nn_operand_create(
            alloc, 
            __gen_node_id(++(*node_count), "out"), 
            SHAPE_DIMENS(_Input->x->shape, 0),
            SHAPE_DIMENS(_Input->x->shape, 1)
        );
        softmax_opt_t* softmax = softmax_opt_create(__gen_node_id(++(*node_count), "softmax_opt"), _Input);
        cg_graph_link(_Input, softmax);
        cg_graph_link(softmax, z);
        
        cg_list_push(build_stack, z);

        cg_list_push(nodes_list, z);
        cg_list_push(nodes_list, softmax);

        return 0;

    } else if (_Input) {
        CG_DEBUG("Error: <%d@%s>: _Input(%s) is not a \'x\'\n", __LINE__, __FILE__, CG_NODE_ID(_Input));
        return -1;
    } else {
        CG_DEBUG("Error: <%d@%s>: _Input is NULL\n", __LINE__, __FILE__);
        return -1;
    }
    return -1;
}

// most square error
int nn_mse(cg_allocator_t* alloc, cg_list_t* build_stack, int* node_count, cg_list_t* nodes_list)
{
    nn_operand_t* _Input = cg_list_pop(build_stack);
    if (_Input && node_is(_Input, "x")) {


        nn_operand_t* labls  = nn_operand_create(alloc, __gen_node_id(++(*node_count), "labels"), SHAPE_DIMENS(_Input->x->shape, 0), 1);
        nn_operand_t* _Loss  = nn_operand_create(alloc, __gen_node_id(++(*node_count), "loss"), 1, 1);
        mse_opt_t*    mse    = mse_opt_create(__gen_node_id(++(*node_count), "mse_opt"), _Input, labls);

        cg_graph_link(_Input, mse);
        cg_graph_link(labls, mse);
        cg_graph_link(mse, _Loss);
        
        cg_list_push(build_stack, _Loss);

        cg_list_push(nodes_list, labls);
        cg_list_push(nodes_list, _Loss);
        cg_list_push(nodes_list, mse);
        
        return 0;

    } else if (_Input) {
        CG_DEBUG("Error: <%d@%s>: _Input(%s) is not a \'x\'\n", __LINE__, __FILE__, CG_NODE_ID(_Input));
        return -1;
    } else{
        CG_DEBUG("Error: <%d@%s>: _Input is NULL\n", __LINE__, __FILE__);
        return -1;
    }
    return -1;
}

// corss entropy
int nn_crossentropy(cg_allocator_t* alloc, cg_list_t* build_stack, int* node_count, cg_list_t* nodes_list)
{
    nn_operand_t* _Input = cg_list_pop(build_stack);
    if (_Input && node_is(_Input, "x")) {

        nn_operand_t* labls  = nn_operand_create(alloc, __gen_node_id(++(*node_count), "labels"), SHAPE_DIMENS(_Input->x->shape, 0), 1);
        nn_operand_t* _Loss  = nn_operand_create(alloc, __gen_node_id(++(*node_count), "loss"), 1, 1);
        crossentropy_opt_t* crx = crossentropy_opt_create(__gen_node_id(++(*node_count), "corssentropy_opt"), _Input, labls);

        cg_graph_link(_Input, crx);
        cg_graph_link(labls, crx);
        cg_graph_link(crx, _Loss);
        
        cg_list_push(build_stack, _Loss);

        cg_list_push(nodes_list, labls);
        cg_list_push(nodes_list, _Loss);
        cg_list_push(nodes_list, crx);
        
        return 0;

    } else if (_Input) {
        CG_DEBUG("Error: <%d@%s>: _Input(%s) is not a \'x\'\n", __LINE__, __FILE__, CG_NODE_ID(_Input));
        return -1;
    } else{
        CG_DEBUG("Error: <%d@%s>: _Input is NULL\n", __LINE__, __FILE__);
        return -1;
    }
    return -1;
}



int nn_regular(cg_allocate_t* alloc, cg_list_t* build_stack, int* node_count, cg_list_t* node_list, float lamada) 
{
    // TODO 1: check all item in build stack is OK
    
    int all_is_W  = 1;
    int have_loss = 0;

    // check it first
    cg_list_node_t* first = CG_LIST_TOP(build_stack);
    while (first != CG_LIST_HEAD(build_stack)) {
        if (node_is(first->ref, "loss")) {
            have_loss = 1;
        } else if (!node_is(first->ref, "W")) {
            all_is_W = 0;
            break;
        }
        first = first->prev;
    }

    if (!have_loss) {
        CG_DEBUG("ERROR <%d@%s>: does not have loss\n", __LINE__, __FILE__);
        return -1;
    }
    if (!all_is_W) {
        CG_DEBUG("ERROR <%d@%s>: not all cg_node is W\n", __LINE__, __FILE__);
        return -1;
    }

    if (all_is_W && have_loss) {
        nn_operand_t* _Input = NULL;
        nn_operand_t* _Loss  = nn_operand_create(alloc, __gen_node_id(++(*node_count), "loss"), 1, 1);
        sum_opt_t* sum_opt   = sum_opt_create(__gen_node_id(++(*node_count), "sum_opt"));

        cg_graph_link(sum_opt, _Loss);

        cg_list_push(node_list, _Loss);
        cg_list_push(node_list, sum_opt);

        while ( _Input = cg_list_pop(build_stack) ) {
            
            if (node_is(_Input, "W")) {
                // TODO: create a WW2
                nn_operand_t* z      = nn_operand_create(alloc, __gen_node_id(++(*node_count), "ww2"), 1, 1);
                sqrsum_opt_t* sqrsum = sqrsum_opt_create(__gen_node_id(++(*node_count), "sqrsum_opt"), _Input, lamada);
                
                cg_graph_link(_Input, sqrsum);
                cg_graph_link(sqrsum, z);
                sum_opt_add_variant(sum_opt, z);
                cg_graph_link(z, sum_opt);

                cg_list_push(node_list, z);
                cg_list_push(node_list, sqrsum);
            } else {
                // is a loss
                sum_opt_add_variant(sum_opt, _Input);
                cg_graph_link(_Input, sum_opt);
            }
        }
        cg_list_push(build_stack, _Loss);
        return 0;
    }
    return -1;
}
