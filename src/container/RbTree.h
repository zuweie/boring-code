/* 
 * @Author: your name
 * @Date: 2020-10-12 10:55:24
 * @LastEditTime: 2020-10-26 12:31:15
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/container/rbtree.h
 */
#ifndef _RBTREE_H_
#define _RBTREE_H_

#include "Cn.h"
#include "base/__rb_tree.h"

typedef Container RbTree;

// #define RbTree_init(tree, setup, conflict_fix, cleanup, insert_cmp) \ 
//     CN_initialize(ptree, rb_tree, NULL, setup, conflict_fix, NULL, NULL, NULL, cleanup, insert_cmp)
     
// #define RbTree_uninit(tree) \
//     CN_uninitialize(ptree, rb_tree)

#define _RbTree(insert_cmp, setup, conflict_fix, cleanup) \
    ({ \
        RbTree tree; \
        CN_initialize(tree, rb_tree, NULL, setup, conflict_fix, cleanup, insert_cmp); \
        tree; \
    })

#define RbTree_(tree) CN_uninitialize(tree, rb_tree);

#endif