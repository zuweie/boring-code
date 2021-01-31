/*
 * @Author: your name
 * @Date: 2021-01-31 16:25:14
 * @LastEditTime: 2021-02-01 00:50:54
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/xarray/xarray.h
 */
#ifndef _U_ARRAY_H_
#define _U_ARRAY_H_
#include "mem_pool/__mem_pool.h"

typedef struct _u_array {
    void* start;
    int axis_n;
    pool_t* alloc;
} u_array_t;

u_array_t UArray_create(poolt_t* alloc, int axis, ...);
void UArray_destroy(u_array_t a);

#define _UArray(palloc, axis, ...) UArray_create(palloc, axis, __VA_ARGS__)
#define UArray_(array) UArray_destory(array)

#define shape_list(array) (size_t*)(array.start)
#define shape(array, axis) ((size_t*)(array.start))[axis]
#define data_ptr(array) (&((size_t*)(array.start))[data.axis_n])

#endif