/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-05-24 09:57:39
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-05-16 15:53:08
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_tensor.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "cg_debug.h"
#include "cg_sub_tensor.h"
#include "cg_tensor.h"

static int __reshape(char** target_elems, int** target_dimens, int new_axes, int new_dimensions[], cg_allocator_t* alloc) 
{
    unsigned int old_size = 0;
    // 这里我将加多一维，第 0 维。例如 axes 为 3。 那么的有 0，1，2，3个轴。第三个轴是第 0 维。
    if (*target_dimens == NULL) {
        *target_dimens = (int*) malloc (((new_axes+1)*2+1) * sizeof(int));
    } else {
        old_size = _D_NUM(*target_dimens) * tensor_elem_size;
        if (_D_AXES(*target_dimens) < new_axes) 
            (*target_dimens) = (int*)realloc((*target_dimens), ((new_axes+1)*2+1) * sizeof(int));
    }

    _D_AXES(*target_dimens) = new_axes;

    // 更新维度信息。
    // 第 0 维存入1，1。
    _D_DIMEN(*target_dimens, new_axes)  = 1;
    _D_STRIDE(*target_dimens, new_axes) = 1;
    
     for (int i=new_axes-1; i>=0 ; --i) {

        _D_DIMEN(*target_dimens, i)   = new_dimensions[i];
        _D_STRIDE(*target_dimens, i)  = _D_DIMEN(*target_dimens, i+1) * _D_STRIDE(*target_dimens, i+1);
    }

    // 更新 element 池子大小 
    if (_D_NUM(*target_dimens) * tensor_elem_size > old_size) {

        void *old_elems = *target_elems;
        *target_elems   = cg_alloc(alloc, _D_NUM(*target_dimens) * tensor_elem_size);

        // 如果原来旧有数据复制旧的，然后回收。
        if (old_elems) {
            memcpy(*target_dimens, old_elems, old_size);
            cg_recycle(alloc, old_elems);
        }
    }
    
    return 0;
}

static int __batch_match(const cg_tensor_t* t1, const cg_tensor_t* t2, int t1_batch_start) 
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
    }
    __reshape(&tensor->elems, &tensor->dimensions, sepc->elem_size, axes, dimensions, alloc);
    return tensor;
}

static cg_tensor_t* __create_tensor_cpy(const cg_tensor_t* thiz) 
{
    cg_tensor_t* tensor = __create_tensor(TENSOR_AXES(thiz), &TENSOR_DIMEN(thiz, 0), thiz->allocator);
    memcpy(tensor->elems, thiz->elems, TENSOR_SIZE(tensor));
    return tensor;
}



// 一种新的 padding 算法, 复杂而又巧妙，简直就是艺术品的算法。nice
static int __do_padding(const sub_tensor_t* dist, const sub_tensor_t* src, float fill, int* padding, int curr_axis) 
{

    int i, j;

    int sub_dist_index[1];
    int sub_src_index[1];

    int padding_left_start = 0;
    int padding_left_end   = padding[curr_axis * 2];

    int padding_middle_start = padding_left_end;
    int padding_middle_end   = padding[curr_axis * 2] + src->sub_dimens[0];

    int padding_right_start = padding_middle_end;
    int padding_right_end   = padding[curr_axis * 2] + src->sub_dimens[0] + padding[curr_axis * 2 + 1];

    // fill the left part
    for (i = padding_left_start; i < padding_left_end; ++i)
    {
        sub_dist_index[0] = i;
        //sub_tensor_t sub_dist = __get_sub_tensor(dist, 1, sub_dist_index);
        sub_tensor_t sub_dist = sub_tensor_get_sub(dist, 1, sub_dist_index);
        if (sub_dist.sub_axes == 0) {
            // 只有一个元素，那么直接赋值
            *((float*)sub_dist.sub_elems) = fill;
        } else {
            sub_tensor_opt_scalar(&sub_dist, fill, __assign_scalar);
        }
    }

    // fill the right part
    for (i = padding_right_start; i < padding_right_end; ++i)
    {
        // fill
        sub_dist_index[0] = i;
        sub_tensor_t sub_dist = sub_tensor_get_sub(dist, 1, sub_dist_index);
        if (sub_dist.sub_axes == 0) {
            // 只有一个元素，那么直接赋值
            *((float*)sub_dist.sub_elems) = fill;
        } else {
            sub_tensor_opt_scalar(&sub_dist, fill, __assign_scalar);
        }
    }

    for (i = padding_middle_start; i < padding_middle_end; ++i)
    {
        //
        sub_dist_index[0] = i;
        sub_src_index[0]  = sub_dist_index[0] - padding[curr_axis * 2];

        sub_tensor_t sub_dist = sub_tensor_get_sub(dist, 1, sub_dist_index);
        sub_tensor_t sub_src  = sub_tensor_get_sub(src,  1, sub_src_index);

        if (sub_dist.sub_axes == 0){
            // 单个元素直接 copy
            float *p_dist = sub_dist.sub_elems;
            float *p_src  = sub_src.sub_elems;
            *p_dist = *p_src;
        }
        else{
            __do_padding(&sub_dist, &sub_src, fill, padding, curr_axis + 1);
        }
    }
    return 0;
}


static int __display_elems(cg_tensor_t* t, int curr_axis, int coord[]) {

    if (curr_axis == TENSOR_AXES(t) -1) {

        coord[curr_axis] = 0;
        int axes         = curr_axis + 1;
        float* p_elem    = __coordinate_router(t->elems, &TENSOR_STRIDE(t, 0), axes, coord);
        for (int k=0; k<2*curr_axis; ++k) {
            printf(" ");
        }
        printf("[");
        for (int i=0; i<TENSOR_DIMEN(t, curr_axis); ++i) {
            if (i == TENSOR_DIMEN(t, curr_axis) - 1)
                printf("%0.2f", p_elem[i]);
            else
                printf("%0.2f, ", p_elem[i]);
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


int cg_tensor_arange(cg_tensor_t* t, cg_tensor_elem_type from, cg_tensor_elem_type to)
{
    char* p_t = t->elems;
    //float  per = (to - from) / TENSOR_NUM(t);
    cg_var_def(per);
    cg_var_def(gap);

    // per = to - from
    cg_tensor_elem_number_opt(per, to, from, -);
    // per = pre / TENSOR_NUM(t)
    cg_tensor_elem_pn_opt(per, per, TENSOR_NUM(t), /);
    
    for (int i=0; i<TENSOR_NUM(t); ++i) {
        //p_t[i] = i*per + from;
        // gap = pre * i
        cg_tensor_elem_pn_opt(gap, per, i, *);
        // p_t[i] = gap + from
        cg_tensor_elem_ptr_opt(p_t + (i*tensor_elem_size), gap, from, +);
    }
    return 0;
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

float* cg_tensor_get(cg_tensor_t* thiz, ...)
{
    int coord[TENSOR_AXES(thiz)];
    va_list vargs;
    va_start(vargs, thiz);
    for (int i=0; i<TENSOR_AXES(thiz); ++i) {
        coord[i] = va_arg(vargs, int);
    }
    va_end(vargs);

    return __coordinate_router(thiz->elems, &TENSOR_STRIDE(thiz, 0), TENSOR_AXES(thiz), coord);
}

int cg_tensor_set(cg_tensor_t* thiz, cg_tensor_elem_type val, ...)
{
    int coord[TENSOR_AXES(thiz)];
    va_list vargs;
    va_start(vargs, thiz);
    for (int i=0; i<TENSOR_AXES(thiz); ++i) {
        coord[i] = va_arg(vargs, int);
    }
    va_end(vargs);

    cg_ref_t elem = __coordinate_router(thiz->elems, &TENSOR_STRIDE(thiz, 0), TENSOR_AXES(thiz), coord);
    //*elem = val;
    cg_tensor_elem_number_assign(elem, val);
    return 0;
}

int cg_tensor_T(cg_tensor_t* thiz)
{
    // TODO: do the T of tensor
    
}

sub_tensor_t cg_tensor_get_sub (cg_tensor_t* thiz, int axes, ...)
{
    int coord[axes];
    va_list vargs;
    va_start(vargs, axes);
    for (int i=0; i<axes; ++i) {
        coord[i] = va_arg(vargs, int);
    }
    va_end(vargs);

    sub_tensor_t sub_thiz = cg_tensor_to_sub_tensor(thiz);
    return sub_tensor_get_sub(&sub_thiz, axes, coord);
}


int cg_tensor_to_tensor(cg_tensor_t* dist, const cg_tensor_t* src)
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

