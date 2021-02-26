/*
 * @Author: your name
 * @Date: 2021-02-25 15:52:40
 * @LastEditTime: 2021-02-26 15:53:38
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/ultra_array/ultra_padding.h
 */

#ifndef _ULTRA_PADDING_H_
#define _ULTRA_PADDING_H_
#include <stdlib.h>

typedef struct _ua_pad_width {
    int before_n;
    int after_n;
} ua_pad_width_t;

typedef enum {ua_pad_mode_constanst = 1, ua_pad_mode_edge} ua_pad_mode_t;

void UArray_do_filling_pad_data(char* first, char* last, size_t cube_size, ua_pad_width_t* pad_width, ua_pad_mode_t mode);
void UArray_fill_pad_data(u_array_t* pad_arr, int target_axis, int curr_axis, char* chunk_start, ua_pad_width_t pad_width[], ua_pad_mode_t mode);

#endif