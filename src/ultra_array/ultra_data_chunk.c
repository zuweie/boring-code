/*
 * @Author: your name
 * @Date: 2021-02-14 19:23:37
 * @LastEditTime: 2021-02-17 17:04:33
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/ultra_array/ultra_data_chunk.c
 */
#include "ultra_data_chunk.h"

ua_data_chunk_t* UA_datachunk_create(char* addr, size_t size) {
    ua_data_chunk_t* ptr = malloc(sizeof(ua_data_chunk_t));
    ptr->chunk_addr = addr;
    ptr->chunk_size = size;
    ptr->next = NULL;
    return ptr;
}
void UA_datachunk_addto(ua_data_chunk_t** chunK_map, ua_data_chunk_t* chunk_node)
{
    if (*chunK_map == NULL) {
        *chunK_map = chunk_node;
    } else {
        ua_data_chunk_t* ptr = *chunK_map;
        while (ptr->next != NULL && (ptr = ptr->next));
        ptr->next = chunk_node;
    }
    return;
}
void UA_datachunk_release(ua_data_chunk_t* chunk_map)
{
    ua_data_chunk_t* ptr = chunk_map;
    while (ptr!= NULL) {
         
        ua_data_chunk_t* node_del = ptr;
        ptr = ptr->next;
        free(node_del);
        
    }
    return;
}