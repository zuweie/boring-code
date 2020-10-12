/*
 * @Author: your name
 * @Date: 2020-10-12 10:55:24
 * @LastEditTime: 2020-10-12 12:25:46
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/rbtree.h
 */
#ifndef _RBTREE_H_
#define _RBTREE_H_

#include "cn.h"
#include "base/__rb_tree.h"

typedef Container RbTree;

#define RbTree_init(ptree, node_cmp, insert_cmp) CN_init(ptree, rb_tree, node_cmp, insert_cmp)
#define RbTree_free(ptree) CN_free(ptree, rb_tree)

#define RbTree_root(ptree) rb_tree_root(cc(ptree))
#define RbTree_null(ptree) rb_tree_null(cc(ptree))

#endif