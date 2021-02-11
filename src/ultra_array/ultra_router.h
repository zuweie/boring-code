/*
 * @Author: your name
 * @Date: 2021-02-08 20:16:55
 * @LastEditTime: 2021-02-11 13:03:50
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/ultra_array/ultra_router.h
 */
#ifndef _ULTRA_ROUTER_
#define _ULTRA_ROUTER_
#include <stdlib.h>

typedef struct _route_node route_node_t;

struct _route_node {

    int axis;
    int __picked;
    int __start;
    int __tail;
    struct _route_node* next;
};

route_node_t* Router_create(int axis, int picked, int start, int tail);

void Router_addto_list(route_node_t** list, route_node_t* route);

void Router_parse(char* router, route_node_t** list);

void Router_release(route_node_t* route_list);



#endif

