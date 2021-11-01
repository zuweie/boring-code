/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-11 10:07:38
 * @LastEditTime: 2021-11-01 11:06:07
 * @LastEditors: Please set LastEditors
 */
#ifndef __RB_TREE_H__
#define __RB_TREE_H__

#include "type_value/__type_value.h"
#include "type_value/__type_value_def.h"
#include "__container.h"

#define _rb_red 0
#define _rb_black 1

#define _null(prb) (&((prb)->_null))

typedef struct _rb_tree_node rb_tree_node_t;
struct _rb_tree_node {
    unsigned char color;
    rb_tree_node_t* parent;
    rb_tree_node_t* left;
    rb_tree_node_t* right;
    type_value_t w[1];
};

typedef struct _rb_tree {
    container_t container;
    rb_tree_node_t* _root;
    rb_tree_node_t* _first;
    rb_tree_node_t* _last;
    rb_tree_node_t _null;
    size_t _size;
    unsigned char multi;
    T_def type_def;
    int (*setup) (type_value_t*, type_value_t*);
    int (*conflict_fix) (type_value_t*, type_value_t*);
}rb_tree_t;

container_t* rb_tree_create(T_clazz* __type_clazz, unsigned char multi_key);

int rb_tree_destroy(container_t*);

iterator_t rb_tree_root(rb_tree_t*);
iterator_t rb_tree_null(rb_tree_t*);
#endif