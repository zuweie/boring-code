/*
 * @Author: your name
 * @Date: 2021-01-14 08:46:38
 * @LastEditTime: 2021-04-09 09:26:49
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/cosine_Transform/cosine_transform.h
 */

#ifndef _COSINE_TRANSFORM_H_
#define _COSINE_TRANSFORM_H_
#include "vtype/vfloat_type.h"

typedef enum{dct_none = 0, dct_ortho =1} dct_norm_t;
typedef enum{dct_i = 1, dct_ii, dct_iii} dct_type_t;

vfloat_t K_cosine_transform_i(int k, int N, vfloat_t sequence[], dct_norm_t norm);
vfloat_t K_cosine_transform_ii(int k, int N, vfloat_t sequence[], dct_norm_t norm);
vfloat_t k_cosine_transform_iii(int k, int N, vfloat_t sequence[], dct_norm_t norm);

int Discrete_cosine_transform(vfloat_t sequence[], int N, int cal_n, vfloat_t out[], dct_norm_t dct_norm, dct_type_t dct_type);

#endif