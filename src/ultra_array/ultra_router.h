/*
 * @Author: your name
 * @Date: 2021-02-08 20:16:55
 * @LastEditTime: 2021-02-15 08:34:24
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/ultra_array/ultra_router.h
 */
#ifndef _ULTRA_ROUTER_
#define _ULTRA_ROUTER_
#include <stdlib.h>
#include "ultra_data_chunk.h"
#include "ultra_array.h"

typedef struct _ua_route_node ua_route_node_t;

struct _ua_route_node {

    int __axis;
    int __picked;
    int __start;
    int __tail;
    struct _ua_route_node* next;

};

typedef struct _ua_router_middle_ware {
    
    size_t* shape;
    int axis_n;
    ua_data_chunk_t* chunk_map;

} ua_router_middle_ware_t;

ua_route_node_t* UA_router_create(int axis, int picked, int start, int tail);

void UA_router_addto(ua_route_node_t** router, ua_route_node_t* node);

void UA_router_parse(char* router_str, route_node_t** router);
void UA_router_analysis(ua_route_node_t* router, u_array_t* arr, ua_router_middle_ware_t* middle_ware);
void UA_router_release(ua_route_node_t* router);

void UA_router_middleware_init(ua_router_middle_ware_t* middle_ware);
void UA_router_middleware_uninit(ua_router_middle_ware_t* middle_ware);

#endif

