/*
 * @Author: your name
 * @Date: 2021-02-25 15:52:31
 * @LastEditTime: 2021-02-26 23:44:00
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/ultra_array/ultra_padding.c
 */
#include <string.h>
#include <stdio.h>
#include "ultra_array.h"
#include "ultra_padding.h"

void UArray_do_filling_pad_data(char* first, char* last, size_t cube_size, ua_pad_width_t* pad_width, ua_pad_mode_t mode)
{
    int l, m, o, p;
    for (l=0, m=1; l<pad_width->before_n; ++l, ++m) {
            
        if (mode == ua_pad_mode_constanst) {

        } else if (mode == ua_pad_mode_edge) {
            memcpy((first - m * cube_size), first, cube_size);
        }
    }

    for (o=0, p=1; o<pad_width->after_n; ++o, ++p) {
        if (mode == ua_pad_mode_constanst) {

        } else if (mode == ua_pad_mode_edge) {
            memcpy((last + p * cube_size), last, cube_size);
        }
    }
    return;
}

void UArray_fill_pad_data(u_array_t* pad_arr, int target_axis, int curr_axis, char* chunk_start, ua_pad_width_t pad_width[], ua_pad_mode_t mode)
{
    if (target_axis == curr_axis) {

        size_t cube_size = UArray_axis_mulitply(pad_arr, curr_axis+1) * sizeof(double);
        char* first = chunk_start + pad_width[curr_axis].before_n * cube_size;
        size_t curr_dimen = UA_shape_axis(pad_arr, curr_axis);
        char* last = first + (curr_dimen - pad_width[curr_axis].before_n - pad_width[curr_axis].after_n - 1) * cube_size;
        UArray_do_filling_pad_data(first, last, cube_size, &pad_width[curr_axis], mode);

    } else {

        size_t cube_size = UArray_axis_mulitply(pad_arr, curr_axis + 1) * sizeof(double);
        size_t curr_dimen = UA_shape_axis(pad_arr, curr_axis);
        size_t fill_number = curr_dimen - pad_width[curr_axis].before_n - pad_width[curr_axis].after_n;
        char* start = chunk_start + cube_size * pad_width[curr_axis].before_n;
        for (int i=0; i<fill_number; ++i) {
            char* __chunk_start = start + i * cube_size;
            UArray_fill_pad_data(pad_arr, target_axis, curr_axis+1, __chunk_start, pad_width, mode);
        }
        
    }
    return;
}

void UArray_cover_pad_width_to_router_str(ua_pad_width_t pad_n[], int pad_n_size, char router_str[])
{
    char start_str[128];
    char tail_str[128];
    int buffer_count = 0;
    int start_str_count = 0;
    int tail_str_count = 0;
    for (int i=0; i<pad_n_size; ++i) {
        ua_pad_width_t pad = pad_n[i];
        int start = pad.before_n;
        int tail  = -1 * pad.after_n;
        sprintf(start_str, "%d", start);
        sprintf(tail_str, "%d", tail);

        while(1) {
            router_str[buffer_count++] = start_str[start_str_count++];
            if (start_str[start_str_count] == '\0') break;
        }
        start_str_count = 0;

        router_str[buffer_count++] = ':';

        while (1){
            router_str[buffer_count++] = tail_str[tail_str_count++];
            if (tail_str[tail_str_count] == '\0') break;
        }
        tail_str_count = 0;
        if (i != pad_n_size -1 ) {
            router_str[buffer_count++] = ',';
        }
    }
}