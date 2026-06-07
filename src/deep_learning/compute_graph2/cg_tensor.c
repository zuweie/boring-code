/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-05-24 09:57:39
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-06-07 22:25:28
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_tensor.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "cg_debug.h"
#include "cg_allocator.h"
#include "cg_sub_tensor.h"
#include "cg_tensor.h"

static int __reshape(char** target_elems, int** target_dimens, int new_axes, int new_dimensions[], cg_allocator_t* alloc) 
{
    unsigned int old_size = 0;

    // delete 这里我将加多一维，第 0 维。例如 axes 为 3。 那么的有 0，1，2，3个轴。第三个轴是第 0 维。
    // 经过全新设计，这里多加一轴的设计去掉。
    if (*target_dimens == NULL) {
        //*target_dimens = (int*) malloc (((new_axes+1)*2+1) * sizeof(int));
        *target_dimens = (int*) malloc ((new_axes * 2 + 1) * sizeof (int));
    } else {
        old_size = _D_NUM(*target_dimens) * cg_tensor_elem_size;
        if (_D_AXES(*target_dimens) < new_axes) 
            //(*target_dimens) = (int*)realloc((*target_dimens), ((new_axes+1)*2+1) * sizeof(int));
            (*target_dimens) = (int*) realloc((*target_dimens), ((new_axes*2+1) * sizeof(int)));
    }

    // dimens 第一位放入轴数。
    _D_AXES(*target_dimens) = new_axes;

    // 更新维度信息。
    // 最后一轴的 stride 是 1，因为是单元 elem，0 轴，0维（是一个点）。
    _D_DIMEN(*target_dimens,  new_axes-1)  = new_dimensions[new_axes-1];
    _D_STRIDE(*target_dimens, new_axes-1)  = 1;
    
     for (int i=new_axes-2; i>=0 ; --i) {
        _D_DIMEN(*target_dimens,  i)  = new_dimensions[i];
        _D_STRIDE(*target_dimens, i)  = _D_DIMEN(*target_dimens, i+1) * _D_STRIDE(*target_dimens, i+1);
    }

    // 更新 element 池子大小 
    if (_D_NUM(*target_dimens) * cg_tensor_elem_size > old_size) {

        void *old_elems = *target_elems;
        *target_elems   = cg_alloc(alloc, _D_NUM(*target_dimens) * cg_tensor_elem_size);

        // 如果原来旧有数据复制旧的，然后回收。
        if (old_elems) {
            memcpy(*target_dimens, old_elems, old_size);
            cg_recycle(alloc, old_elems);
        }
    }
    
    return 0;
}

static int __batch_match(cg_tensor_t* t1, cg_tensor_t* t2, int t1_batch_start) 
{
    if (TENSOR_AXES(t1) >= TENSOR_AXES(t2)) {
        
        int axes_diff = TENSOR_AXES(t1) - TENSOR_AXES(t2);
        int t2_batch_index;
        int t1_batch_index;
        for (t1_batch_index = t1_batch_start; t1_batch_index<=0; --t1_batch_index) {
            t2_batch_index = t1_batch_index - axes_diff;
            if (t2_batch_index >=0 
            && TENSOR_DIMEN(t2, t2_batch_index) != 1 
            && TENSOR_DIMEN(t1, t1_batch_index) != TENSOR_DIMEN(t2, t2_batch_index)) {
                CG_DEBUG("T1 [%d] dimen is %d, T2 [%d] dimen is %d, does not match\n", \
                    t1_batch_index, TENSOR_DIMEN(t1, t1_batch_index), t2_batch_index, TENSOR_DIMEN(t2, t2_batch_index));
                return 0;
            } else if (t2_batch_index < 0) {
                // t2 batch index 
                return 1;
            }
        }
        // match all dimens index
        return 1;
    }
    CG_DEBUG("T1 AXES(%s) < T2 AXES(%s)\n", TENSOR_AXES(t1), TENSOR_AXES(t2));
    return 0;
}


static cg_tensor_t* __create_tensor(int axes, int dimensions[], cg_allocator_t* alloc)
{
    cg_tensor_t* tensor = (cg_tensor_t*) malloc(sizeof(cg_tensor_t));
    *tensor = (cg_tensor_t) {
        .allocator  = alloc,
        .dimensions = NULL,
        .elems      = NULL
    };
    __reshape((char**)&tensor->elems, (int**)&tensor->dimensions, axes, dimensions, alloc);
    return tensor;
}

static cg_tensor_t* __create_tensor_cpy(cg_tensor_t* thiz) 
{
    cg_tensor_t* tensor = __create_tensor(TENSOR_AXES(thiz), &TENSOR_DIMEN(thiz, 0), thiz->allocator);
    memcpy(tensor->elems, thiz->elems, TENSOR_SIZE(tensor));
    return tensor;
}

static sub_tensor_t __get_sub_tensor(cg_tensor_t* thiz, int axes, int coord[]) 
{
    sub_tensor_t sub_thiz = cg_tensor_to_sub_tensor(thiz);
    sub_tensor_t sub_dest;
    sub_tensor_get_sub(&sub_dest, &sub_thiz, axes, coord);
    return sub_dest;
}

static int __display_elems(cg_tensor_t* t, int curr_axis, int coord[]) {

    if (curr_axis == TENSOR_AXES(t) - 2) {

        coord[curr_axis]  = 0;
        int axes          = curr_axis + 1;
        sub_tensor_t elem = __get_sub_tensor(t, curr_axis, coord);
        cg_tensor_elem_type* pelem = elem.sub_elems;

        for (int k=0; k<2*curr_axis; ++k) {
            printf(" ");
        }
        printf("[");
        for (int i=0; i<elem.sub_dimens[0]; ++i) {
            cg_tensor_elem_display(&pelem[i], "0.2");
            if (i != elem.sub_dimens[0] -1 ){
                printf(",");
            } 
        }
        printf(" ]\n");

    } else {

        for (int k=0; k<2*curr_axis; ++k) {
            printf(" ");
        }
        printf("[ \n");
        for (int i=0; i<TENSOR_DIMEN(t, curr_axis); ++i) {
            coord[curr_axis] = i;
            __display_elems(t, curr_axis + 1, coord);
        }
        for (int k=0; k<2*curr_axis; ++k) {
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
    free(thiz->dimensions);
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
    printf("AXES:%d, SHAPE: ", TENSOR_AXES(t));
    for (int i=0; i<TENSOR_AXES(t)+1; ++i) {
        printf("%d, ", TENSOR_DIMEN(t, i));
    }
    printf("STRIDE: ");
    for (int i=0; i<TENSOR_AXES(t)+1; ++i) {
        printf("%d, ", TENSOR_STRIDE(t, i));
    }
    printf("\n");
    int coord[TENSOR_AXES(t)];
    __display_elems(t, 0, coord);
    return 0;
}

cg_ref_t cg_tensor_get(cg_tensor_t* thiz, ...)
{
    int axes = TENSOR_AXES(thiz);
    int coord[axes];
    int coord_invalid = 0;
    va_list vargs;
    va_start(vargs, thiz);
    for (int i=0; i<axes; ++i) {
        coord[i] = va_arg(vargs, int);
        if (coord[i] < 0 && coord[i] >= TENSOR_DIMEN(thiz, i)) {
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
    if (TENSOR_AXES(thiz) != 2) {
        CG_DEBUG("Error <%d@%s>: shape of thiz is not 2\n", __LINE__, __FILE__);
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
    if (axes <0 || axes > TENSOR_AXES(thiz)) {
        CG_DEBUG("Error <%d@%s>: invalid axes(%d)\n", __LINE__, __FILE__, axes);
        return (sub_tensor_t) {
            .sub_axes = -1,
            .sub_dimens = NULL,
            .sub_stride = NULL,
            .sub_elems  = NULL
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
        .sub_axes      = TENSOR_AXES(tensor), 
        .sub_dimens    = &TENSOR_DIMEN(tensor,0), 
        .sub_stride    = &TENSOR_STRIDE(tensor, 0)
    };
}

cg_tensor_t* cg_tensor_slice(cg_tensor_t* thiz, int slice_axes, ...)
{
    if (slice_axes <=0 && slice_axes > TENSOR_AXES(thiz)) {
        CG_DEBUG("Error <%d@%s>: slice_axes(%d) is invalid or slice_axes(%d) is large then thiz axes(%d)\n", \
            __LINE__, __FILE__, slice_axes, slice_axes, TENSOR_AXES(thiz));
        return NULL;
    }

    int i;
    int slice[slice_axes<<1];
    int dest_dimens[TENSOR_AXES(thiz)];
    int slice_invalid = 0;
    va_list args;
    va_start(args, slice_axes);
    for (i=0; i<slice_axes; ++i) {
        slice[i*2]     = va_arg(args, int);
        slice[i*2+1]   = va_arg(args, int);

        if (slice[i*2]   <0 && slice[i*2]   > TENSOR_DIMEN(thiz, i) 
        &&  slice[i*2+1] <0 && slice[i*2+1] > TENSOR_DIMEN(thiz, i)
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

        for (;i<TENSOR_AXES(thiz) ; ++i) {
            dest_dimens[i] = TENSOR_DIMEN(thiz, i);
        }

        cg_tensor_t* dest = __create_tensor(TENSOR_AXES(thiz), dest_dimens, thiz->allocator);
    
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
    if (padding_axes > TENSOR_AXES(thiz)) {
        CG_DEBUG("Error <%d@%s>: padding_axes(%d) is large then thiz axes(%d)\n", \
            __LINE__, __FILE__, padding_axes, TENSOR_AXES(thiz)-1 \
        );
        return NULL;
    }

    int i;
    int padding[padding_axes<<1];
    int dest_dimens[TENSOR_AXES(thiz)];
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
        dest_dimens[i]   = TENSOR_DIMEN(thiz, i) + padding[i*2] + padding[i*2+1];
    }
    va_end(args);

    if (!padding_invalid) {

        for (; i<TENSOR_AXES(thiz); ++i) {
            dest_dimens[i] = TENSOR_DIMEN(thiz, i);
        }

        cg_tensor_t* dest = __create_tensor(TENSOR_AXES(thiz), dest_dimens, thiz->allocator);
        
        sub_tensor_t sub_dest = cg_tensor_to_sub_tensor(dest);
        sub_tensor_t sub_src  = cg_tensor_to_sub_tensor(thiz);
    
        int ret = sub_tensor_padding(&sub_dest, &sub_src, padding_axes, padding, mode, fill);
        if (ret == -1) cg_tensor_recycle(dest);
        return !ret ? dest : NULL;
    }
    return NULL;

}
