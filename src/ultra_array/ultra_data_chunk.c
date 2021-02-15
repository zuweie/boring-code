/*
 * @Author: your name
 * @Date: 2021-02-14 19:23:37
 * @LastEditTime: 2021-02-14 22:24:38
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/ultra_array/ultra_data_chunk.c
 */
#include "ultra_data_chunk.h"

data_chunk_t* DataChunk_create(char* addr, size_t size) {
    data_chunk_t* ptr = malloc(sizeof(data_chunk_t));
    ptr->chunk_addr = addr;
    ptr->chunk_size = size;
    ptr->next = NULL;
    return ptr;
}
void DataChunk_addto_list(data_chunk_t** chunK_list, data_chunk_t* chunk)
{
    if (*chunK_list == NULL) {
        *chunK_list = chunk;
    } else {
        data_chunk_t* ptr = *chunK_list;
        while (ptr->next != NULL && (ptr = ptr->next));
        ptr->next = chunk;
    }
    return;
}
void DataChunk_release(data_chunk_t* chunk_list)
{
    data_chunk_t* ptr = chunk_list;
    while (ptr!= NULL) {
         
        data_chunk_t* node_del = ptr;
        ptr = ptr->next;
        free(node_del);
    }
    return;
}