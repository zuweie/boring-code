#include <stdlib.h>
#include <string.h>
#include "cg_ref.h"
#include "cg_elem_spec.h"
#include "cg_tensor.h"
#include "cg_sub_tensor.h"


static inline cg_ref_t __coordinate_router(sub_tensor_t* sub_tensor, int axes, int coordinate[])
{
   
    for (int i=0; i<axes; ++i) {

        
    }
}

static int __do_slice(sub_tensor_t* sub_dest, sub_tensor_t* sub_src, const int slice_axes, const int slice[], int working_axis, int dist_coord[], int src_coord[])
{
    int i,j;
    sub_tensor_t dest;
    sub_tensor_t src;
    if (working_axis == slice_axes - 1 ) {

        for (i=slice[working_axis*2], j=0; i<slice[working_axis*2+1]; ++i, ++j) {
            dist_coord[working_axis] = j;
            src_coord[working_axis]  = i;

            sub_tensor_get_sub(&dest, sub_dest, slice_axes, dist_coord);
            sub_tensor_get_sub(&src, sub_src, slice_axes, src_coord);

            sub_tensor_to_sub(&dest, &src);
        }
        
    } else {
        for (i=slice[working_axis*2], j=0; i<slice[working_axis*2+1]; ++i, ++j) {
            dist_coord[working_axis] = j;
            src_coord[working_axis]  = i;
            __do_slice(sub_dest, sub_src, slice_axes, slice, working_axis+1, dist_coord, src_coord);
        }
    }
    return 0;
}

static int __do_padding(sub_tensor_t* sub_dest, const sub_tensor_t* sub_src, const int padding_axes, const int padding[], int working_axis, int dest_coord[], int src_coord[], padding_mode_t mode, cg_ref_t to_fill) 
{
    int i, j;

    int padding_left_start = 0;
    int padding_left_end   = padding[working_axis * 2];

    int padding_middle_start = padding_left_end;
    int padding_middle_end   = src->sub_dimens[working_axis] + padding[working_axis * 2];

    int padding_right_start = padding_middle_end;
    int padding_right_end   = src->sub_dimens[working_axis] + padding[working_axis * 2 + 1];

    sub_tensor_t dest;
    sub_tensor_t src;

    for (i=padding_left_start; i<padding_left_end; ++i) {
        dest_coord[working_axis] = i;
        dest = sub_tensor_get_sub(sub_dest, working_axis, dest_coord);

        if (mode == pd_mode_fill) {
            sub_tenson_foreach(&dest, to_fill, NULL, elem_opt_assign);

        } else if (mode == pd_mode_edge) {
            // 只拿第一个。
            src_coord[working_axis]  = 0;
            src = sub_tensor_get_sub(sub_dest, working_axis, src_coord);
            sub_tensor_to_sub(&dest, &src);
        }
    }
    
    for (i=padding_middle_start, j=0; i<padding_middle_end; ++i, ++j) {
        dest_coord[working_axis] = i;
        src_coord[working_axis]  = j;
        if (working_axis == padding_axes - 1) {
            // value copy 
            sub_tensor_get_sub(&dest, sub_dest, working_axis, dest_coord);
            sub_tensor_get_sub(&src, sub_src, working_axis, src_coord);

            sub_tensor_to_sub(&dest, &src);

        } else {
            // 继续下一个维度的填充。
            __do_padding(sub_dest, sub_src, padding_axes, padding, working_axis+1, dest_coord, src_coord, mode, to_fill);
        }
    }

    for (i=padding_right_start; i<padding_middle_end; ++i) {
        dest_coord[working_axis] = i;
        sub_tensor_get_sub(&dest, sub_dest, working_axis, dest_coord);
        
        if (mode == pd_mode_fill) {
            // 
            sub_tensor_foreach(&dest, to_fill, NULL, elem_opt_assign);

        } else if (mode == pd_mode_edge) {
            // 只拿最后一个。
            src_coord[working_axis]  = sub_src->sub_dimens[working_axis] - 1;
            sub_tensor_get_sub(&src, sub_src, working_axis, src_coord);
            
            sub_tensor_to_sub(&dest, &src);
        }
    }
    return 0;
} 

int sub_tensor_get_sub(sub_tensor_t* sub_tensor, const sub_tensor_t* sub_t1, int axes, int coord[]) 
{   
    if (sub_t1->sub_axes >= axes) {
        void* sub_elems  = __coordinate_router(sub_t1, axes, coord);
        int   sub_axes   = sub_t1->sub_axes - axes;
        int*  sub_dimens = &(sub_t1->sub_dimens[axes]);
        int*  sub_stride = &(sub_t1->sub_stride[axes]);

        *sub_tensor =  (sub_tensor_t) {
            .sub_elems=sub_elems, 
            .sub_axes=sub_axes, 
            .sub_dimens=sub_dimens, 
            .sub_stride=sub_stride, 
            .sub_elem_spec = sub_t1->sub_elem_spec
        };
        return 0;
    }

    *sub_tensor =  (sub_tensor_t) {
        .sub_elems=NULL, 
        .sub_axes=-1, 
        .sub_dimens=NULL, 
        .sub_stride=NULL, 
        .sub_elem_spec = NULL
    };
    return -1;
}

int sub_tensor_elem_number(sub_tensor_t* sub_tensor)
{
    return sub_tensor->sub_dimens[0] * sub_tensor->sub_stride[0];
}

int sub_tensor_slice(sub_tensor_t* dest, sub_tensor_t* sub_src, int slice_axes, int slice[])
{
    int dest_coord[slice_axes];
    int src_coord[slice_axes];
    return __do_slice(dest, src, slice_axes, slice, 0, dest_coord, src_coord);
}

int sub_tensor_padding(sub_tensor_t* dest, sub_tensor_t* src, int padding_axes, int padding[], padding_mode_t mode, cg_ref_t to_fill)
{
    int dest_coord[padding_axes];
    int src_coord[padding_axes];
    return __do_padding(dest, src, padding_axes, padding[], 0, dest_coord, src_coord, mode, to_fill);
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
        memcpy(dist->sub_elems, src->sub_elems, src_number * src->sub_elem_spec->elem_size);
        return 0;
    }
    return -1;
}

int sub_tensor_T(sub_tensor_t* dist, const sub_tensor_t* sub_tensor)
{
    return 0;
}

int sub_tensor_foreach(sub_tensor_t* dest, cg_ref_t opt1, cg_ref_t opt2, elem_opt_t elem_opt)
{
    int elem_num = dest->sub_dimens[0] * dest->sub_stride[0];
    for (int i=0; i<elem_num; ++i) {
        CALL_ELEM_OPT(dest->sub_elem_spec, elem_opt, (dest->sub_elems + (i * dest->sub_elem_spec->eleme_size)), opt1, opt2);
    }
    return 0;
}