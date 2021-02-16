/*
 * @Author: your name
 * @Date: 2021-02-14 19:23:54
 * @LastEditTime: 2021-02-16 23:25:05
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/ultra_array/ultra_data_chunk.h
 */
#ifndef _ULTRA_DATA_CHUNK_H_
#define _ULTRA_DATA_CHUNK_H_

#include <stdlib.h>

typedef struct _ua_data_chunk ua_data_chunk_t;

typedef struct _ua_data_chunk {
    char* chunk_addr;
    size_t chunk_size;
    struct _ua_data_chunk* next;
} ua_data_chunk_t;


ua_data_chunk_t* UA_datachunk_create(char* addr, size_t size);
void UA_datachunk_addto(ua_data_chunk_t** chunK_map, ua_data_chunk_t* chunk_node);
void UA_datachunk_release(ua_data_chunk_t* chunk_map);
#endif