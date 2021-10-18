/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-03 15:07:40
 * @LastEditTime: 2021-10-18 15:59:47
 * @LastEditors: Please set LastEditors
 */

#ifndef __LIST_H__
#define __LIST_H__
#include "type_value/__type_value.h"
#include "type_value/__type_value_def.h"
#include "__container.h"

#define list_head(list) (&(((list_t*)list)->_sentinel))
#define list_tail(list) (&(((list_t*)list)->_sentinel))
#define list_first(list) (list_head(list)->next)
#define list_last(list)  (list_tail(list)->prev)

typedef struct _list_node list_node_t;
struct _list_node
{
    /* 数据节点的大小不再固定 */
    list_node_t* prev;
    list_node_t* next;
    type_value_t w[1];
};

typedef struct _list 
{
    container_t container;
    list_node_t _sentinel;
    size_t _size;
    
} list_t;

container_t* list_create(T_def* _def);
int list_destroy(container_t*);
#endif
