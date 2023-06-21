/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-06-19 16:32:16
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-06-21 10:17:20
 * @FilePath: /boring-code/src/statistical_learning/matrix2_statistical.h
 * @Description: 针对 matrix2 的一些统计。
 */

#ifndef __MATRIX2_COUNT_H__
#define __MATRIX2_COUNT_H__
#include "vtype/vfloat_type.h"

#define MAT2_COUNTING_SIZE_PTR(counting_ptr) ((int*)(counting_ptr))
#define MAT2_COUNTING_LIST_PTR(counting_ptr) ((vfloat_t*)( &(MAT2_COUNTING_SIZE_PTR(counting_ptr)[1])))
#define MAT2_COUNTING_NUMBERS_PTR(counting_ptr)  ((int*)(&(MAT2_COUNTING_LIST_PTR(counting_ptr)[*MAT2_COUNTING_SIZE_PTR(counting_ptr)])))

int __mat2_count_element(vfloat_t* in,  int in_size,  void** counting);
int __mat2_get_element_number(void* counting, vfloat_t target);
#endif