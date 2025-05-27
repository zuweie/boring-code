/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-05-27 11:41:55
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-05-27 11:46:27
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_node.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __CG_NODE_H__
#define __CG_NODE_H__

typedef void* cg_ref_t;
typedef struct cg_node {
    cg_ref_t ref;
    struct cg_node* prev;
} cg_node_t;

#endif