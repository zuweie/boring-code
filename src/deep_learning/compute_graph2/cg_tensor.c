/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-05-24 09:57:39
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-05-29 16:07:11
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_tensor.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
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
    tensor->elems = (float*)cg_alloc(alloc, elems_size * sizeof(float));
    
    va_end(args);
    return tensor;
}

int cg_tensor_cycle(cg_tensor_t* thiz)
{
    cg_recycle(thiz->allocator, thiz->elems);
    free(thiz);
    return 0;
}