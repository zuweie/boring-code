/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-05-24 09:57:43
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-05-31 21:56:33
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_tensor.h
 * @Description: 好难
 */
#ifndef __CG_TENSOR_H__
#define __CG_TENSOR_H__
#include "cg_allocator.h"

#define TENSOR_ELEM_SIZE          sizeof(float)
#define _D_AXES(dimensions)        ((dimensions)[0])
#define _D_DIMEN(dimensions, i)    ((dimensions)[i+1])
#define _D_STRIDE(dimensions, i)   ((dimensions)[_D_AXES(dimensions)+i+1])
#define _D_NUM(dimensions)         _D_DIMEN(dimensions, 0) * _D_STRIDE(dimensions, 0)
#define _D_SIZE(dimensions)        _D_NUM(diemnsions) * TENSOR_ELEM_SIZE

#define TENSOR_AXES(tensor)      _D_AXES((tensor)->dimensions)
#define TENSOR_DIMEN(tensor, i)  _D_DIMEN((tensor)->dimensions, i)
#define TENSOR_STRIDE(tensor, i) _D_STRIDE((tensor)->dimensions, i)
#define TENSOR_NUM(tensor)       (TENSOR_DIMEN(tensor, 0) * TENSOR_STRIDE(tensor, 0))
#define TENSOR_SIZE(tensor)      (TENSOR_NUM(tensor) * TENSOR_ELEM_SIZE)

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