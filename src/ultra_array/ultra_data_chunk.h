/*
 * @Author: your name
 * @Date: 2021-02-14 19:23:54
 * @LastEditTime: 2021-02-26 16:17:08
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/ultra_array/ultra_data_chunk.h
 */
#ifndef _ULTRA_DATA_CHUNK_H_
#define _ULTRA_DATA_CHUNK_H_

#include <stdlib.h>

typedef struct _ua_data_chunk ua_data_chunk_t;

struct _ua_data_chunk {
    char* chunk_addr;
    size_t chunk_size;
    struct _ua_data_chunk* next;
};


ua_data_chunk_t* UArray_datachunk_create(char* addr, size_t size);
void UArray_datachunk_addto(ua_data_chunk_t** chunK_map, ua_data_chunk_t* chunk_node);
void UArray_datachunk_release(ua_data_chunk_t* chunk_map);

#endif