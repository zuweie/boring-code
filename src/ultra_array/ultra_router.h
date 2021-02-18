/*
 * @Author: your name
 * @Date: 2021-02-08 20:16:55
 * @LastEditTime: 2021-02-17 21:37:40
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/ultra_array/ultra_router.h
 */
#ifndef _ULTRA_ROUTER_
#define _ULTRA_ROUTER_
#include <stdlib.h>
#include "ultra_data_chunk.h"
#include "ultra_array.h"

typedef struct _ua_indicator ua_indicator_t;

struct _ua_indicator {
    int __axis;
    int __picked;
    int __start;
    int __tail;
    struct _ua_indicator* next;
};

typedef struct _ua_chunk_note {
    size_t* shape;
    int axis_n;
    ua_data_chunk_t* chunk_map;
} ua_chunk_note_t;

ua_indicator_t* UA_indicator_create(int axis, int picked, int start, int tail);
void UA_indicator_release(ua_indicator_t* );

void UA_indicator_addto(ua_indicator_t**, ua_indicator_t*);

int UA_indicator_parse(char*, ua_indicator_t**);
int UA_indicator_analysis(ua_indicator_t*, u_array_t*, ua_chunk_note_t*);
int UA_survey_chuck_address(u_array_t*, char*, ua_indicator_t*, ua_chunk_note_t*);

void UA_chunk_note_finalize(ua_chunk_note_t*);

#endif

