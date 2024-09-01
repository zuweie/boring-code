/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-10-11 15:08:05
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2024-08-31 10:28:04
 * @FilePath: /boring-code/src/ultra_array/x_array.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __TENSOR_H__
#define __TENSOR_H__

typedef struct 
{
    /* data */
    int axisn;
    int* dimens;
    char* elems;
    int data_size;

} tensor_t;

tensor_t* tensor_create(int axisn, ...);
tensor_t* tensor_slice(tensor_t* arr, ...);
tensor_t* tensor_padding(tensor_t* arr, float padding_to, ...);
int tensor_arange(tensor_t* arr, float start, float end);
int tensor_display(tensor_t* arr);
int tensor_recycle(tensor_t* arr);

#endif