/*
 * @Author: your name
 * @Date: 2021-01-31 16:24:27
 * @LastEditTime: 2021-02-03 11:31:04
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

static size_t 
__axis_mulitply(size_t axes[], int axis_n, int from) 
{
    size_t size = 1;
    for (int i=from; i<axis_n; ++i) {
        size *= axes[i];
    }
    return size;
}

static void*
__alloc_start(pool_t* alloc, size_t n) 
{
    void* start;
    if (alloc) {
        start = allocate(alloc, n);
    } else {
        start = malloc(n);
    }
    return start;
}

static void
__recycle_start(u_array_t* parr) 
{
    if (parr->start != NULL) {
        if (parr->alloc) {
            deallocate(parr->alloc, parr->start);
        }else{
            free(parr->start);
        }
        parr->start = NULL;
    }
    return;

}

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
        n_array.start = __alloc_start(alloc, total_size);

        if (axis_n > 0) 
            memcpy(n_array.start, axes, axis_n*sizeof(size_t));
        
        return n_array;
    }
    return ua_unable;
}

void UArray_range(u_array_t *a, int range)
{
    double* data   = UA_data_ptr(a);
    size_t  size_a = UA_size(a);
    
    for (int i=0; i<range && i<size_a; ++i) {
        data[i] = i;
    }
    return;
}

int UArray_reshape(u_array_t* a, size_t axes[], int axis_n) 
{
    size_t size_r = __axis_mulitply(axes, axis_n, 0);
    size_t size_a = UA_size(a);
    if (size_r == size_a) {
        if (a->axis_n == axis_n) {
            memcpy(UA_shape(a), axes, axis_n * sizeof(size_t));
        } else {
            // 否则需要重新分配内存。
            // 计算 size
            size_t data_size  = __axis_mulitply(axes, axis_n, 0) * sizeof(double);
            size_t total_size = axis_n * sizeof(size_t) + data_size;

            void* start = __alloc_start(a->alloc, total_size);
            //复制 axis 信息
            memcpy(start, axes, axis_n * sizeof(size_t));
            // 复制 data
            memcpy( &(((size_t*)start)[axis_n]), UA_data_ptr(a),  data_size);

            __recycle_start(a);
            a->start = start;
        }
        return 0;
    }
    return -1;
}

void UArray_destroy(u_array_t* parr)
{  
    return __recycle_start(parr);
}

//操作完后降维产生副本。
u_array_t UArray_operate_new_copy(u_array_t* arr, int axis, operater_t op)
{
    if (axis>=0 && axis<arr->axis_n) {

        size_t number = 1;
        size_t* shape = UA_shape(arr);
        size_t axes[axis == 0?1:axis];

        for (int i=0; i<axis; ++i) {
            number  *= UA_shape_axis(arr, i);
            axes[i]  = UA_shape_axis(arr, i);
        }
        
        size_t step = 1;
        for (int j=axis; j<arr->axis_n; ++j) {
            step *= UA_shape_axis(arr, j);
        }
        // 此处需要降维
        u_array_t n_arr = UArray_create_with_axes_array(arr->alloc, axis, axes);

        double* arr_data_ptr  = UA_data_ptr(arr);
        double* narr_data_ptr = UA_data_ptr(&n_arr);
        
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


size_t UArray_axis_mulitply(u_array_t* a, int axis_idx_from) 
{
    return __axis_mulitply(UA_shape(a), a->axis_n, axis_idx_from);
}

u_array_t UArray_transpose_new_copy(u_array_t* arr, size_t trans_axis_index[]) 
{
    size_t trans_axes[arr->axis_n];

    for (int i=0; i<arr->axis_n; ++i) {
        trans_axes[i] = UA_shape_axis(arr, trans_axis_index[i]);
    }

    u_array_t trans = UArray_create_with_axes_array(arr->alloc, arr->axis_n, trans_axes);

    double* trans_data = UA_data_ptr(&trans);
    double* arr_data   = UA_data_ptr(arr);
    
    size_t  arr_size   = UA_size(arr);
    
    size_t arr_coord[arr->axis_n];
    size_t trans_coord[trans.axis_n];
    
    for (int i=0; i<arr_size; ++i) {
        UA_cover_offset(arr, i, arr_coord);
        // 坐标转换啊。
        for (int j=0; j<trans.axis_n; ++j) {
            trans_coord[j] = arr_coord[ trans_axis_index[j] ];
        }
        size_t index = UA_cover_coordinate(&trans, trans_coord);
        trans_data[index] = arr_data[i];
    }
    return trans;
}

u_array_t UArray_transform_new_copy(u_array_t* arr) {
    size_t axis_index[arr->axis_n];
    for (int i=0; i<arr->axis_n; ++i) {
        axis_index[i] = arr->axis_n - i -1;
    }
    return UArray_transpose_new_copy(arr, axis_index);
}

size_t UArray_xd_coord_to_1d_offset(u_array_t* arr, size_t* coord)
{
    size_t offset = 0, axis_mulitply;
    int axis_n = arr->axis_n;
    for (int i=0; i<axis_n; ++i) {
        size_t co = coord[i];
        axis_mulitply = UArray_axis_mulitply(arr, i+1);
        offset += co * axis_mulitply;
    }
    return offset;
}

void UArray_1d_offset_to_xd_coord(u_array_t* arr, size_t offset, size_t* coord)
{
    size_t div, mod, i, axis_mulitply, middle_value;
    middle_value = offset;
    for(i=0; i<arr->axis_n-1; ++i) {
        axis_mulitply = UArray_axis_mulitply(arr, i+1);
        div = middle_value / axis_mulitply;
        mod = middle_value % axis_mulitply;
        coord[i] = div;
        middle_value = mod;
    }
    coord[i] = mod;
    return;
}