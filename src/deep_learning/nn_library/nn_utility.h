/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-08-22 10:01:50
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-11-22 19:41:41
 * @FilePath: /boring-code/src/deep_learning/nn_library/nn_utility.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __NN_BUILD_H__
#define __NN_BUILD_H__

typedef struct cg_allocate cg_allocate_t;
typedef struct cg_list cg_list_t;
typedef struct cg_node cg_node_t;
typedef struct nn_operand nn_operand_t;

// 获取 node 的类型
char* node_typeof(cg_node_t* node);
// 判断 node 是不是某种类型
char* node_is(cg_node_t* node, const char* type);

nn_operand_t* node_group_find(nn_operand_t* z, const char* node_type);

// 线性运算
int nn_linear(cg_allocator_t* alloc, cg_list_t* build_stack, int in_dimens, int out_dimens, cg_list_t* node_list);
// relu 激活
int nn_relu(cg_allocator_t* alloc, cg_list_t* build_stack, cg_list_t* nodes_list);
// softmax
int nn_softmax(cg_allocator_t* alloc, cg_list_t* build_stack, cg_list_t* nodes_list);
// most square error
int nn_mse(cg_allocator_t* alloc, cg_list_t* build_stack, cg_list_t* nodes_list);
// corss entropy
int nn_crossentropy(cg_allocator_t* alloc, cg_list_t* build_stack, cg_list_t* nodes_list);
// regular
int nn_regular(cg_allocate_t* alloc, cg_list_t* build_stack, cg_list_t* nodes_list, float lamada);
// embedding
int nn_embdeeing(cg_allocate_t* alloc, cg_list_t* build_stack, cg_list_t* nodes_list);
// positional encodeing
int nn_positional_encoding(cg_allocate_t* alloc, cg_list_t* build_stack, cg_list_t* nodes_list);
// multi head attention
int nn_multi_head_attention(cg_allocate_t* alloc, cg_list_t* build_stack, cg_list_t* nodes_list);
#endif