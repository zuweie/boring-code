/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-06-19 16:32:16
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-06-20 14:53:38
 * @FilePath: /boring-code/src/statistical_learning/matrix2_statistical.h
 * @Description: 针对 matrix2 的一些统计。
 */

#ifndef __MATRIX2_COUNT_H__
#define __MATRIX2_COUNT_H__
#include "vtype/vfloat_type.h"

#define MAX2_DIFF_SIZE_PTR(diff_ptr) ((int*)(diff_ptr))
#define MAX2_DIFF_LIST_PTR(diff_ptr) ((vfloat_t*)( &(MAX2_DIFF_SIZE_PTR(diff_ptr)[1])))
#define MAX2_DIFF_NUMBERS_PTR(diff_ptr)  ((int*)(&(MAX2_DIFF_LIST_PTR(diff_ptr)[*MAX2_DIFF_SIZE_PTR(diff_ptr)])))

int __mat2_list_different(vfloat_t* in,  int in_size,  void** out1);
int __mat2_get_diff_number(void* diff, vfloat_t target);
#endif