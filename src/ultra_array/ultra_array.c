/*
 * @Author: your name
 * @Date: 2021-01-31 16:24:27
 * @LastEditTime: 2021-02-01 23:18:57
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/xarray/xarray.c
 */
#include <stdarg.h>
#include <string.h>
#include "ultra_array.h"

u_array_t ua_unable = {
    .start = NULL,
    .axis_n = -1,
    .alloc = NULL
};

u_array_t UArray_create_with_axes_dots(pool_t* alloc, int axis_n, ...)
{
    va_list valist;
    va_start(valist, axis_n);
    size_t axes[axis_n];
    for (int i=0; i<axis_n; ++i) {
        axes[i] = va_arg(valist, size_t);
    }
    va_end(valist);
    return UArray_create_with_axes_array(alloc, axis_n, axes);
}

u_array_t UArray_create_with_axes_array(pool_t* alloc, int axis_n, size_t axes[]) 
{
    if (axis_n >= 0) {
        u_array_t n_array;
        n_array.alloc  = alloc;
        n_array.axis_n = axis_n;

        size_t data_n = 1;

        for (int i=0; i<axis_n; ++i) {
            data_n *= axes[i];
        }
        size_t total_size = axis_n * sizeof(size_t) + data_n * sizeof(double);

        if (alloc) {
            n_array.start = allocate(alloc, total_size);
        } else {
            n_array.start = malloc(total_size);
        }

        // for(int j=0; j<axis_n; ++j) {
        //     ((size_t*)(n_array.start))[j] = (axis_n == 0? 1: axes[j]);
        // }
        if (axis_n > 0) 
            memcpy(n_array.start, axes, axis_n*sizeof(size_t));
        
        return n_array;
    }
    return ua_unable;
}

void UArray_destroy(u_array_t* parr)
{  
    if (parr->axis_n < 0)
        return;

    if (parr->alloc) {
        deallocate(parr->alloc, parr->start);
    }else{
        free(parr->start);
    }
    parr->start = NULL;

    return;
}

//操作完后降维产生副本。
u_array_t UArray_operate_elems_copy(u_array_t* arr, int axis, operater_t op)
{
    if (axis>=0 && axis<arr->axis_n) {

        size_t number = 1;
        size_t* shape = UA_shape_p(arr);
        size_t axes[axis == 0?1:axis];

        for (int i=0; i<axis; ++i) {
            number  *= UA_shape_axis_p(arr, i);
            axes[i]  = UA_shape_axis_p(arr, i);
        }
        
        size_t step = 1;
        for (int j=axis; j<arr->axis_n; ++j) {
            step *= UA_shape_axis_p(arr, j);
        }
        // 此处需要降维
        u_array_t n_arr = UArray_create_with_axes_array(arr->alloc, axis, axes);

        double* arr_data_ptr  = UA_data_ptr_p(arr);
        double* narr_data_ptr = UA_data_ptr(n_arr);
        
        double result;

        // 开始计算
        for (int k=0, m=0; k<number; ++k, ++m) {

            if (op == ua_sub || op == ua_sum) {
                result = 0.f;
            } else {
                result = 1.f;
            }
            
            for (int l=0; l<step; ++l) {
                switch (op)
                {
                case ua_sum:
                    result += arr_data_ptr[ k*step + l ];
                    break;
                case ua_sub:
                    result -= arr_data_ptr[ k*step + l ];
                    break;
                case ua_mulitply:
                    result *= arr_data_ptr[ k*step + l ];
                    break;
                case ua_div:
                    result /= arr_data_ptr[ k*step + l ];
                    break;
                default:
                    break;
                }
            }
            narr_data_ptr[m] = result;
        }
        return n_arr;
    }
}