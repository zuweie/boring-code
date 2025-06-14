/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-05-24 09:57:43
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-06-07 21:08:36
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_tensor.h
 * @Description: 好难
 */
#ifndef __CG_TENSOR_H__
#define __CG_TENSOR_H__
#include "cg_allocator.h"

#define TENSOR_ELEM_SIZE          sizeof(float)
#define _D_AXES(dimensions)        ((dimensions)[0])
#define _D_DIMEN(dimensions, i)    ((dimensions)[i+1])
#define _D_STRIDE(dimensions, i)   ((dimensions)[_D_AXES(dimensions)+i+2])
#define _D_NUM(dimensions)         _D_DIMEN(dimensions, 0) * _D_STRIDE(dimensions, 0)
#define _D_SIZE(dimensions)        _D_NUM(dimensions) * TENSOR_ELEM_SIZE

#define TENSOR_AXES(tensor)      _D_AXES((tensor)->dimensions)
#define TENSOR_DIMEN(tensor, i)  _D_DIMEN((tensor)->dimensions, i)
#define TENSOR_STRIDE(tensor, i) _D_STRIDE((tensor)->dimensions, i)
#define TENSOR_NUM(tensor)        (TENSOR_DIMEN(tensor, 0) * TENSOR_STRIDE(tensor, 0))
#define TENSOR_SIZE(tensor)      (TENSOR_NUM(tensor) * TENSOR_ELEM_SIZE)

typedef struct cg_tensor {
    cg_allocator_t* allocator;
    void*           elems;
    int*            dimensions;
} cg_tensor_t;

typedef struct __sub_tensor {
    void* sub_elems;
    int*  sub_stride;
    int*  sub_dimens;
    int   sub_axes;
} __sub_tensor_t;

cg_tensor_t* cg_tensor_create(cg_allocator_t* alloc, int axes, ...);
cg_tensor_t* cg_tensor_create_cpy(cg_tensor_t* thiz);
int cg_tensor_recycle(cg_tensor_t* thiz);

cg_tensor_t* cg_tensor_slice(cg_tensor_t* thiz, int axes, ...);
cg_tensor_t* cg_tensor_padding(cg_tensor_t* this, float fill, int padding_axes, ...);

int cg_tensor_dot(cg_tensor_t*, cg_tensor_t*, cg_tensor_t* );
int cg_tensor_sum(cg_tensor_t* , cg_tensor_t* , cg_tensor_t* );
int cg_tensor_subtract(cg_tensor_t* , cg_tensor_t* , cg_tensor_t* );
int cg_tensor_scale(cg_tensor_t*, float);
int cg_tensor_fill(cg_tensor_t*, float);
int cg_tensor_arange(cg_tensor_t*, float, float);
int cg_tensor_inspect(cg_tensor_t*);
float* cg_tensor_get(cg_tensor_t*, ...);
__sub_tensor_t cg_tensor_get_sub (cg_tensor_t*, int axes, int* coord);
int cg_tensor_sub_to_sub(__sub_tensor_t* dist, const __sub_tensor_t* src);
int cg_tensor_set(cg_tensor_t*, float val, ...);
int cg_tensor_T(cg_tensor_t*, ...);

#endif