/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-05-24 09:57:39
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-05-30 12:28:19
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_tensor.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "cg_debug.h"
#include "cg_tensor.h"

static float* __coordinate_router(cg_tensor_t* tensor, int axes, int coordinate[]) 
{
    float* dist = tensor->elems;
    for (int i=0; i<axes; ++i) {
        dist += coordinate[i] * TENSOR_STRIDE(tensor, i)
    }
    return dist, 
}

static int __reshape(cg_tensor_t* tensor, int axes, int dimensions[]) 
{
    unsigned int old_size = 0;
    if (tensor->dimensions == NULL) { 
        tensor->dimensions = (int*) malloc ((1+2*axes) * sizeof(int));
    } else if (axes > TENSOR_AXES(tensor)) {
        old_size = TENSOR_SIZE(tensor);
        tensor->dimensions = (int*) realloc ((1+2*axes) * sizeof(int));
    }

    TENSOR_AXES(tensor) = axes;
    // 更新维度信息。
    for (int i=axes-1; i>=0 ; --i) {
        TENSOR_DIMEN(tensor, i)  = dimensions[i];
        TENSOR_STRIDE(tensor, i) = (i == axes-1 ? 1 : TENSOR_DIMEN(tensor, (i+1)) * TENSOR_STRIDE(tensor, (i+1)));
    }

    // 跟新 element 池子大小 
    if (TENSOR_SIZE(tensor) > old_size) {

        float* new_elemes = cg_alloc(tensor->allocator, TENSOR_SIZE(tensor));

        // 如果原来旧有数据复制旧的，然后回收。
        if (tensor->elems) {
            memcpy(new_elemes, tensor->elems, old_size);
            cg_recycle(tensor->allocator, tensor->elems);
        }
        tensor->elems = new_elemes;
    }
    
    return 0;
}
cg_tensor_t* __create_tensor(cg_allocator_t* alloc, int axes, int dimension[])
{
    cg_tensor_t* tensor = (cg_tensor_t*) malloc(sizeof(cg_tensor_t) + sizeof(int) * axes);
    tensor->allocator   = alloc;
    tensor->dimensions  = NULL;
    tensor->elems       = NULL;
    __reshape(tensor, axes, dimension);
    return tensor;
}

cg_tensor_t* cg_tensor_create(cg_allocator_t* alloc, int axes, ...)
{
    int dimensions[axes];

    va_list args;
    va_start(args, axes);
    for (int i=0; i<axes; ++i) {
        dimensions[i] = va_arg(args, int);
    }
    va_end(args);
    
    return __create_tensor(alloc, axes, dimensions);
}

int cg_tensor_cycle(cg_tensor_t* thiz)
{
    cg_recycle(thiz->allocator, thiz->elems);
    free(thiz->dimensions);
    free(thiz);
    return 0;
}


cg_tensor_t* cg_tensor_slice(cg_tensor_t* thiz, int slice_axes, ...)
{
    int slice_dimensions[ 2 * TENSOR_AXES(thiz) ];
    int new_dimensions[TENSOR_AXES(thiz)];

    va_list vargs;
    va_start(vargs, slice_axes);
    for (i=0; i<TENSOR_AXES(thiz); ++i) {
        slice_dimensions[i*2]   = i<slice_axes? va_arg(args, int) : 0;
        slice_dimensions[i*2+1] = i<slice_axes? va_arg(args, int) : TENSOR_DIMEN(this, i);
        new_dimensions[i] = slice_dimensions[i*2+1] - slice_dimensions[i*2];
    }
    va_end(vargs);

    cg_tensor_t* tensor = __create_tensor(thiz->alloc, TENSOR_AXES(this), new_dimensions);

    // copy 数据
     
    return tensor;
}

cg_tensor_t* cg_tensor_padding(cg_tensor_t* this, int axes, ...);
{

}

int cg_tensor_dot(cg_tensor_t* r, cg_tensor_t* t1, cg_tensor_t* t2)
int cg_tensor_sum(cg_tensor_t* r, cg_tensor_t* t1, cg_tensor_t* t2)
int cg_tensor_subtract(cg_tensor_t* r, cg_tensor_t* t1, cg_tensor_t* t2)
int cg_tensor_scale(cg_tensor_t*, float scale)
