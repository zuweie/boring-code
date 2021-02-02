/*
 * @Author: your name
 * @Date: 2021-01-31 16:24:27
 * @LastEditTime: 2021-02-02 15:21:13
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
u_array_t UArray_operate_new_copy(u_array_t* arr, int axis, operater_t op)
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

u_array_t UArray_transpose_new_copy(u_array_t* arr, size_t* trans_axis_index) 
{
    
    // size_t trans_axes[arr->axis_n];
    // for (int i=0; i<arr->axis_n; ++i) {
    //     trans_axes[i] = UA_shape_axis_p(arr, trans_axis[i]);
    // }

    // u_array_t trans = UArray_create_with_axes_array(arr->alloc, arr->axis_n, trans_axes);

    // double* trans_data = UA_data_ptr(trans);
    // double* arr_data   = UA_data_ptr_p(arr);

    // size_t trans_size = UA_size_p(arr);
}

size_t UArray_xd_coord_to_1d_index(u_array_t* arr, size_t* coord)
{
    size_t index = 0, axis_mulitply;
    int axis_n = arr->axis_n;
    for (int i=0; i<axis_n; ++i) {
        size_t co = coord[i];
        axis_mulitply = UA_axis_mulitply_p(arr, i+1);
        index += co * axis_mulitply;
    }
    return index;
}

void UArray_1d_index_to_xd_coord(u_array_t* arr, size_t offset, size_t* coord)
{
    size_t div, mod, i, axis_mulitply;
    div = offset;
    for(i=0; i<arr->axis_n-1; ++i) {
        axis_mulitply = UA_axis_mulitply_p(arr, i+1);
        div = div / axis_mulitply;
        mod = div % axis_mulitply;
        coord[i] = div;
        div = mod;
    }
    coord[i] = mod;
    return;
}