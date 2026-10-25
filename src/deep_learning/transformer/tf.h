/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-09-13 12:38:03
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-09-17 21:46:04
 * @FilePath: /boring-code/src/deep_learning/transformer/tf.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __TF_H__
#define __TF_H__

#include "deep_learning/compute_graph2/cg_allocator.h"

typedef struct cg_list cg_list_t;

typedef struct tf {

    cg_allocator_t alloc;
    cg_list_t*     build_stack;
    cg_list_t*     node_list;

} tf_t;

int tf_init(tf_t* model);
int tf_reset(tf_t* model);

#endif