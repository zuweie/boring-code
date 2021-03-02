/*
 * @Author: your name
 * @Date: 2021-02-08 20:16:55
 * @LastEditTime: 2021-03-02 12:11:09
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/ultra_array/ultra_router.h
 */
#ifndef _ULTRA_ROUTER_
#define _ULTRA_ROUTER_
#include <stdlib.h>

typedef struct _u_array u_array_t;
typedef struct _ua_indicator ua_indicator_t;
typedef struct _ua_chunk_note ua_chunk_note_t;
typedef struct _ua_data_chunk ua_data_chunk_t;

struct _ua_indicator {
    int __axis;
    int __picked;
    int __start;
    int __tail;
    struct _ua_indicator* next;
};

struct _ua_chunk_note {
    size_t* shape;
    int axis_n;
    ua_data_chunk_t* chunk_map;
};

ua_indicator_t* UArray_indicator_create(int axis, int picked, int start, int tail);
void UArray_indicator_release(ua_indicator_t* );

void UArray_indicator_addto(ua_indicator_t**, ua_indicator_t*);

int UArray_indicator_parse(char*, ua_indicator_t**);
int UArray_indicator_analysis(ua_indicator_t*, u_array_t*, ua_chunk_note_t*);
int UArray_survey_chuck_address(u_array_t*, char*, ua_indicator_t*, ua_chunk_note_t*);

void UArray_chunk_note_finalize(ua_chunk_note_t*);

#define __indicators(_p_marco_indicator, marco_picked, marco_start, marco_tail) \
    ({ \
        ua_indicator_t* __p_indicator = _p_marco_indicator; \
        int marco_axis = 0; \
        if ( __p_indicator != NULL )  \
            marco_axis = __p_indicator->__axis + 1; \
        void* p = UArray_indicator_create(marco_axis, marco_picked, marco_start, marco_tail); \
        UArray_indicator_addto(&__p_indicator, p); \
        __p_indicator; \
    })

#define __indicators_picked(_p_marco_indicator, marco_picked) __indicators(_p_marco_indicator, marco_picked, 0, 0)
#define __indicators_start_tail(_p_marco_indicator, marco_start, marco_tail) __indicators(_p_marco_indicator, -1, marco_start, marco_tail)
#define __indicators_start(_p_marco_indicator, marco_start) __indicators(_p_marco_indicator, -1, marco_start, 0)
#define __indicators_tail(_p_marco_indicator, marco_tail) __indicators(_p_marco_indicator, -1, 0, marco_tail)

#endif

