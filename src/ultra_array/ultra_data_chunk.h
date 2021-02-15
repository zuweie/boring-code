/*
 * @Author: your name
 * @Date: 2021-02-14 19:23:54
 * @LastEditTime: 2021-02-15 08:18:03
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/ultra_array/ultra_data_chunk.h
 */
#ifndef _ULTRA_DATA_CHUNK_H_
#define _ULTRA_DATA_CHUNK_H_

#include <stdlib.h>

typedef struct _data_chunk data_chunk_t;

typedef struct _data_chunk {
    char* chunk_addr;
    size_t chunk_size;
    struct _data_chunk* next;
} data_chunk_t;


data_chunk_t* DataChunk_create(char* addr, size_t size);
void DataChunk_addto_list(data_chunk_t** chunK_list, data_chunk_t* chunk);
void DataChunk_release(data_chunk_t* chunk_list);
#endif