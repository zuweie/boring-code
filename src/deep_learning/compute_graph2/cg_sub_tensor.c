#include <stdlib.h>
#include "cg_ref.h"
#include "cg_tensor.h"
#include "cg_sub_tensor.h"


static inline cg_ref_t __coordinate_router(sub_tensor_t* sub_tensor, int axes, int* coordinate)
{
    char* dist = sub_tensor->sub_elems;

    for (int i=0; i<axes; ++i) {
        dist += coordinate[i] * sub_tensor->sub_stride[i] * sub_tensor->sub_elem_size;
    }
    return dist;
}

static inline cg_ref_t __linear_router(sub_tensor_t* sub_tensor, int i) 
{
    return ((char*) (sub_tensor->sub_elem) + (i) * (sub_tensor->sub_elem_size));
}

sub_tensor_t sub_tensor_get_sub(const sub_tensor_t* sub_t1, int axes, int* coord) 
{   
    if (sub_t1->sub_axes >= axes) {
        void* sub_elems  = __coordinate_router(sub_t1, axes, coord);
        int   sub_axes   = sub_t1->sub_axes - axes;
        int*  sub_dimens = &(sub_t1->sub_dimens[axes]);
        int*  sub_stride = &(sub_t1->sub_stride[axes]);

        return (sub_tensor_t){
            .sub_elems=sub_elems, 
            .sub_axes=sub_axes, 
            .sub_dimens=sub_dimens, 
            .sub_stride=sub_stride, 
            .sub_elem_size=sub_t1->sub_elem_size
        };
    }

    return (sub_tensor_t) {
        .sub_elems=NULL, 
        .sub_axes=-1, 
        .sub_dimens=NULL, 
        .sub_stride=NULL, 
        .sub_elem_size = 0
    };
}

int sub_tensor_opt_scalar(const sub_tensor_t* sub, float scalar,  int (*opt)(cg_ref_t* dist, cg_ref_t e, float scalar))
{
    //cg_ref_t p_elems  = sub->sub_elems;
    int size        = sub->sub_dimens[0] * sub->sub_stride[0];
    for (int i = 0; i<size; ++i) {
        opt(
            __linear_router(sub, i),
            __linear_router(sub, i),
            scalar
        );
    }
    return 0;
}

int sub_tensor_opt(sub_tensor_t* dist, const sub_tensor_t* sub_t1, const sub_tensor_t* sub_t2, int (*opt)(cg_ref_t dist, cg_ref_t e1, cg_ref_t e2))
{
    int size = dist->sub_dimens[0] * dist->sub_stride[0];
    for (int i=0; i<size; ++i) {

         opt(
            __linear_router(dist, i),
            __linear_router(sub_t1, i),
            __linear_router(sub_t2, i)
        );

    }
    return 0;
}

int sub_tensor_dot(sub_tensor_t* dist, const sub_tensor_t* sub_t1, const sub_tensor_t* sub_t2, int (*opt)(cg_ref_t dist, cg_ref_t e1, cg_ref_t e2)) 
{

    cg_ref_t p_dist = dist->sub_elem;
    cg_ref_t p_m1   = sub_t1->sub_elems;
    cg_ref_t p_m2   = sub_t2->sub_elems;

    int    r1     = sub_t1->sub_dimens[0];
    int    c1     = sub_t1->sub_dimens[1];
    int    r2     = sub_t2->sub_dimens[0];
    int    c2     = sub_t2->sub_dimens[1];
    int    k      = 0;

    for (int i=0; i<r1; ++i) {
        for (int j=0; j<c2; ++j) {
            //p_dist[k++] = p_m1[i*c1+j] * p_m2[j*r2+i];
            opt(
                __linear_router(p_dist, k++),
                __linear_router(sub_t1, i*c1+j);
                __linear_router(sub_t2, j*r2+i);
            );
        }
    }
    return 0;
}

int sub_tensor_to_sub(sub_tensor_t* dist, const sub_tensor_t* src)
{
    int src_number  = src->sub_dimens[0] * src->sub_stride[0];
    int dist_number = dist->sub_dimens[0] * dist->sub_stride[0];
    if (dist_number >= src_number ) {
        memcpy(dist->sub_elems, src->sub_elems, src_number * src->sub_elem_size);
        return 0;
    }
    return -1;
}

int sub_tensor_T(sub_tensor_t* dist, const sub_tensor_t* sub_tensor)
{
    return 0;
}