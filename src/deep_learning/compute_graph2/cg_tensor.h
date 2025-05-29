#ifndef __CG_TENSOR_H__
#define __CG_TENSOR_H__
#include "cg_allocator.h"

typedef struct cg_tensor {
    cg_allocator_t* allocator;
    int   axes;
    float* elems;
    int dimens[0];
} cg_tensor_t;

cg_tensor_t* cg_tensor_create(cg_allocator_t* alloc, int axes, ...);
int cg_tensor_recycle(cg_tensor_t* thiz);

int cg_tensor_slice(cg_tensor_t* thiz, ...)
int cg_tensor_padding(cg_tensor_t* this, ...)

int cg_tensor_dot(cg_tensor_t* r, cg_tensor_t* t1, cg_tensor_t* t2)
int cg_tensor_sum(cg_tensor_t* r, cg_tensor_t* t1, cg_tensor_t* t2)
int cg_tensor_subtract(cg_tensor_t* r, cg_tensor_t* t1, cg_tensor_t* t2)
int cg_tensor_scale(cg_tensor_t*, float scale)


#endif