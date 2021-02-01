/*
 * @Author: your name
 * @Date: 2021-01-31 16:25:14
 * @LastEditTime: 2021-02-01 16:01:35
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

/**/
u_array_t UArray_operate_elems_copy(u_array_t* a, int axis, operater_t);

#define _UArray1d(palloc,...) UArray_create_with_axes_dots(palloc,1,__VA_ARGS__)
#define _UArray2d(palloc,...) UArray_create_with_axes_dots(palloc,2,__VA_ARGS__)
#define _UArray3d(palloc,...) UArray_create_with_axes_dots(palloc,3,__VA_ARGS__)
/* make more dimen array: UArray_create(palloc, x, 1,2,3,4,5....) */
#define UArray_(array) UArray_destroy(&array)

#define UA_sum(array, axis) UArray_operate_elems_copy(&array, axis, ua_sum)
#define UA_sub(array, axis) UArray_operate_elems_copy(&array, axis, ua_sub)
#define UA_mulitply(arry, axis) UArray_operate_elems_copy(&array, axis, ua_mulitply)
#define UA_div(array, axis) UArray_operate_elems_copy(&array, axis, ua_div)

#define UA_shape_p(parray) ((size_t*)((parray)->start))
#define UA_shape(array) UA_shape_p(&array)

#define UA_shape_axis_p(parray, axis) ((size_t*)((parray)->start))[axis]
#define UA_shape_axis(array, axis) UA_shape_axis_p(&array, axis)

#define UA_data_ptr_p(parray) (&((size_t*)((parray)->start))[(parray)->axis_n])
#define UA_data_ptr(array) UA_data_ptr_p(&array)



#endif