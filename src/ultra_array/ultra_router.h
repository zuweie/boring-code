/*
 * @Author: your name
 * @Date: 2021-02-08 20:16:55
 * @LastEditTime: 2022-10-14 13:04:33
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/ultra_array/ultra_router.h
 */
#ifndef __ULTRA_ROUTER_H__
#define __ULTRA_ROUTER_H__
#include <stdlib.h>
typedef struct _u_array u_array_t;
typedef struct _ua_indicator ua_indicator_t;
typedef struct _ua_chunk_note ua_chunk_note_t;
typedef struct _ua_data_chunk ua_data_chunk_t;
typedef struct _ua_slicer ua_slicer_t;

struct _ua_indicator {
    int __axis;
    int __picked;
    int __start;
    int __tail;
    struct _ua_indicator* next;
};

struct _ua_slicer {
    int __start;
    int __tail;
};

struct _ua_chunk_note {
    size_t* shape;
    int axis_n;
    ua_data_chunk_t* chunk_map;
};

ua_indicator_t* UArray_indicator_create(int axis, int picked, int start, int tail);
void UArray_indicator_release(ua_indicator_t* );

void UArray_indicator_addto(ua_indicator_t**, ua_indicator_t*);

int UArray_indicator_parse_str(char*, ua_indicator_t**);
int UArray_indicator_parse_slicer(int, ua_slicer_t[], ua_indicator_t**);
int UArray_indicator_analysis(ua_indicator_t*, u_array_t*, ua_chunk_note_t*);
int UArray_survey_chuck_address(u_array_t*, char*, ua_indicator_t*, ua_chunk_note_t*);

void UArray_chunk_note_finalize(ua_chunk_note_t*);

/*
 *
 * 
 */
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

#define UA_scope_indicators(marco_indicators_arr, marco_n) \
    ua_indicator_t marco_indicators_arr[marco_n]; \
    { \
        int i; \
        for (i=0; i<(marco_n-1); ++i) { \
            marco_indicators_arr[i].next   = &marco_indicators_arr[i+1]; \
            marco_indicators_arr[i].__axis = i; \
            marco_indicators_arr[i].__start = 0; \
            marco_indicators_arr[i].__tail  = 0; \
            marco_indicators_arr[i].__picked = -1;\
        } \
        marco_indicators_arr[i].next   = NULL; \
        marco_indicators_arr[i].__axis = i;    \
        marco_indicators_arr[i].__start = 0; \
        marco_indicators_arr[i].__tail  = 0; \
        marco_indicators_arr[i].__picked = -1;\
    }

#define UA_set_scope_indicators(marco_indicators_arr, marco_axis, marco_picked, marco_start, marco_tail) \
    { \
        marco_indicators_arr[marco_axis].__picked = marco_picked; \
        marco_indicators_arr[marco_axis].__start  = marco_start;  \
        marco_indicators_arr[marco_axis].__tail   = marco_tail;   \
    }

#define UA_set_scope_indicators_start(marco_indicators_arr, marco_axis, marco_start) \
    UA_set_scope_indicators(marco_indicators_arr, marco_axis, -1, marco_start, 0)

#define UA_set_scope_indicators_tail(marco_indicators_arr, marcmarco_axiso_i, marco_tail) \
    UA_set_scope_indicators(marco_indicators_arr, marco_axis, -1, 0, marco_tail)

#define UA_set_scope_indicators_picked(marco_indicators_arr, marco_axis, marco_picked) \
    UA_set_scope_indicators(marco_indicators_arr, marco_axis, marco_picked, 0, 0)
    
#define UA_set_scope_indicators_selected(marco_indicators_arr, marco_axis, marco_start, marco_tail) \
    UA_set_scope_indicators(marco_indicators_arr, marco_axis, -1, marco_start, marco_tail)

#define _sc(s, t) (s),(t)
#define _pk(i) _sc(i, i+1)
#endif

