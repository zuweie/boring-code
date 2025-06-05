/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-05-24 09:57:39
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-06-05 21:30:57
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_tensor.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "cg_debug.h"
#include "cg_tensor.h"

static float __assign_scalar(float e, float scalar) 
{
    return scalar;
}

static float __mulit_scalar(float e, float scalar) 
{
    return e * scalar;
}

static float __add_opt(float e1, float e2)
{
    return e1 + e2;
}

static float __mulit_opt(float e1, float e2) 
{
    return e1 * e2;
}

static float __subtract_opt(float e1, float e2) 
{
    return e1 - e2;
}

static void* __coordinate_router(const void* base, const int* strides, int axes, int* coordinate) 
{
    char* dist = base;
    for (int i=0; i<axes; ++i) {
        dist += coordinate[i] * strides[i] * TENSOR_ELEM_SIZE;
    }
    return dist;
}

static __sub_tensor_t __get_sub_tensor(__sub_tensor_t sub_t1, int axes, int* coord) 
{   
    if (sub_t1.sub_axes > 0) {
        void* sub_elems  = __coordinate_router(sub_t1.sub_elems, sub_t1.sub_stride, axes, coord);
        int   sub_axes   = sub_t1.sub_axes - axes;
        int*  sub_dimens = &sub_t1.sub_dimens[axes];
        int*  sub_stride = &sub_t1.sub_stride[axes];
        return (__sub_tensor_t){.sub_elems=sub_elems, .sub_axes=sub_axes, .sub_dimens=sub_dimens, .sub_stride=sub_stride};
    }
    return (__sub_tensor_t) {.sub_elems=NULL, .sub_axes=-1, .sub_dimens=NULL, .sub_stride=NULL};
}

static __sub_tensor_t __to_sub_tensor(cg_tensor_t* tensor) 
{
    return (__sub_tensor_t){.sub_elems=tensor->elems, .sub_axes=TENSOR_AXES(tensor), .sub_dimens=&TENSOR_DIMEN(tensor,0), .sub_stride=&TENSOR_STRIDE(tensor, 0)};
}

static int __sub_tensor_opt_scalar(__sub_tensor_t sub, float scalar, float (*opt)(float e, float scalar))
{
    float* p_elems  = sub.sub_elems;
    int size        = sub.sub_dimens[0] * sub.sub_stride[0];
    for (int i = 0; i<size; ++i) {
        p_elems[i] = opt(p_elems[i], scalar);
    }
    return 0;
}

static int __sub_tensor_opt(__sub_tensor_t dist, __sub_tensor_t sub_t1, __sub_tensor_t sub_t2, float(*opt)(float e1, float e2))
{
    float* p_dist_elems = dist.sub_elems;
    float* p_t1_elems   = sub_t1.sub_elems;
    float* p_t2_elems   = sub_t2.sub_elems;

    int size = dist.sub_dimens[0] * dist.sub_stride[0];

    for (int i=0; i<size; ++i) {
        p_dist_elems[i] = opt(p_t1_elems[i], p_t2_elems[i]);
    }
    return 0;
}

static int __sub_tensor_dot(__sub_tensor_t dist, __sub_tensor_t sub_t1, __sub_tensor_t sub_t2) 
{
    float* p_dist = dist.sub_elems;
    float* p_m1   = sub_t1.sub_elems;
    float* p_m2   = sub_t2.sub_elems;
    int    r1     = sub_t1.sub_dimens[0];
    int    c1     = sub_t1.sub_dimens[1];
    int    r2     = sub_t2.sub_dimens[0];
    int    c2     = sub_t2.sub_dimens[1];
    int    k      = 0;

    for (int i=0; i<r1; ++i) {
        for (int j=0; j<c2; ++j) {
            p_dist[k++] = p_m1[i*c1+j] * p_m2[j*r2+i];
        }
    }
    return 0;
}

static int __reshape(char** target_elems, int** target_dimens, int new_axes, int new_dimensions[], cg_allocator_t* alloc) 
{
    unsigned int old_size = 0;
    // 这里我将加多一维，第 0 维。例如 axes 为 3。 那么的有 0，1，2，3个轴。第三个轴是第 0 维。
    if (*target_dimens == NULL) {
        *target_dimens = (int*) malloc (((new_axes+1)*2+1) * sizeof(int));
    } else {
        old_size = _D_SIZE(*target_dimens);
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

    // 跟新 element 池子大小 
    if (_D_SIZE(*target_dimens) > old_size) {

        void *old_elems = *target_elems;
        *target_elems   = cg_alloc(alloc, _D_SIZE(*target_dimens));

        // 如果原来旧有数据复制旧的，然后回收。
        if (old_elems) {
            memcpy(*target_dimens, old_elems, old_size);
            cg_recycle(alloc, old_elems);
        }
    }
    
    return 0;
}

static cg_tensor_t* __create_tensor(int axes, int dimensions[], cg_allocator_t* alloc)
{
    cg_tensor_t* tensor = (cg_tensor_t*) malloc(sizeof(cg_tensor_t));
    tensor->allocator   = alloc;
    tensor->dimensions  = NULL;
    tensor->elems       = NULL;
    __reshape(&tensor->elems, &tensor->dimensions, axes, dimensions, alloc);
    return tensor;
}

static int __do_slice(void** dist, __sub_tensor_t sub_src, const int slice[], int curr_axis, int src_coord[]) 
{
    if (curr_axis == sub_src.sub_axes -1) {
        int axes = curr_axis + 1;
        // 拷贝发生在最后一维
        src_coord[curr_axis] = slice[curr_axis * 2];

        unsigned int size = (slice[curr_axis * 2+1] - slice[curr_axis*2]) * TENSOR_ELEM_SIZE;
        const void* src   = __coordinate_router(sub_src.sub_elems, sub_src.sub_stride, axes, src_coord); 

        memcpy((*dist), src, size);
        //到下一个
        (*dist) = ((char*)(*dist)) + size;
    } else {
        for (int i=slice[curr_axis*2]; i<slice[curr_axis*2+1]; ++i) {
            src_coord[curr_axis] = i;
            __do_slice(dist, sub_src, slice, curr_axis+1, src_coord);
        }
    }
    return 0;
}

// 一种新的 padding 算法, 复杂而又巧妙，简直就是艺术品的算法。nice
static int __do_padding(const __sub_tensor_t dist, const __sub_tensor_t src, float fill, int* padding, int curr_axis) 
{

    int i, j;

    int sub_dist_index;
    int sub_src_index;

    int padding_left_start = 0;
    int padding_left_end   = padding[curr_axis * 2];

    int padding_middle_start = padding_left_end;
    int padding_middle_end   = padding[curr_axis * 2] + src.sub_dimens[0];

    int padding_right_start = padding_middle_end;
    int padding_right_end   = padding[curr_axis * 2] + src.sub_dimens[0] + padding[curr_axis * 2 + 1];

    // fill the left part
    for (i = padding_left_start; i < padding_left_end; ++i)
    {
        sub_dist_index = i;
        __sub_tensor_t sub_dist = __get_sub_tensor(dist, 1, &sub_dist_index);
        if (sub_dist.sub_axes == 0) {
            // 只有一个元素，那么直接赋值
            *((float*)sub_dist.sub_elems) = fill;
        } else {
            __sub_tensor_opt_scalar(sub_dist, fill, __assign_scalar);
        }
    }

    // fill the right part
    for (i = padding_right_start; i < padding_right_end; ++i)
    {
        // fill
        sub_dist_index = i;
        __sub_tensor_t sub_dist = __get_sub_tensor(dist, 1, &sub_dist_index);
        if (sub_dist.sub_axes == 0) {
            // 只有一个元素，那么直接赋值
            *((float*)sub_dist.sub_elems) = fill;
        } else {
            __sub_tensor_opt_scalar(sub_dist, fill, __assign_scalar);
        }
    }

    for (i = padding_middle_start; i < padding_middle_end; ++i)
    {
        //
        sub_dist_index = i;
        sub_src_index  = sub_dist_index - padding[curr_axis * 2];

        __sub_tensor_t sub_dist = __get_sub_tensor(dist, 1, &sub_dist_index);
        __sub_tensor_t sub_src  = __get_sub_tensor(src,  1, &sub_src_index);

        if (sub_dist.sub_axes == 0){
            // 单个元素直接 copy
            float *p_dist = sub_dist.sub_elems;
            float *p_src  = sub_src.sub_elems;
            *p_dist = *p_src;
        }
        else{
            __do_padding(sub_dist, sub_src, fill, padding, curr_axis + 1);
        }
    }
    return 0;
}

static int __do_doting(__sub_tensor_t dist, __sub_tensor_t sub1, __sub_tensor_t sub2, int curr_axis, int* dist_coord) 
{
    if (curr_axis == dist.sub_axes-2) {
        int axes = curr_axis + 1;
        
        int sub1_coord[sub1.sub_dimens[curr_axis]];
        int sub2_coord[sub2.sub_dimens[curr_axis]];

        for (int i=0; i<curr_axis; ++i) {
            sub1_coord[i] = (sub1_coord[i] == 1 ? sub1_coord[i] : dist_coord[i]);
            sub2_coord[i] = (sub2_coord[i] == 1 ? sub2_coord[i] : dist_coord[i]);
        }
        __sub_tensor_t sub_dist = __get_sub_tensor(dist, axes, dist_coord);
        __sub_tensor_t sub_sub1 = __get_sub_tensor(sub1, axes, sub1_coord);
        __sub_tensor_t sub_sub2 = __get_sub_tensor(sub2, axes, sub2_coord);

        return __sub_tensor_dot(sub_dist, sub_sub1, sub_sub2);

    } else {
        for (int i=0; i<dist.sub_dimens[curr_axis]; ++i) {
            dist_coord[curr_axis] = i;
            __do_doting(dist, sub1, sub2, curr_axis+1, dist_coord);
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

int cg_tensor_recycle(cg_tensor_t* thiz)
{
    cg_recycle(thiz->allocator, thiz->elems);
    free(thiz->dimensions);
    free(thiz);
    return 0;
}

cg_tensor_t* cg_tensor_slice(cg_tensor_t* thiz, int slice_axes, ...)
{
    int slice[ 2 * TENSOR_AXES(thiz) ];
    int new_dimensions[TENSOR_AXES(thiz)];

    va_list vargs;
    va_start(vargs, slice_axes);
    for (int i=0; i<TENSOR_AXES(thiz); ++i) {
        slice[i*2]   = i<slice_axes? va_arg(vargs, int) : 0;
        slice[i*2+1] = i<slice_axes? va_arg(vargs, int) : TENSOR_DIMEN(thiz, i);
        new_dimensions[i] = slice[i*2+1] - slice[i*2];
    }
    va_end(vargs);

    cg_tensor_t* tensor = __create_tensor(TENSOR_AXES(thiz), new_dimensions, thiz->allocator);

    // copy 数据
    int coordinate[TENSOR_AXES(thiz)];
    void* dist = tensor->elems;
    __do_slice(&dist, __to_sub_tensor(thiz), slice, 0, coordinate);
    return tensor;
}

cg_tensor_t* cg_tensor_padding(cg_tensor_t* thiz, float fill, int padding_axes, ...)
{
    int padding[TENSOR_AXES(thiz) * 2];
    int new_dimens[TENSOR_AXES(thiz)];

    va_list vargs;
    va_start(vargs, padding_axes);
    for (int i=0; i<TENSOR_AXES(thiz); ++i) {
        padding[i*2]      = i<padding_axes ? va_arg(vargs, int) : 0;
        padding[i*2+1]    = i<padding_axes ? va_arg(vargs, int) : 0;
        new_dimens[i]     = padding[i*2] + padding[i*2+1] + TENSOR_DIMEN(thiz, i);
    }
    
    cg_tensor_t* tensor = __create_tensor(TENSOR_AXES(thiz), new_dimens, thiz->allocator);
    __do_padding(__to_sub_tensor(tensor), __to_sub_tensor(thiz), fill, padding, 0);

    return tensor;
}

int cg_tensor_dot(cg_tensor_t* r, cg_tensor_t* t1, cg_tensor_t* t2)
{
    // 检查 t1, t2 维度是否能广播。
    int axes1 = TENSOR_AXES(t1);
    int axes2 = TENSOR_AXES(t2);
    if (axes1 != axes2) {
        CG_DEBUG("t1 axes %d not euq t2 axes %d\n", axes1, axes2);
        return -1;
    } 
    for (int i=0; i<axes1-2; ++i) {
        if (TENSOR_DIMEN(t1, i) != TENSOR_DIMEN(t2, i)) {
            if (TENSOR_DIMEN(t1, i) != 1 && TENSOR_DIMEN(t2, i) != 1) {
                CG_DEBUG("t1 %d axis`s dimens is %d, t2 %d axis`s dimens is %d, not match!\n", i, TENSOR_DIMEN(t1, i), i, TENSOR_DIMEN(t2, i));
                return -1;
            } 
        }
    }

    if (TENSOR_DIMEN(t1, axes1-1) != TENSOR_DIMEN(t2,axes2-2)) {
        CG_DEBUG("t1 col is %d, t2 row is %d, not match!\n", TENSOR_DIMEN(t1,axes1-1), TENSOR_DIMEN(t2, axes2-1));
        return -1;
    }

    int coordinate[TENSOR_AXES(t1)];
    int new_dimens[TENSOR_AXES(t1)];
    for (int i=0; i<TENSOR_AXES(t1); ++i) {
        new_dimens[i] = TENSOR_DIMEN(t1, i) > TENSOR_DIMEN(t2, i) ? TENSOR_DIMEN(t1, i) : TENSOR_DIMEN(t2, i);
    }


    __reshape(r->elems, &r->dimensions, TENSOR_AXES(t1), new_dimens, t1->allocator);

    return __do_doting(__to_sub_tensor(r), __to_sub_tensor(t1), __to_sub_tensor(t2), 0, coordinate);
}

int cg_tensor_sum(cg_tensor_t* r, cg_tensor_t* t1, cg_tensor_t* t2)
{
    int axes1 = TENSOR_AXES(t1);
    int axes2 = TENSOR_AXES(t2);
    if (axes1 != axes2) {
        CG_DEBUG("t1 axes %d not euq t2 axes %d\n", axes1, axes2);
        return -1;
    } 
    for (int i=0; i<axes1; ++i) {
        if (TENSOR_DIMEN(t1, i) != TENSOR_DIMEN(t2, i)) {
            CG_DEBUG("t1 %d axis`s dimens is %d, t2 %d axis`s dimens is %d\n, not match!", i, TENSOR_DIMEN(t1, i), i, TENSOR_DIMEN(t2, i));
            return -1;
        }
    }

    __reshape(&r->elems, &r->dimensions, axes1, &t1->dimensions[1], r->allocator);

    // reshap the r
    // TODO: do sum.
    __sub_tensor_opt(__to_sub_tensor(r), __to_sub_tensor(t1), __to_sub_tensor(t2), __add_opt);
    return 0;
}

int cg_tensor_subtract(cg_tensor_t* r, cg_tensor_t* t1, cg_tensor_t* t2)
{
    int axes1 = TENSOR_AXES(t1);
    int axes2 = TENSOR_AXES(t2);
    if (axes1 != axes2) {
        CG_DEBUG("t1 axes %d not euq t2 axes %d\n", axes1, axes2);
        return -1;
    } 
    for (int i=0; i<axes1; ++i) {
        if (TENSOR_DIMEN(t1, i) != TENSOR_DIMEN(t2, i)) {
            CG_DEBUG("t1 %d axis`s dimens is %d, t2 %d axis`s dimens is %d\n, not match!", i, TENSOR_DIMEN(t1, i), i, TENSOR_DIMEN(t2, i));
            return -1;
        }
    }
    __reshape(&r->elems, &r->dimensions, axes1, &t1->dimensions[1], r->allocator);

    // reshap the r
    // TODO: do sum.
    __sub_tensor_opt(__to_sub_tensor(r), __to_sub_tensor(t1), __to_sub_tensor(t2), __subtract_opt);
    return 0;
}

int cg_tensor_scale(cg_tensor_t* t, float scalar)
{
    return __sub_tensor_opt_scalar(__to_sub_tensor(t), scalar, __mulit_scalar);
}


int cg_tensor_fill(cg_tensor_t* t, float fill)
{
    // float* p_t  = t->elems;
    // for (int i=0; i<TENSOR_NUM(t); ++i) {
    //     p_t[i] = fill;
    // }
    return __sub_tensor_opt_scalar(__to_sub_tensor(t), fill, __assign_scalar);

}

int cg_tensor_arange(cg_tensor_t* t, float from, float to)
{
    float* p_t = t->elems;
    float  per = (to - from) / TENSOR_NUM(t);

    for (int i=0; i<TENSOR_NUM(t); ++i) {
        p_t[i] = i*per + from;
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

int cg_tensor_set(cg_tensor_t* thiz, float val, ...)
{
    int coord[TENSOR_AXES(thiz)];
    va_list vargs;
    va_start(vargs, thiz);
    for (int i=0; i<TENSOR_AXES(thiz); ++i) {
        coord[i] = va_arg(vargs, int);
    }
    va_end(vargs);

    float* elem = __coordinate_router(thiz->elems, &TENSOR_STRIDE(thiz, 0), TENSOR_AXES(thiz), coord);
    *elem = val;
    return 0;
}
