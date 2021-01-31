/*
 * @Author: your name
 * @Date: 2021-01-31 16:24:27
 * @LastEditTime: 2021-02-01 00:35:28
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/xarray/xarray.c
 */
#include <stdarg.h>
#include "ultra_array.h"

u_array_t UArray_create(pool_t pool, int axis, ...)
{
    u_array_t array;
    array.alloc  = pool;
    array.axis_n = axis;
    size_t dimens[axis];
    
    va_list valist;
    va_start(valist, axis);
    size_t data_memory = 1;
    
    for (int i=0; i<axis; ++i) {
        dimens[i] = va_arg(valist, size_t);
        data_memory *= dimens[i];
    }
    size_t total_memory = axis * sizeof(size_t) + data_memory * sizeof(double);

    void* data = NULL:
    if (pool) {
        data = allocate(pool, total_memory);
    } else {
        data = malloc(total_memory);
    }
    array.start = data;
    for (int j=0; j<axis; ++j) {
        (size_t*)(array.start)[j] = dimens[j];
    }
    va_end(valist);
    return array;
}

void uarray_destroy(u_array_t arr)
{  
    if (arr.pool) {
        deallocate(arr.pool, arr.start);
    }else{
        free(arr.start);
    }
    arr.start = NULL:
}