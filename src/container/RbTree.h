/* 
 * @Author: your name
 * @Date: 2020-10-12 10:55:24
 * @LastEditTime: 2020-10-16 12:17:41
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/rbtree.h
 */
#ifndef _RBTREE_H_
#define _RBTREE_H_

#include "Cn.h"
#include "base/__rb_tree.h"

typedef Container RbTree;

#define RbTree_init(ptree, insert_cmp) CN_initialize(ptree, rb_tree, NULL, insert_cmp)
#define RbTree_uninit(ptree, cleanup) CN_uninitialize(ptree, rb_tree, cleanup)

#endif