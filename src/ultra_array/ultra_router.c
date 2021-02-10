/*
 * @Author: your name
 * @Date: 2021-02-09 13:27:15
 * @LastEditTime: 2021-02-10 13:42:22
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/ultra_array/ultra_router.c
 */
#include <stdlib.h>
#include <string.h>
#include "ultra_router.h"
#include "ultra_array.h"

route_node_t* Router_create(int axis, size_t axis_start, size_t axis_tail)
{
    route_node_t* route = malloc(sizeof(route_node_t));
    route->axis = axis;
    route->axis_start = axis_start;
    route->axis_tail   = axis_tail;
    route->next       = NULL;
    return route;
}

void Router_addto_list(route_node_t** route_list, route_node_t* route) 
{
    if (*route_list == NULL) {
        *route_list = route;
    } else {
        route_node_t* ptr = *route_list;
        while (ptr->next != NULL && (ptr = ptr->next));
        ptr->next = route;
    }
    return;
}

void Router_parse(u_array_t* arr, char router[], route_node_t** route_list)
{
    *route_list = NULL;
    int curr_axis = 0;

    const int BUF_SZ  = 256;
    char buf[BUF_SZ]  = {'\0'};

    size_t axis_start = 0;
    size_t axis_tail   = 0;

    char *forward, *buf_ptr;
    forward = router;
    buf_ptr = buf;

    int scope_index = 0;;
    int tail;
    while( *forward != '\0' ) {

        if ( *forward == ',' ) {

            if (buf[0] != '\0') {

                *(buf_ptr+1) = '\0';

                if (scope_index == 0) {
                    // axis_start 与 axis_tail 同一位置
                    axis_start = atoi(buf);
                    axis_tail   = axis_start + 1;
                } else {
                    tail = atoi(buf);
                    if (tail <0) {
                        axis_tail = tail + UA_shape_axis(arr, curr_axis);
                    } else {
                        axis_tail = tail;
                    }
                }
            } else {
                if (scope_index == 0) {
                    axis_start = 0;
                }
                axis_tail = UA_shape_axis(arr, curr_axis);
            }

            buf[0] = '\0';
            buf_ptr = buf;

            route_node_t* route = Router_create(curr_axis++, axis_start, axis_tail);
            Router_addto_list(route_list, route);
            scope_index = 0;

        } else if ( *forward == ':' ) {

            if (buf[0] != '\0') {
                *(buf_ptr+1) = '\0';
                axis_start = atoi(buf);
            } else {
                axis_start = 0;
            }

            buf_ptr = buf;
            buf[0] = '\0';
            scope_index = 1;

        } else {
            *buf_ptr++ = *forward;
        } 

        forward++;
    }
    //结束后看看还有没有
    if (strlen(buf) != 0) {

        *(buf_ptr + 1) = '\0';

        if ( scope_index == 0 ) {
            axis_start = atoi(buf);
            axis_tail   = axis_start + 1;
        } else {
            tail = atoi(buf);
            if (tail < 0) {
                axis_tail = tail + UA_shape_axis(arr, curr_axis);
            } else {
                axis_tail = tail;
            }
        }
        route_node_t* route = Router_create(curr_axis++, axis_start, axis_tail);
        Router_addto_list(route_list, route);
    } else if ( scope_index == 1 ) {
        
        axis_tail = UA_shape_axis(arr, curr_axis);

        route_node_t* route = Router_create(curr_axis++, axis_start, axis_tail);
        Router_addto_list(route_list, route);

    }
    return;
}
void Router_router_to_shape(route_node_t* list, u_array_t* arr, size_t **shape, int *axis_n) 
{

}

void Router_release(route_node_t* route_list)
{
    route_node_t* ptr = route_list;
    while (ptr!= NULL) {
         
        route_node_t* node_del = ptr;
        ptr = ptr->next;
        free(node_del);
    }
    return;
}