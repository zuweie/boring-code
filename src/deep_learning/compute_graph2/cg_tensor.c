/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-05-24 09:57:39
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-05-31 22:09:54
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_tensor.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>
#include "cg_debug.h"
#include "cg_tensor.h"

static void* __coordinate_router(const char* base, const int* dimensions,  int axes, int* coordinate) 
{
    char* dist = tensor->elems;
    for (int i=0; i<axes; ++i) {
        dist += coordinate[i] * _D_STRIDE(dimensions,i) * TENSOR_ELEM_SIZE;
    }
    return dist;
}

static int __reshape(char** target_elems, int** target_dimens, int new_axes, int new_dimensions[], cg_allocator_t* alloc) 
{
    unsigned int old_size = 0;
    if (*target_dimens == NULL) {
        *target_dimens = (int*) malloc ((new_axes*2+1) * sizeof(int))
    } else {
        old_size = _D_SIZE(*target_dimens);
        if (_D_AXES(*target_dimens) < new_axes) 
            (*target_dimens) = (int*)realloc((new_axes*2+1) * sizeof(int));
    }

    _D_AXES(*target_dimens) = new_axes;

    // 更新维度信息。
     for (int i=new_axes-1; i>=0 ; --i) {

        _D_DIMEN(*target_dimens, i)   = new_dimensions[i];
        _D_STRIDE(*target_dimens, i)  = (i == new_axes-1? 1 : _D_DIMEN(*target_dimens, i+1) * _D_STRIDE(*target_dimens, i+1));
    }

    // 跟新 element 池子大小 
    if (_D_SIZE(*target_dimens) > old_size) {

        void *old_elems = *target_elems;
        *target_elems   = cg_alloc(tensor->allocator, _D_SIZE(*target_dimens));

        // 如果原来旧有数据复制旧的，然后回收。
        if (old_elems) {
            memcpy(*target_dimens, old_elems, old_size);
            cg_recycle(alloc, old_elems);
        }
    }
    
    return 0;
}

static cg_tensor_t* __create_tensor(int axes, int dimension[], cg_allocator_t* alloc)
{
    cg_tensor_t* tensor = (cg_tensor_t*) malloc(sizeof(cg_tensor_t) + sizeof(int) * axes);
    tensor->allocator   = alloc;
    tensor->dimensions  = NULL;
    tensor->elems       = NULL;
    __reshape(&tensor->elems, &tensor->dimensions, axes, dimension, alloc);
    return tensor;
}

static int __do_slice(void** dist, const void* src_base, const int* src_dimensions, const int slice[], int curr_axis, int coordinate[]) 
{
    if (curr_axis == _D_AXES(src_dimensions)-1) {
        int axes = curr_axis + 1;
        // 拷贝发生在最后一维
        coordinate[curr_axis] = slice[curr_axis * 2];

        unsigned int size = (slice[curr_axis * 2+1] - slice[curr_axis*2]) * TENSOR_ELEM_SIZE;
        const void* src   = __coordinate_router(src_base, src_dimensions, axes, coordinate); 

        memcpy((*dist), src, size);
        //到下一个
        (*dist) = ((char*)(*dist)) + size;
    } else {
        for (int i=slice[curr_axis*2]; i<slice[curr_axis*2+1]; ++i) {
            coordinate[curr_axis] = i;
            return __do_slice(dist, src_base, src_dimensions, slice, curr_axis+1, coordinate);
        }
    }
    return 0;
}

// 一种新的 padding 算法, 复杂而又巧妙，简直就是艺术品的算法。nice
static int __do_padding(const void* target_base, const int* target_dimens, const void* src_base, const int* src_dimens, float fill, const int padding[], int curr_axis, int target_coord[]) 
{
    if (curr_axis < _D_AXES(target_dimens)) {
        
        int i, j;
        char*        target_dist    = NULL;
        int          axes           = curr_axis + 1;
        target_coord[curr_axis]     = 0;

        int padding_left_start      = 0;
        int padding_left_end        = padding[curr_axis*2];

        int padding_middle_start    = padding_left_end;
        int padding_middle_end      = padding[curr_axis*2] + _D_DIMEN(src_dimens, curr_axis);

        int padding_right_start     = padding_middle_end;
        int padding_right_end       = padding[curr_axis*2] + _D_DIMEN(src_dimens, curr_axis) + padding[curr_axis*2+1];

        // fill the left part
        for (i = padding_left_start; i<padding_left_end ; ++i){
            // fill
            target_coord[curr_axis] = i;
            target_dist             = __coordinate_router(target_base,target_dimens, axes, target_coord);

            for (j=0; j<_D_STRIDE(target_dimens, curr_axis); ++j, target_dist += TENSOR_ELEM_SIZE) {
                *((float*)target_dist) = fill;
            }
            
        }

        // fill the right part
        for (i = padding_right_start; i < padding_right_end, ++i) {
            // fill
            target_coord[curr_axis] = i;
            target_dist = __coordinate_router(target_base, target_dimens, axes, target_coord);
            for (j=0; j<_D_STRIDE(target_dimens, curr_axis); ++j, target_dist += TENSOR_ELEM_SIZE) {
                *((float*)target_dist) = fill;
            }
        }

        // fill fucking middle part
        if (curr_axis == _D_AXES(target_dimens)-1) {
            // 根据
            int src_coord[axes];
            for(int k=0; k<axes; ++k) {
                src_coord[k] = target_coord[k] - padding[k*2];
            }

            const char* src        = __coordinate_router(src_base, src_dimens, axes, src_coord);
            target_dist            = __coordinate_router(target_base, target_dimens, axes,  target_coord);
            unsigned int cpy_size  = _D_DIMEN(src_dimens, curr_axis); * TENSOR_ELEM_SIZE
            memcpy(target_dist, src, cpy_size);
            return 0;

        } else {

            for (i = padding_middle_start; i<padding_middle_end; ++i){
                //
                target_coord[curr_axis] = i;
                return __do_padding(target_base, target_dimens, src_base, src_dimens, fill, padding, curr_axis+1, target_coord);
            }
        }
    }
    return 0;
}

static int __matrix_doting (void* dist, void* m1, int r1, int c1, void* m2, int r2, int c2)
{
    memset(dist, 0, r1*c2*sizeof(float));
    float* p_dist = dist;
    float* p_m1   = m1;
    float* p_m2   = m2;
    int    k      = 0
    for (int i=0; i<r1; ++i) {
        for (int j=0; j<c2; ++j) {
            p_dist[k++] = p_m1[i*c1+j] * p_m2[j*r2+i];
        }
    }
    return 0;
}

static int __do_doting(void* target_base, int* target_dimens, const void* t1_base, const int* t1_dimens, const void* t2_base, const int* t2_dimens, int curr_axis, int target_coord[], int t1_coord[], int t2_coord[]) {
    
    int axes = curr_axis + 1;

    // 到了倒数第二维度。开始做矩阵乘法。
    if (curr_axis == _D_AXES(target_dimens) - 2) {
        void* dist = __coordinate_router(target_base, target_dimens, axes, target_coord);
        void* m1   = __coordinate_router(t1_base, t1_dimens, axes, t1_coord);
        void* m2   = __coordinate_router(t2_base, t2_dimens, axes, t2_coord);

        return __matrix_doting(dist, m1, _D_DIMEN(t1_dimens, curr_axis), _D_DIMEN(t1_dimens, curr_axis+1), m2, _D_DIMEN(t2_dimens, curr_axis), _D_DIMEN(t2_dimens, curr_axis+1));

    } else {
        for (int i=0; i<_D_DIMEN(target_dimens); ++i) {
            target_coord[curr_axis] = i;
            t1_coord[curr_axis] = i < _D_DIMEN(t1_dimens, curr_axis) ? i : 0;
            t2_coord[curr_axis] = i < _D_DIMEN(t2_dimens, curr_axis) ? i : 0;
            return __do_doting(target_base, target_dimens, t1_base, t1_dimens, t2_base, t2_dimens, curr_axis+1, target_coord, t1_coord, t2_coord);
        }
    }
}

static int __dot(void** target_base, int** target_dimens, const void* t1_base, const int* t1_dimens, const void* t2_base, const int* t2_dimens, cg_allocator_t* alloc)
{
    // 计算内积后的形状。
    int axes  = _D_AXES(t1_dimens);
    int dimens[axes];
    int target_coord[axes];
    int t1_coord[axes];
    int t2_coord[axes];

    int i;
    for (i=0; i<axes-2, ++i) {
        dimens[i] = _D_DIMEN(t1_dimens, i) >= _D_DIMEN(t2_dimens, i)? _D_DIMEN(t1_dimens, i) : _D_DIMEN(t2_dimens, i);
    }
    dimens[axes-2] = _D_DIMEN(t1_dimens, axes-2);
    dimens[axes-1] = _D_DIMEN(t2_dimens, axes-1);

    // 首先改变形状。
    __reshape(target_base, target_dimens, axes, dimens, alloc);

    return __do_doting((*target_base), (*target_dimens), t1_base, t1_dimens, t2_base, t2_dimens, 0, target_coord, t1_coord, t2_coord);
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
    int slice[ 2 * TENSOR_AXES(thiz) ];
    int new_dimensions[TENSOR_AXES(thiz)];

    va_list vargs;
    va_start(vargs, slice_axes);
    for (i=0; i<TENSOR_AXES(thiz); ++i) {
        slice[i*2]   = i<slice_axes? va_arg(args, int) : 0;
        slice[i*2+1] = i<slice_axes? va_arg(args, int) : TENSOR_DIMEN(this, i);
        new_dimensions[i] = slice[i*2+1] - slice[i*2];
    }
    va_end(vargs);

    cg_tensor_t* tensor = __create_tensor(thiz->alloc, TENSOR_AXES(thiz), new_dimensions);

    // copy 数据
    int coordinate[TENSOR_AXES(tensor)];
    void* dist = tensor->elems;
    //__do_slice(&dist, thiz, slice, 0, coordinate);
    __do_slice(&dist, thiz->elems, thiz->dimensions, slice, 0, coordinate)
    return tensor;
}

cg_tensor_t* cg_tensor_padding(cg_tensor_t* thiz, float fill, int padding_axes, ...)
{
    int padding[TENSOR_AXES(thiz) * 2];
    int new_dimensions[TENSOR_AXES(thiz)];

    va_list vargs;
    va_start(vargs, padding_axes);
    for (int i=0; i<TENSOR_AXES(thiz); ++i) {
        padding[i*2]   = i<padding_axes ? va_arg(vargs, int) : 0;
        padding[i*2+1] = i<padding_axes ? va_arg(vargs, int) : 0;
        new_dimensions = padding[i*2] + padding[i*2+1] + TENSOR_DIMEN(thiz, i);
    }
    cg_tensor_t* tensor = __create_tensor(thiz->allocator, TENSOR_AXES(thiz), new_dimensions);
    void* dist = tensor->elems;
    int coordinate[TENSOR_AXES(tensor)];
    __do_padding(tensor->elems, tensor->dimensions, thiz->elems, thiz->dimensions, fill, padding, 0, coordinate);
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
    for (int i=0; i<axes1; ++i) {
        if (TENSOR_DIMEN(t1, i) != TENSOR_DIMEN(t2, i)) {
            if (TENSOR_DIMEN(t1, i) != 1 && TENSOR_DIMEN(t2, i) != 1) {
                CG_DEBUG("t1 %d axis`s dimens is %d, t2 %d axis`s dimens is %d\n, not match!", i, TENSOR_DIMEN(t1, i), i, TENSOR_DIMEN(t2, i));
                return -1;
            } 
        }
    }

    return __dot(&r->elems, *r->dimensions, t1->elems, t1->dimensions, t2->elems, t2->dimensions, r->allocator);
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
    float* p_r  = r->elems;
    float* p_t1 = t1->elems;
    float* p_t2 = t2->elems;
    
    for (int i=0; i<TENSOR_NUM(r); ++i) {
        p_r[i] = p_t1[i] + p_t2[i];
    }
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
    float* p_r  = r->elems;
    float* p_t1 = t1->elems;
    float* p_t2 = t2->elems;
    
    for (int i=0; i<TENSOR_NUM(r); ++i) {
        p_r[i] = p_t1[i] - p_t2[i];
    }
    return 0;
}

int cg_tensor_scale(cg_tensor_t* t, float scale)
{
    float* p_t  = r->elems;
    for (int i=0; i<TENSOR_NUM(r); ++i) {
        p_t[i] *= scale;
    }
    return 0;
}
