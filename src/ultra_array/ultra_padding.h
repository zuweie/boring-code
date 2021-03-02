/*
 * @Author: your name
 * @Date: 2021-02-25 15:52:40
 * @LastEditTime: 2021-03-02 11:52:22
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/ultra_array/ultra_padding.h
 */

#ifndef _ULTRA_PADDING_H_
#define _ULTRA_PADDING_H_
#include <stdlib.h>

typedef struct _u_array u_array_t;
typedef struct _ua_pad_width ua_pad_width_t;

struct _ua_pad_width {
    int before_n;
    int after_n;
};

typedef enum _ua_pad_mode {ua_pad_mode_constanst = 1, ua_pad_mode_edge} ua_pad_mode_t;

void UArray_do_filling_pad_data(char*, char*, size_t, ua_pad_width_t*, ua_pad_mode_t);
void UArray_fill_pad_data(u_array_t*, int, int, char*, ua_pad_width_t[], ua_pad_mode_t);
void UArray_cover_pad_width_to_router_str(ua_pad_width_t[], int, char[]);
int UArray_parse_pad_width_str(char[], ua_pad_width_t[], int pad_width_size);


#endif