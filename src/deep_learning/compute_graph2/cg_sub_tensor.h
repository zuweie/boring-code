/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-03-15 08:15:48
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-03-15 09:49:06
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_sub_tensor.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __CG_SUB_TENSOR_H__
#define __CG_SUB_TENSOR_H__

static inline float __assign_scalar(float e, float scalar) 
{
    return scalar;
}

static inline float __mulit_scalar(float e, float scalar) 
{
    return e * scalar;
}

static inline float __add_opt(float e1, float e2)
{
    return e1 + e2;
}

static inline float __mulit_opt(float e1, float e2) 
{
    return e1 * e2;
}

static inline float __subtract_opt(float e1, float e2) 
{
    return e1 - e2;
}

static inline sub_tensor_t __get_sub_tensor(const sub_tensor_t* sub_t1, int axes, int* coord) 
{   
    if (sub_t1->sub_axes >= axes) {
        void* sub_elems  = __coordinate_router(sub_t1->sub_elems, sub_t1->sub_stride, axes, coord);
        int   sub_axes   = sub_t1->sub_axes - axes;
        int*  sub_dimens = &(sub_t1->sub_dimens[axes]);
        int*  sub_stride = &(sub_t1->sub_stride[axes]);
        return (sub_tensor_t){.sub_elems=sub_elems, .sub_axes=sub_axes, .sub_dimens=sub_dimens, .sub_stride=sub_stride};
    }
    return (sub_tensor_t) {.sub_elems=NULL, .sub_axes=-1, .sub_dimens=NULL, .sub_stride=NULL};
}

static inline sub_tensor_t __to_sub_tensor(const cg_tensor_t* tensor) 
{
    return (sub_tensor_t){.sub_elems=tensor->elems, .sub_axes=TENSOR_AXES(tensor), .sub_dimens=&TENSOR_DIMEN(tensor,0), .sub_stride=&TENSOR_STRIDE(tensor, 0)};
}

static inline int __sub_tensor_opt_scalar(const sub_tensor_t* sub, float scalar, float (*opt)(float e, float scalar))
{
    float* p_elems  = sub->sub_elems;
    int size        = sub->sub_dimens[0] * sub->sub_stride[0];
    for (int i = 0; i<size; ++i) {
        p_elems[i] = opt(p_elems[i], scalar);
    }
    return 0;
}

static inline int __sub_tensor_opt(const sub_tensor_t* dist, const sub_tensor_t* sub_t1, const sub_tensor_t* sub_t2, float(*opt)(float e1, float e2))
{
    float* p_dist_elems = dist->sub_elems;
    float* p_t1_elems   = sub_t1->sub_elems;
    float* p_t2_elems   = sub_t2->sub_elems;

    int size = dist->sub_dimens[0] * dist->sub_stride[0];

    for (int i=0; i<size; ++i) {
        p_dist_elems[i] = opt(p_t1_elems[i], p_t2_elems[i]);
    }
    return 0;
}

static inline int __sub_tensor_dot(const sub_tensor_t* dist, const sub_tensor_t* sub_t1, const  sub_tensor_t* sub_t2) 
{
    float* p_dist = dist->sub_elems;
    float* p_m1   = sub_t1->sub_elems;
    float* p_m2   = sub_t2->sub_elems;
    int    r1     = sub_t1->sub_dimens[0];
    int    c1     = sub_t1->sub_dimens[1];
    int    r2     = sub_t2->sub_dimens[0];
    int    c2     = sub_t2->sub_dimens[1];
    int    k      = 0;

    for (int i=0; i<r1; ++i) {
        for (int j=0; j<c2; ++j) {
            p_dist[k++] = p_m1[i*c1+j] * p_m2[j*r2+i];
        }
    }
    return 0;
}


#endif