/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-08-22 10:01:50
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-08-22 23:26:59
 * @FilePath: /boring-code/src/deep_learning/nn_library/nn_utility.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __NN_BUILD_H__
#define __NN_BUILD_H__

typedef struct cg_allocate cg_allocate_t;
typedef struct cg_list cg_list_t;
typedef struct cg_node cg_node_t;
typedef struct linear_opt linear_opt_t;
typedef struct softmax_opt softmax_opt_t;
typedef struct mse_opt mse_opt_t;
typedef struct crossentropy_opt crossentropy_opt_t;

// 获取 node 的类型
char* node_type_of(cg_node_t* node);
// 判断 node 是不是某种类型
char* node_is(cg_node_t* node, const char* type);
// 线性运算
linear_opt_t* linear(cg_allocator_t* alloc, cg_list_t* build_stack, int *node_count, int in_dimens, int out_dimens, cg_list_t* node_recycle_list);
// relu 激活
relu_opt_t* relu(cg_allocator_t* alloc, cg_list_t* build_stack, int* node_count, cg_list_t* node_recycle_list);
// softmax
softmax_opt_t* softmax(cg_allocator_t* alloc, cg_list_t* build_stack, int* node_count, cg_list_t* node_recycle_list);
// most square error
mse_opt_t* mse(cg_allocator_t* alloc, cg_list_t* build_stack, int* node_count, cg_list_t* node_recycle_list);
// corss entropy
crossentropy_opt_t* crossentropy(cg_allocator_t* alloc, cg_list_t* build_stack, int* node_count, cg_list_t* node_recycle_list);

#endif