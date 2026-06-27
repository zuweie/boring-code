/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-05-24 09:57:39
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-06-27 13:55:28
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_tensor.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "cg_debug.h"
#include "cg_allocator.h"
#include "cg_tensor_shape.h"
#include "cg_sub_tensor.h"
#include "cg_tensor.h"

static int __reshape(cg_tensor_axis_t** target_shape, char** target_elems, int new_axes, int new_dimensions[], cg_allocator_t* alloc) 
{
    unsigned int old_number = 0;
    
    if (*target_shape) {
        old_number = AXIS_NUMBER(*target_shape);
        cg_tensor_shape_recycle(*target_shape);
    }

    cg_tensor_shape_create(target_shape, new_axes, new_dimensions);

    if (AXIS_NUMBER(*target_shape) > old_number) {
        void* old_elems = *target_elems;
        *target_elems   = cg_alloc(alloc, AXIS_NUMBER(*target_shape) * cg_tensor_elem_size);

        if (old_elems) {
            memcpy(*target_elems, old_elems, old_number * cg_tensor_elem_size);
            cg_recycle(alloc, old_elems);
        }
    }
    
    return 0;
}

static int __batch_match(cg_tensor_t* t1, cg_tensor_t* t2, int t1_batch_start) 
{
    if (AXIS_AXES(t1->shape) >= AXIS_AXES(t2->shape)) {
        
        int axes_diff = AXIS_AXES(t1->shape) - AXIS_AXES(t2->shape);
        int t2_batch_index;
        int t1_batch_index;

        for (t1_batch_index = t1_batch_start; t1_batch_index<=0; --t1_batch_index) {
            t2_batch_index = t1_batch_index - axes_diff;
            if (t2_batch_index >=0 
            && SHAPE_DIMENS(t2->shape, t2_batch_index) != 1 
            && SHAPE_DIMENS(t1->shape, t1_batch_index) != SHAPE_DIMENS(t2->shape, t2_batch_index)) {
                CG_DEBUG("Error <%d@%s>: T1 [%d] dimens is %d, T2 [%d] dimens is %d, does not match\n", \
                    __LINE__, __FILE__,\
                    t1_batch_index, SHAPE_DIMENS(t1->shape, t1_batch_index), t2_batch_index, SHAPE_DIMENS(t2->shape, t2_batch_index));
                return 0;
            } else if (t2_batch_index < 0) {
                // t2 batch index 
                return 1;
            }
        }
        // match all dimens index
        return 1;
    }
    CG_DEBUG("Error: <%d@%s>: T1 AXES(%s) < T2 AXES(%s)\n", __LINE__, __FILE__, AXIS_AXES(t1->shape), AXIS_AXES(t2->shape));
    return 0;
}


static cg_tensor_t* __create_tensor(int axes, int dimensions[], cg_allocator_t* alloc)
{
    cg_tensor_t* tensor = (cg_tensor_t*) malloc(sizeof(cg_tensor_t));
    *tensor = (cg_tensor_t) {
        .allocator  = alloc,
        .shape      = NULL,
        .elems      = NULL
    };
    __reshape(&tensor->shape, (char**)&tensor->elems, axes, dimensions, alloc);
    return tensor;
}

static cg_tensor_t* __create_tensor_cpy(cg_tensor_t* thiz) 
{
    int input_axes = thiz->shape->axes;
    int input_dimens[input_axes];
    cg_tensor_shape_get_dimens(thiz->shape, input_axes, input_dimens);
    cg_tensor_t* tensor = __create_tensor(input_axes, input_dimens, thiz->allocator);
    memcpy(tensor->elems, thiz->elems, TENSOR_SIZE(thiz));
    return tensor;
}

static sub_tensor_t __get_sub_tensor(cg_tensor_t* thiz, int axes, int coord[]) 
{
    sub_tensor_t sub_thiz = cg_tensor_to_sub_tensor(thiz);
    sub_tensor_t sub_dest;
    sub_tensor_get_sub(&sub_dest, &sub_thiz, axes, coord);
    return sub_dest;
}

static int __display_elems(sub_tensor_t* sub_tensor, int space) {

    if (AXIS_AXES(sub_tensor->shape) == 1) {

        for (int k=0; k<2*space; ++k) {
            printf(" ");
        }
        printf("[");
        
        for (int i=0; i<AXIS_DIMENS(sub_tensor->shape); ++i) {
            cg_tensor_elem_display( cg_tensor_elem_offset_addr(sub_tensor->sub_elems, i), "0.2");
        }
        printf("]\n");

    } else {

        sub_tensor_t sub;

        for (int k=0; k<2*space; ++k) {
            printf(" ");
        }
        printf("[ \n");

        for (int i=0; i<AXIS_DIMENS(sub_tensor->shape); ++i) {

            sub_tensor_get_sub(&sub, sub_tensor, 1, (int[]){i});

            __display_elems(&sub, space+1);
        }
        for (int k=0; k<2*space; ++k) {
            printf(" ");
        }
        printf("]\n");

    }
    return 0;
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
    
    return __create_tensor(axes, dimensions, alloc);
}

cg_tensor_t* cg_tensor_create_cpy(cg_tensor_t* thiz) 
{
    return __create_tensor_cpy(thiz);
}

int cg_tensor_recycle(cg_tensor_t* thiz)
{
    cg_recycle(thiz->allocator, thiz->elems);
    cg_tensor_shape_recycle(thiz->shape);
    free(thiz);
    return 0;
}

int cg_tensor_fill(cg_tensor_t* t, cg_tensor_elem_type fill)
{
    sub_tensor_t sub_t1 = cg_tensor_to_sub_tensor(t);
    return sub_tensor_fill(&sub_t1, fill);
}

int cg_tensor_arange(cg_tensor_t* t, cg_tensor_elem_type from, cg_tensor_elem_type to)
{
    sub_tensor_t sub_t1 = cg_tensor_to_sub_tensor(t);
    return sub_tensor_arange(&sub_t1, from, to);
}

int cg_tensor_inspect(cg_tensor_t* t)
{
    sub_tensor_t sub_t = cg_tensor_to_sub_tensor(t);
    
    printf("AXES:%d, SHAPE: ", AXIS_AXES(t->shape));
    for (int i=0; i<AXIS_AXES(t->shape); ++i) {
        printf("%d, ", SHAPE_DIMENS(t->shape, i));
    }
    printf("STRIDE: ");
    for (int i=0; i<AXIS_AXES(t->shape); ++i) {
        printf("%d, ", SHAPE_STRIDE(t->shape, i));
    }
    printf("\n");
    __display_elems(&sub_t,  0);
    return 0;
}

cg_ref_t cg_tensor_get(cg_tensor_t* thiz, ...)
{
    int axes = AXIS_AXES(thiz->shape);
    int coord[axes];
    int coord_invalid = 0;
    va_list vargs;
    va_start(vargs, thiz);
    for (int i=0; i<axes; ++i) {
        coord[i] = va_arg(vargs, int);
        if (coord[i] < 0 && coord[i] >= SHAPE_DIMENS(thiz->shape, i)) {
            CG_DEBUG("Error <%d@%s>: coord[%d](%d) is invalid\n", __LINE__, __FILE__, i, coord[i]);
            coord_invalid = 1;
            break;
        }
    }
    va_end(vargs);

    if (coord_invalid) return NULL;
    sub_tensor_t elem = __get_sub_tensor(thiz, axes, coord);
    return elem.sub_elems;
}



int cg_tensor_T(cg_tensor_t* thiz)
{
    // TODO: do the T of tensor
    if (AXIS_AXES(thiz->shape) != 2) {
        CG_DEBUG("Error <%d@%s>: shape of this tensor is not 2\n", __LINE__, __FILE__);
        return -1;
    }

    cg_tensor_t* cpy_src = __create_tensor_cpy(thiz);

    sub_tensor_t sub_thiz    = cg_tensor_to_sub_tensor(thiz);
    sub_tensor_t sub_cpy_src = cg_tensor_to_sub_tensor(cpy_src);

    int ret = sub_tensor_T(&sub_thiz, &sub_cpy_src);

    cg_tensor_recycle(cpy_src);
    
    return ret;
}

sub_tensor_t cg_tensor_get_sub (cg_tensor_t* thiz, int axes, ...)
{
    if (axes <0 || axes > AXIS_AXES(thiz->shape)) {
        CG_DEBUG("Error <%d@%s>: invalid axes(%d)\n", __LINE__, __FILE__, axes);
        return (sub_tensor_t) {
            .shape = NULL,
            .sub_elems = NULL
        };
    }
    int coord[axes];
    va_list vargs;
    va_start(vargs, axes);
    for (int i=0; i<axes; ++i) {
        coord[i] = va_arg(vargs, int);
    }
    va_end(vargs);

    return __get_sub_tensor(thiz, axes, coord);
}


int cg_tensor_to_tensor(cg_tensor_t* dist, cg_tensor_t* src)
{
    sub_tensor_t sub_dist = cg_tensor_to_sub_tensor(dist);
    sub_tensor_t sub_src  = cg_tensor_to_sub_tensor(src);
    return sub_tensor_to_sub(&sub_dist, &sub_src);
}


int cg_tensor_load(cg_tensor_t* dist, const void* src)
{
    int size = TENSOR_SIZE(dist);
    memcpy(dist->elems, src, size);
    return 0;
}

sub_tensor_t cg_tensor_to_sub_tensor(cg_tensor_t* tensor) 
{
    return (sub_tensor_t) {
        .sub_elems     = tensor->elems, 
        .shape         = tensor->shape
    };
}

cg_tensor_t* cg_tensor_slice(cg_tensor_t* thiz, int slice_axes, ...)
{
    if (slice_axes <=0 && slice_axes > AXIS_AXES(thiz->shape)) {
        CG_DEBUG("Error <%d@%s>: slice_axes(%d) is invalid or slice_axes(%d) is large then thiz axes(%d)\n", \
            __LINE__, __FILE__, slice_axes, slice_axes, AXIS_AXES(thiz->shape));
        return NULL;
    }

    int i;
    int slice[slice_axes<<1];
    int dest_dimens[AXIS_AXES(thiz->shape)];
    int slice_invalid = 0;
    va_list args;
    va_start(args, slice_axes);
    for (i=0; i<slice_axes; ++i) {
        slice[i*2]     = va_arg(args, int);
        slice[i*2+1]   = va_arg(args, int);

        if (slice[i*2]   <0 && slice[i*2]   > SHAPE_DIMENS(thiz->shape, i) 
        &&  slice[i*2+1] <0 && slice[i*2+1] > SHAPE_DIMENS(thiz->shape, i)
        &&  slice[i*2+1] < slice[i*2]
        ) {
            CG_DEBUG("Error <%d@%s>: slice[%d*2](%d) or slice[%i*2+1](%d) is invalid\n", \
                __LINE__, __FILE__, i, slice[i*2], i, slice[i*2+1]);

            slice_invalid = 1;
            break;
        } 
        dest_dimens[i] = slice[i*2+1] - slice[i*2];
    }
    va_end(args);
    

    
    if (!slice_invalid) {

        for (;i<AXIS_AXES(thiz->shape); ++i) {
            dest_dimens[i] = SHAPE_DIMENS(thiz->shape, i);
        }

        cg_tensor_t* dest = __create_tensor(AXIS_AXES(thiz->shape), dest_dimens, thiz->allocator);
    
        sub_tensor_t sub_dest  = cg_tensor_to_sub_tensor(dest);
        sub_tensor_t sub_src   = cg_tensor_to_sub_tensor(thiz);

        int ret = sub_tensor_slice(&sub_dest, &sub_src, slice_axes, slice);

        // slice 失败了。回收 dest。
        if (ret == -1) cg_tensor_recycle(dest);

        return !ret ? dest : NULL;
    }
    return NULL;

}

cg_tensor_t* cg_tensor_padding(cg_tensor_t* thiz, padding_mode_t mode, cg_tensor_elem_type fill, int padding_axes, ...)
{
    if (padding_axes > AXIS_AXES(thiz->shape)) {
        CG_DEBUG("Error <%d@%s>: padding_axes(%d) is large then thiz axes(%d)\n", \
            __LINE__, __FILE__, padding_axes, AXIS_AXES(thiz->shape) \
        );
        return NULL;
    }

    int i;
    int padding[padding_axes<<1];
    int dest_dimens[AXIS_AXES(thiz->shape)];
    int padding_invalid = 0;
    va_list args;
    va_start(args, padding_axes);
    for (i=0; i<padding_axes; ++i) {
        padding[i*2]     = va_arg(args, int);
        padding[i*2+1]   = va_arg(args, int);
        if (padding[i*2] < 0 && padding[i*2+1] < 0) {
            CG_DEBUG("Error <%d@%s>: padding[%d*2](%d) or padding[%d*2+1](%d) is invalid\n", \
                __LINE__, __FILE__, i, padding[i*2], i, padding[i*2+1] \
            );
            padding_invalid = 1;
            break;
        }
        dest_dimens[i]   = SHAPE_DIMENS(thiz->shape, i) + padding[i*2] + padding[i*2+1];
    }
    va_end(args);

    if (!padding_invalid) {

        for (; i<AXIS_AXES(thiz->shape); ++i) {
            dest_dimens[i] = SHAPE_DIMENS(thiz->shape, i);
        }

        cg_tensor_t* dest = __create_tensor(AXIS_AXES(thiz->shape), dest_dimens, thiz->allocator);
        
        sub_tensor_t sub_dest = cg_tensor_to_sub_tensor(dest);
        sub_tensor_t sub_src  = cg_tensor_to_sub_tensor(thiz);
    
        int ret = sub_tensor_padding(&sub_dest, &sub_src, padding_axes, padding, mode, fill);
        if (ret == -1) cg_tensor_recycle(dest);
        return !ret ? dest : NULL;
    }
    return NULL;

}
