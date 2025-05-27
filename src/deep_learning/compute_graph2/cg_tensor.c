#include <stdlib.h>
#include <stdarg.h>
#include "cg_debug.h"
#include "cg_tensor.h"

cg_tensor_t* cg_tensor_create(cg_allocator_t* alloc, int axes, ...)
{
    cg_tensor_t* tensor = (cg_tensor_t*) malloc(sizeof(cg_tensor_t) + sizeof(int) * axes);
    tensor->allocator   = alloc;
    tensor->axes        = axes;

    va_list args;
    va_start(args, axes);
    int elems_size = 1;

    for (int i=0; i<axes; ++i) {
        tensor->dimens[i] = va_arg(args, int);
        elems_size       *= tensor->dimens[i];
    }
    tensor->elems = cg_alloc(alloc, elems_size * sizeof(float));
    
    va_end(args);
    return tensor;
}

int cg_tensor_destroy(cg_tensor_t* thiz)
{
    cg_recycle(thiz->allocator, thiz->elems);
    free(thiz);
    return 0;
}