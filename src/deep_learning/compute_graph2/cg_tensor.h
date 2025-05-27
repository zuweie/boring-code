#ifndef __CG_TENSOR_H__
#define __CG_TENSOR_H__
#include "cg_allocator.h"

typedef struct cg_tensor {
    cg_allocator_t* allocator;
    int   axes;
    char* elems;
    int dimens[0];
} cg_tensor_t;

cg_tensor_t* cg_tensor_create(cg_allocator_t* alloc, int axes, ...);
int cg_tensor_destroy(cg_tensor_t* thiz);

#endif