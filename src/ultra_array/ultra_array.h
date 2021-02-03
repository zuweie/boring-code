/*
 * @Author: your name
 * @Date: 2021-01-31 16:25:14
 * @LastEditTime: 2021-02-03 11:16:16
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/xarray/xarray.h
 */
#ifndef _ULTRA_ARRAY_H_
#define _ULTRA_ARRAY_H_

#include "mem_pool/__mem_pool.h"

typedef enum {ua_sum = 1 , ua_sub, ua_mulitply, ua_div} operater_t;

typedef struct _u_array {
    void* start;
    int axis_n;
    pool_t* alloc;
} u_array_t;

extern u_array_t ua_unable;

u_array_t UArray_create_with_axes_dots(pool_t* alloc, int axis_n, ...);
u_array_t UArray_create_with_axes_array(pool_t* alloc, int axis_n, size_t axes[]);
void UArray_destroy(u_array_t* parr);

/* 操作后产生新的 ultra array 的函数 */
u_array_t UArray_operate_new_copy(u_array_t* a, int axis, operater_t);
u_array_t UArray_transpose_new_copy(u_array_t* a, size_t trans_axis_index[]);
u_array_t UArray_transform_new_copy(u_array_t* a);

/* 以下操作不会产生新副本 */
size_t UArray_xd_coord_to_1d_offset(u_array_t* arr, size_t* coord);
void UArray_1d_offset_to_xd_coord(u_array_t* arr, size_t offset, size_t* coord);
int UArray_reshape(u_array_t* a, size_t axes[], int axis_n);
size_t UArray_axis_mulitply(u_array_t* a, int axis_idx_from);
void UArray_range(u_array_t *a, int range);

#define _UArray1d(palloc,...) UArray_create_with_axes_dots(palloc,1,__VA_ARGS__)
#define _UArray2d(palloc,...) UArray_create_with_axes_dots(palloc,2,__VA_ARGS__)
#define _UArray3d(palloc,...) UArray_create_with_axes_dots(palloc,3,__VA_ARGS__)
// #define _UArray_range(palloc, range) UArray_range(palloc, range)

/* make more dimen array: UArray_create(palloc, x, 1,2,3,4,5....) */
#define UArray_(parray) UArray_destroy(parray)

#define UA_sum(parray, axis) UArray_operate_new_copy(parray, axis, ua_sum)
#define UA_sub(parray, axis) UArray_operate_new_copy(parray, axis, ua_sub)
#define UA_mulitply(parry, axis) UArray_operate_new_copy(parray, axis, ua_mulitply)
#define UA_div(parray, axis) UArray_operate_new_copy(parray, axis, ua_div)
#define UA_T(parray) UArray_transform_new_copy(parray)
#define UA_range(parray, range) UArray_range(parray, range)

#define UA_cover_coordinate(parray, coord) UArray_xd_coord_to_1d_offset(parray, coord)
#define UA_cover_offset(parray, offset, coord) UArray_1d_offset_to_xd_coord(parray, offset, coord)

#define UA_shape(parray) ((size_t*)((parray)->start))
#define UA_shape_axis(parray, axis_index) ((size_t*)((parray)->start))[axis_index]
#define UA_data_ptr(parray) (&((size_t*)((parray)->start))[(parray)->axis_n])

#define UA_size(parray) UArray_axis_mulitply(parray, 0)
#define UA_reshape(parray, axes, axis_n) UArray_reshape(parray, axes, axis_n)

#endif