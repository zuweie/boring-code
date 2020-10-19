/* 
 * @Author: your name
 * @Date: 2020-10-12 10:55:24
 * @LastEditTime: 2020-10-19 17:07:54
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/rbtree.h
 */
#ifndef _RBTREE_H_
#define _RBTREE_H_

#include "Cn.h"
#include "base/__rb_tree.h"

typedef Container RbTree;

#define RbTree_init(tree, insert_cmp) CN_initialize(ptree, rb_tree, NULL, insert_cmp)
#define RbTree_uninit(tree, cleanup) CN_uninitialize(ptree, rb_tree, cleanup)
#endif