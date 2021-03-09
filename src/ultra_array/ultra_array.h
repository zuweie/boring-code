/*
 * @Author: your name
 * @Date: 2021-01-31 16:25:14
 * @LastEditTime: 2021-03-08 11:06:42
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/xarray/xarray.h
 */
#ifndef _ULTRA_ARRAY_H_
#define _ULTRA_ARRAY_H_
#include "ultra_padding.h"

typedef enum _operator {ua_sum = 1 , ua_sub, ua_mulitply, ua_div} operater_t;

typedef struct _ua_chunk_note ua_chunk_note_t;
typedef struct _ua_data_chunk ua_data_chunk_t;
typedef struct _ua_indicator ua_indicator_t;
typedef struct _u_array u_array_t;

struct _u_array {
    char *start[2];
    int axis_n;
};

extern u_array_t ua_unable;

u_array_t UArray_create_with_axes_dots(int, ...);
u_array_t UArray_create_with_axes_array(int, size_t[]);
void UArray_destroy(u_array_t*);

/* 操作后产生新的 ultra array 副本的函数 */
void* UArray_data_copy(u_array_t*);

u_array_t* UArray_operate(u_array_t*, int, operater_t);
u_array_t* UArray_transpose(u_array_t*, size_t[]);
u_array_t* UArray_transform(u_array_t*);
u_array_t* UArray_reshape(u_array_t*, size_t[], int);
u_array_t* UArray_arange(u_array_t*, int);
u_array_t* UArray_arange_scope(u_array_t*, int, int);
u_array_t* UArray_ones(u_array_t*, double);
u_array_t* UArray_assimilate(u_array_t*, char[], u_array_t*);
u_array_t* UArray_assimilate_with_indicators(u_array_t*, ua_indicator_t*, u_array_t*);
u_array_t* UArray_log(u_array_t*);
u_array_t* UArray_pow2(u_array_t*);

/* return new copy */
u_array_t UArray_dot_new_copy(u_array_t*, u_array_t*);
u_array_t UArray_fission(u_array_t*, char[]);
u_array_t UArray_fission_with_indicators(u_array_t*, ua_indicator_t*);
u_array_t UArray_padding(u_array_t*, ua_pad_width_t[], ua_pad_mode_t);
u_array_t UArray_pad(u_array_t*, char[], ua_pad_mode_t);
u_array_t UArray_empty_like(u_array_t*);

size_t UArray_xd_coord_to_1d_offset(u_array_t*, size_t*);
void UArray_1d_offset_to_xd_coord(u_array_t*, size_t, size_t*);
size_t UArray_axis_mulitply(u_array_t* a, int);
double UArray_get(u_array_t*, ...);
void UArray_set(u_array_t*, double, ...);

#define _UArray1d(...) UArray_create_with_axes_dots(1,__VA_ARGS__)
#define _UArray2d(...) UArray_create_with_axes_dots(2,__VA_ARGS__)
#define _UArray3d(...) UArray_create_with_axes_dots(3,__VA_ARGS__)
#define _UArrayXd(x,...) UArray_create_with_axes_dots(x, __VA_ARGS__)
// #define _UArray_range(palloc, range) UArray_range(palloc, range)
#define UArray_(parray) UArray_destroy(parray)

#define UA_get(parray, ...) UArray_get(parray, __VA_ARGS__)
#define UA_set(parray, v, ...) UArray_set(parray, v, __VA_ARGS__)

#define UA_sum(parray, axis) UArray_operate(parray, axis, ua_sum)
#define UA_sub(parray, axis) UArray_operate(parray, axis, ua_sub)
#define UA_mulitply(parry, axis) UArray_operate(parray, axis, ua_mulitply)
#define UA_div(parray, axis) UArray_operate(parray, axis, ua_div)
#define UA_T(parray) UArray_transform(parray)
#define UA_arange(parray, range) UArray_arange(parray, range)
#define UA_scope(parray, start, tail) UArray_arange_scope(parray, start, tail)

#define UA_ones(parray, v) UArray_ones(parray, v)
#define UA_dot(pa1, pa2) UArray_dot_new_copy(pa1, pa2)
#define UA_data_copy(parray) UArray_data_copy(parray)
#define UA_fission(parray, router) UArray_fission(parray, router)
#define UA_assimilate(pa1, router, pa2) UArray_assimilate(pa1, router, pa2)
#define UA_copy(parray) UArray_fission(parray, "")
#define UA_log(parray) UArray_log(parray)
#define UA_pow2(parray) UArray_pow2(parray)
#define UA_empty_like(parray) UArray_empty_like(parray)
#define UA_pad_edge(parray, pad_width) UArray_pad(parray, pad_width, ua_pad_mode_edge)

#define UA_cover_coordinate(parray, coord) UArray_xd_coord_to_1d_offset(parray, coord)
#define UA_cover_offset(parray, offset, coord) UArray_1d_offset_to_xd_coord(parray, offset, coord)

#define UA_shape(parray) ((size_t*)(parray)->start[0])
#define UA_axisn(parray) ((parray)->axis_n)
#define UA_shape_axis(parray, axis_index) UA_shape(parray)[axis_index]
#define UA_data_ptr(parray) ((char*)(parray)->start[1])
#define UA_size(parray) UArray_axis_mulitply(parray, 0)
#define UA_reshape(parray, axes, axis_n) UArray_reshape(parray, axes, axis_n)
#define UA_where(parray, condition, replace) \
    ({ \
        double* data_ptr = UA_data_ptr(parray); \
        size_t size_arr  = UA_size(parray); \
        for (size_t i=0; i<size_arr; ++i) { \
            if ( data_ptr[i] condition ) { \
                data_ptr[i] = replace; \
            } \
        } \
    })

#endif