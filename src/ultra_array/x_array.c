/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-10-11 15:07:54
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-10-12 15:58:32
 * @FilePath: /boring-code/src/ultra_array/x_array.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "x_array.h"


static int __calculate_dimens_unit_size(int* dimens, int axisn, int axis)
{
    if (axis < axisn) {

        int unit_size = 1;

        for (int i=axis+1; i<axisn; ++i) {

            unit_size *= dimens[i];

        }

        return unit_size *= sizeof(float);

    }
    return -1;
}

static int __count_arr_elems_number(x_array_t* arr) 
{
    int number = 1;
    for (int i=0; i<arr->axisn; ++i) {
        number *= arr->dimens[i];
    }
    return number;
}


static int __do_displaying(x_array_t* arr, char* base, int curr_dimen, int space)
{
    space++;

    if (curr_dimen == arr->axisn-1) {
        float* elems_ptr = base;

        for (int i=0; i<space; ++i) printf("  ");
        printf("[");
        for (int i=0; i<arr->dimens[curr_dimen]; ++i)
        {
            printf("%0.3f, ", elems_ptr[i]);
        }
        printf("]\n");
    } else {

        for (int i=0; i<space; ++i) printf("  ");
        printf("[\n");
        
        int unit_size = __calculate_dimens_unit_size(arr->dimens, arr->axisn, curr_dimen);
        
        for (int i=0; i<arr->dimens[curr_dimen]; ++i) {
            
            __do_displaying(arr, base + (i*unit_size), curr_dimen+1, space);

        }

        for (int i=0; i<space; ++i) printf("  ");
        printf("]\n");

    }
    return 0;
}


static int __do_padding(x_array_t* dest, x_array_t* src, char* dest_base, char* src_base,  int* padding_info, int curr_axis)
{
    int padding_left  = padding_info[curr_axis*2];
    int dest_unit_size = __calculate_dimens_unit_size(dest->dimens, dest->axisn,  curr_axis);
    
    if (curr_axis == dest->axisn-1) {

        // 这是最后一个维度。
        memcpy(dest_base+padding_left*dest_unit_size, src_base, src->dimens[curr_axis] * sizeof(float));
        
    } else {
        
        for (int i=padding_left, j=0; i<(padding_left + src->dimens[curr_axis]); ++i, ++j) {

            int src_unit_size  = __calculate_dimens_unit_size(src->dimens, src->axisn, curr_axis);
            __do_padding(dest, src, dest_base+i*dest_unit_size, src_base+j*src_unit_size, padding_info, curr_axis+1);

        }

    }
    return 0;
}

static int __do_slicing(char** dest, char* base, int* slice_info, x_array_t* src, int curr_axis)
{
    int start     = slice_info[curr_axis*2];
    int open_end  = slice_info[curr_axis*2 +1];
    int unit_size = __calculate_dimens_unit_size(src->dimens, src->axisn, curr_axis);
    
    if (curr_axis == src->axisn-1) {

        int cpy_size = (open_end - start) * unit_size;
        memcpy(*dest, base + start * unit_size, cpy_size);
        *dest += cpy_size;

    } else {

        for (int i=start; i<open_end; ++i) {

            //base += i * unit_size;
            __do_slicing(dest, base + i*unit_size, slice_info, src, curr_axis+1);

        }

    }
    return 0;
}

x_array_t* xarray_create(int axisn, ...)
{
    x_array_t* arr = malloc(sizeof(x_array_t));
    arr->axisn = axisn;
    arr->dimens = malloc(axisn * sizeof (int));
    
    va_list valist;
    va_start(valist, axisn);
    int elems_size = 1;

    for (int i=0; i<axisn; ++i) {
        (arr->dimens)[i] = va_arg(valist, int);
        elems_size *= (arr->dimens)[i];
    }

    elems_size *= sizeof(float);

   arr->elems = malloc (elems_size);
   va_end(valist);
   return arr;
}


int xarray_arange(x_array_t* arr, float start, float open_end)
{
    int elems_number = __count_arr_elems_number(arr);
    float per_value  = (open_end - start) / (float) elems_number;
    float* elems_ptr = arr->elems;
    for (int i=0; i<elems_number; ++i) {
        elems_ptr[i] = start + i * per_value;
    }
    return 0;
}

x_array_t* xarray_slice(x_array_t* arr, ...)
{
    x_array_t* slice_arr = malloc( sizeof (x_array_t));
    slice_arr->axisn = arr->axisn;
    slice_arr->dimens = malloc ( arr->axisn * sizeof(int) );

    int slice_info[arr->axisn * 2];

    va_list valist;
    va_start(valist, arr);

    for (int i=0; i<arr->axisn*2; i += 2) {
        slice_info[i]   = va_arg(valist, int);
        slice_info[i+1] = va_arg(valist, int);
        slice_arr->dimens[i/2] = slice_info[i+1] - slice_info[i];
    }

    int pool_size = __count_arr_elems_number(slice_arr) * sizeof(float);
    slice_arr->elems = malloc (pool_size);
    char* elems_ptr = slice_arr->elems;
    __do_slicing(&elems_ptr, arr->elems, slice_info, arr, 0);
    va_end(valist);
    
    return slice_arr;
}

x_array_t* xarray_padding(x_array_t* arr, float padding_value, ...)
{
    x_array_t* padding_arr = malloc( sizeof (x_array_t));
    padding_arr->axisn = arr->axisn;
    padding_arr->dimens = malloc ( arr->axisn * sizeof (int) );

    int padding_info[arr->axisn * 2];

    va_list valist;
    va_start(valist, padding_value);

    for (int i=0; i<arr->axisn*2; i+=2) {
        padding_info[i]   = va_arg(valist, int);
        padding_info[i+1] = va_arg(valist, int);
        padding_arr->dimens[i/2] = padding_info[i] + padding_info[i+1] + arr->dimens[i/2];
    }

    int padding_elems_number = __count_arr_elems_number(padding_arr);
    padding_arr->elems = malloc (padding_elems_number * sizeof(float));

    float* elems_ptr = padding_arr->elems;
    for (int i=0; i<padding_elems_number; ++i) {
        elems_ptr[i] = padding_value;
    }
    __do_padding(padding_arr, arr, padding_arr->elems, arr->elems, padding_info, 0);

    va_end(valist);
    return padding_arr;
}

int xarray_display(x_array_t* arr)
{
    printf(" shape: <");
    for (int i=0; i<arr->axisn; ++i) {
        printf("%d ", arr->dimens[i]);
    }
    printf(">\n");

    return __do_displaying(arr, arr->elems, 0, 0);
}

int xarray_recycle(x_array_t* arr)
{
    free(arr->dimens);
    free(arr->elems);
    free(arr);
    return 0;
}