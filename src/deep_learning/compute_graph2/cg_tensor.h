/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-05-24 09:57:43
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-05-30 11:44:09
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_tensor.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE  
 */
#ifndef __CG_TENSOR_H__
#define __CG_TENSOR_H__
#include "cg_allocator.h"

#define TENSOR_AXES(tensor)      ((tensor)->dimension[0])
#define TENSOR_DIMEN(tensor, i)  ((tensor)->dimension[i+1])
#define TENSOR_STRIDE(tensor, i) ((tensor)->dimension[TENSOR_AXES[(tensor)]+1+i])
#define TENSOR_NUM(tensor)  (TENSOR_DIMEN(tensor, 0) * TENSOR_STRIDE(tensor, 0))

#define TENSOR_SIZE(tensor) (TENSOR_NUM(tensor) * sizeof(float))

typedef struct cg_tensor {
    cg_allocator_t* allocator;
    float*          elems;
    int*            dimensions;
} cg_tensor_t;

cg_tensor_t* cg_tensor_create(cg_allocator_t* alloc, int axes, ...);
int cg_tensor_recycle(cg_tensor_t* thiz);

cg_tensor_t* cg_tensor_slice(cg_tensor_t* thiz, int axes, ...);
cg_tensor_t* cg_tensor_padding(cg_tensor_t* this, int axes, ...);

int cg_tensor_dot(cg_tensor_t* r, cg_tensor_t* t1, cg_tensor_t* t2);
int cg_tensor_sum(cg_tensor_t* r, cg_tensor_t* t1, cg_tensor_t* t2);
int cg_tensor_subtract(cg_tensor_t* r, cg_tensor_t* t1, cg_tensor_t* t2);
int cg_tensor_scale(cg_tensor_t*, float scale);

#endif