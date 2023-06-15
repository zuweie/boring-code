/*
 * @Author: your name
 * @Date: 2021-02-09 13:27:15
 * @LastEditTime: 2022-10-14 12:57:22
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/ultra_array/ultra_router.c
 */
#include <stdlib.h>
#include <string.h>
#include "ultra_data_chunk.h"
#include "ultra_array.h"
#include "ultra_router.h"
#include "vtype/vfloat_type.h"
int UArray_survey_chuck_address(u_array_t* arr, char* chunk_start_from, ua_indicator_t* indicator, ua_chunk_note_t* chunk_note) 
{

    size_t sub_chunk_size = (indicator->__axis < UA_axisn(arr) -1  ? UArray_axis_mulitply(arr, indicator->__axis + 1) : 1) * sizeof(vfloat_t);
    int sub_chunk_number = 1;

    if (indicator->next ==  NULL) {
        // 最后一个 route nod         
        size_t offset = 0;

        // 计算下一个维度每一个块的大小。

        if (indicator->__picked == -1) {
            sub_chunk_number = (indicator->__tail <= 0 ? UA_shape_axis(arr, indicator->__axis) + indicator->__tail : indicator->__tail) - indicator->__start;
            offset = indicator->__start * sub_chunk_size;
        } else {
            offset = indicator->__picked * sub_chunk_size;
        }
        
        ua_data_chunk_t* new_chunk = UArray_datachunk_create(chunk_start_from + offset, sub_chunk_size * sub_chunk_number);
        UArray_datachunk_addto(&(chunk_note->chunk_map), new_chunk);
        
    } else {

        if (indicator->__picked == -1) {
            // : 的情况
            int tail = (indicator->__tail <= 0 ? UA_shape_axis(arr, indicator->__axis) + indicator->__tail : indicator->__tail);
            for (int i=indicator->__start; i<tail; ++i) {
                char* sub_chunk_start_from = chunk_start_from + i * sub_chunk_size;
                UArray_survey_chuck_address(arr, sub_chunk_start_from, indicator->next, chunk_note);
            }
        } else {
            // picked 的情况
            char* sub_chunk_start_from = chunk_start_from + indicator->__picked * sub_chunk_size;
            UArray_survey_chuck_address(arr, sub_chunk_start_from, indicator->next, chunk_note);
        }
    }
    return 0;
}

ua_indicator_t* UArray_indicator_create(int axis, int picked, int start, int tail)
{
    ua_indicator_t* index = malloc(sizeof(ua_indicator_t));
    index->__axis = axis;
    index->__picked = picked;
    index->__start = start;
    index->__tail  = tail;
    index->next    = NULL;
    return index;
}
ua_indicator_t* UArray_indicator_init(ua_indicator_t* index, int axis, int picked, int start, int tail) 
{
    
}

void UArray_indicator_addto(ua_indicator_t** indicator_list, ua_indicator_t* indicator) 
{
    if (*indicator_list == NULL) {
        *indicator_list = indicator;
    } else {
        ua_indicator_t* ptr = *indicator_list;
        while (ptr->next != NULL && (ptr = ptr->next));
        ptr->next = indicator;
    }
    return;
}

int UArray_indicator_parse_slicer(int n, ua_slicer_t slicers[], ua_indicator_t** indicator_list) 
{
    *indicator_list = NULL;
    int curr_axis = 0;
    for (int i=0; i<n; ++i) {
        ua_indicator_t* indicator = UArray_indicator_create(curr_axis++, -1, slicers[i].__start, slicers[i].__tail);
        UArray_indicator_addto(indicator_list, indicator);
    }
    return 0;
}

int UArray_indicator_parse_str(char indicator_str[], ua_indicator_t** indicator_list)
{
    *indicator_list = NULL;
    int curr_axis = 0;

    const int BUF_SZ  = 256;
    char buf[BUF_SZ]  = {'\0'};

    int axis_start = 0;
    int axis_tail  = 0;
    int    picked  = -1;

    char *forward, *buf_ptr;
    forward = indicator_str;
    buf_ptr = buf;

    int scope_index = 0;;
    int tail;
    while( *forward != '\0' ) {

        if ( *forward == ',' ) {

            if (buf[0] != '\0') {
                // buf 不为空的情况
                *(buf_ptr+1) = '\0';
                if (scope_index == 0) {
                    picked = atoi(buf);
                    axis_start = 0;
                    axis_tail  = 0;
                } else {
                    picked = -1;
                    axis_tail = atoi(buf);
                }
            } else {
                // buf 为空的情况
                picked = -1;
                if (scope_index == 0) {
                    //'',
                    axis_start = 0;
                } 
                //':'
                axis_tail = 0;
            }

            //buf[0] = '\0';
            memset(buf, 0, strlen(buf));
            buf_ptr = buf;

            ua_indicator_t* indicator = UArray_indicator_create(curr_axis++, picked, axis_start, axis_tail);
            UArray_indicator_addto(indicator_list, indicator);
            scope_index = 0;

        } else if ( *forward == ':' ) {

            if (buf[0] != '\0') {
                *(buf_ptr+1) = '\0';
                axis_start = atoi(buf);
            } else {
                axis_start = 0;
            }
            
            memset(buf, 0, strlen(buf));
            buf_ptr = buf;
            scope_index = 1;

        } else {
            *buf_ptr++ = *forward;
        } 
        forward++;
    }
    //结束后看看还有没有
    if (strlen(buf) != 0) {

        *(buf_ptr + 1) = '\0';

        if ( scope_index == 0 ) {
            picked = atoi(buf);
            axis_start = 0;
            axis_tail  = 0;
        } else {
            picked = -1;
            axis_tail = atoi(buf);
        }
        ua_indicator_t* indicator = UArray_indicator_create(curr_axis++, picked, axis_start, axis_tail);
        UArray_indicator_addto(indicator_list, indicator);

    } else if ( scope_index == 1 ) {
        picked = -1;
        axis_tail = 0;

        ua_indicator_t* indicator = UArray_indicator_create(curr_axis++, picked, axis_start, axis_tail);
        UArray_indicator_addto(indicator_list, indicator);
    }
    return 0;
}


int UArray_indicator_analysis(ua_indicator_t* indicator_list, u_array_t* arr, ua_chunk_note_t* chunk_note) 
{
    chunk_note->shape = NULL;
    chunk_note->axis_n = 0;
    chunk_note->chunk_map = NULL;
    
    ua_indicator_t* ptr = indicator_list;
    int last_axis = -1;
    // 计算总的维数
    while(ptr != NULL) {
        if (ptr->__picked == -1) (chunk_note->axis_n)++;
        last_axis = ptr->__axis;
        ptr = ptr->next;
    }

    if (last_axis >= arr->axis_n) {
        return -1;
    }

    chunk_note->axis_n = chunk_note->axis_n + UA_axisn(arr) - (last_axis+1);

    if (chunk_note->axis_n > 0) 
        chunk_note->shape = malloc( chunk_note->axis_n * sizeof(size_t) );
    else 
        return -1;

    ptr = indicator_list;
    int axis_index = 0;
    while(ptr != NULL) {
        if (ptr->__picked == -1) {
            int tail = (ptr->__tail<=0?UA_shape_axis(arr, ptr->__axis) + ptr->__tail:ptr->__tail);
            (chunk_note->shape)[axis_index++] =  tail - ptr->__start;
        } 
        ptr = ptr->next;
    }

    for (int i = (last_axis+1); i<arr->axis_n; ++i){
        (chunk_note->shape)[axis_index++] = UA_shape_axis(arr, i);
    }

    // -------------------------
    ptr = indicator_list;    
    if (ptr != NULL) {
        UArray_survey_chuck_address(arr, UA_data_ptr(arr), ptr, chunk_note);
    } else {
        chunk_note->chunk_map = UArray_datachunk_create(UA_data_ptr(arr), UA_size(arr));
    }
    return 0;  
}

void UArray_indicator_release(ua_indicator_t* indicator_list)
{
    ua_indicator_t* ptr = indicator_list;
    while (ptr!= NULL) {
         
        ua_indicator_t* node_del = ptr;
        ptr = ptr->next;
        free(node_del);
    }
    return;
}

void UArray_chunk_note_finalize(ua_chunk_note_t* chunk_note) 
{
    if (chunk_note->shape) free(chunk_note->shape);

    UArray_datachunk_release(chunk_note->chunk_map);

    return;
}