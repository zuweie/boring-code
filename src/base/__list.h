/*
 * @Description: In User Settings Edit
 * @Author: your name
 * @Date: 2019-09-03 15:07:40
 * @LastEditTime: 2021-10-15 15:20:15
 * @LastEditors: Please set LastEditors
 */

#ifndef _LIST_H_
#define _LIST_H_
#include "type_value/__type_value.h"
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

container_t* list_create();
int list_destroy(container_t*);
#endif
