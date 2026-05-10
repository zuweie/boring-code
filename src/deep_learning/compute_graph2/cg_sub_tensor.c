#include <stdlib.h>
#include <string.h>
#include "cg_debug.h"
#include "cg_ref.h"
#include "cg_elem_spec.h"
#include "cg_tensor.h"
#include "cg_sub_tensor.h"

static inline cg_ref_t __coordinate_router(sub_tensor_t* sub_tensor, int axes, int coordinate[])
{

    if (axes >=0 && axes <= sub_tensor->sub_axes-1) {
        char* base = sub_tensor->elems;

        for (int i=0; i<axes; ++i) {
            base += coordinate[i] * sub_tensor->sub_stride[i] * sub_tensor->sub_elem_spec->eleme_size;
        }
        return base;
    }
    return NULL;
}

static int __do_slice(sub_tensor_t* sub_dest, sub_tensor_t* sub_src, const int slice_axes, const int slice[], int working_axis, int dist_coord[], int src_coord[])
{
    int i,j;
    if (working_axis == slice_axes - 1 ) {

        sub_tensor_t dest;
        sub_tensor_t src;

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
            sub_tenson_iterate(&dest, to_fill, NULL, elem_opt_assign);

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
            sub_tensor_get_sub(&src,  sub_src,  working_axis, src_coord);

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
            sub_tensor_iterate(&dest, to_fill, NULL, elem_opt_assign);

        } else if (mode == pd_mode_edge) {
            // 只拿最后一个。
            src_coord[working_axis]  = sub_src->sub_dimens[working_axis] - 1;
            sub_tensor_get_sub(&src, sub_src, working_axis, src_coord);
            
            sub_tensor_to_sub(&dest, &src);
        }
    }
    return 0;
} 

static int __do_binary_opt(sub_tensor_t* dest, sub_tensor_t* t1, sub_tensor_t* t2, int working_axis, int t1_batch_axes, int t2_batch_axes, int dest_coord[], int t1_coord[], int t2_coord[], int (*batch_opt)(sub_tensor_t* sub_dest, sub_tensor_t* sub_t1, sub_tensor_t* sub_t2))
{

    if (working_axis == t1_batch_axes-1) {

        sub_tensor_t sub_dest;
        sub_tensor_t sub_t1;
        sub_tensor_t sub_t2;
        sub_tensor_get_sub(&sub_dest, dest, t1_batch_axes, dest_coord);
        sub_tensor_get_sub(&sub_t1, t1, t1_batch_axes, t1_coord);
        sub_tensor_get_sub(&sub_t2, t2, t2_batch_axes, t2_coord);

        return batch_opt(&sub_dest, &sub_t1, &sub_t2);

    } else {
        int t2_axis = working_axis - (t1_batch_axes - t2_batch_axes);
        for (int i=0; i<t1->sub_axes[working_axis]; ++i) {
            dest_coord[working_axis] = i;
            t1_coord[working_axis]   = i;
            if (t2_axis >= 0) {
                if (t2->sub_axes[t2_axis] == t1->sub_axes[working_axis]) {
                    t2_coord[t2_axis] = i;
                } else if (t2->sub_axes[t2_axis] == 1) {
                    t2_coord[t2_axis] = 0;
                } else {
                    CG_DEBUG("ERROR <%d@%d>: t2->sub_axes[%d](%d) does not equal t1->sub_axes[%d](%d) either not 1(act %d)\n", \
                        __LINE__, __FILE__, \
                        t2_axis, t2->sub_axes[t2_axis], \
                        working_axis, t2->sub_axes[working_axis]\
                    );
                    return -1
                }
            }
            __do_binary_opt(dest, t1, t2, working_axis+1, t1_batch_axes, t2_batch_axes, dest_coord, t1_coord, t2_coord, opt);
        }
    }
}


int sub_tensor_get_sub(sub_tensor_t* sub_tensor, sub_tensor_t* sub_src, int axes, int coord[]) 
{   
    void* sub_elems = __coordinate_router(sub_src, axes, coord);

    if (sub_elems) {
        int   sub_axes   = sub_src->sub_axes -  axes;
        int*  sub_dimens = &(sub_src->sub_dimens[sub_axes]);
        int*  sub_stride = &(sub_src->sub_stride[sub_axes]);

        *sub_tensor = (sub_tensor_t) {
            .sub_elems  = sub_elems,
            .sub_axes   = sub_axes,
            .sub_dimens = sub_dimens,
            .sub_stride = sub_stride,
            .sub_elem_spec = sub_src->sub_elem_spec
        }
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



int sub_tensor_slice(sub_tensor_t* dest, sub_tensor_t* src, int slice_axes, int slice[])
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

int sub_tensor_binary_opt(sub_tensor_t* dest, sub_tensor_t* t1, sub_tensor_t *t2, int t1_batch_axes, int t2_batch_axes, int (*opt)(sub_tensor_t* sub_dest, sub_tensor_t* sub_t1, sub_tensor_t* sub_t2))
{
    // all the check will be do in cg_tensor level
    int dest_batch_coord[t1_batch_axes];
    int t1_batch_coord[t1_batch_axes];
    int t2_batch_coord[t2_batch_axes];
    return __do_binary_opt(dest, t1, t2, 0, t1_batch_axes, t2_batch_axes, dest_batch_coord, t1_batch_coord, t2_batch_coord, opt);
}

int sub_tensor_dot(sub_tensor_t* dest, sub_tensor_t* sub_t1, sub_tensor_t* sub_t2) 
{

    char* p_dest = dest->sub_elem;
    char* p_m1   = sub_t1->sub_elems;
    char* p_m2   = sub_t2->sub_elems;

    int    r1     = sub_t1->sub_dimens[0];
    int    c1     = sub_t1->sub_dimens[1];
    int    r2     = sub_t2->sub_dimens[0];
    int    c2     = sub_t2->sub_dimens[1];
    int    k      = 0;

    for (int i=0; i<r1; ++i) {
        for (int j=0; j<c2; ++j) {
            //p_dist[k++] = p_m1[i*c1+j] * p_m2[j*r2+i];
            CALL_ELEM_OPT(\
                dest->sub_elem_spec, \
                elem_opt_multiply,   \
                (p_dest + ((k++)    * dest->sub_elem_spec->elem_size)),   \
                (p_m1   + ((i*c1+j) * sub_t1->sub_elem_spec->elem_size)), \
                (p_m2   + ((j*r2+i) * sub_t2->sub_elem_spec->elem_size )),\
            );
        }
    }
    return 0;
}

int sub_tensor_subtract(sub_tensor_t* dest, sub_tensor_t* t1, sub_tensor_t* t2)
{
    int elem_size   = t1->sub_axes[0] * t1->sub_stride[0];
    char* dest_base = dest->sub_elems;
    char* t1_base   = t1->sub_elems;
    char* t2_base   = t2->sub_elems;

    for (int i=0; i<elem_size; ++i) {
        CALL_ELEM_OPT(\
            dest->sub_elem_spec,\
            elem_opt_subtract,  \
            (dest_base + (i*dest->sub_elem_spec->elem_size)),\
            (t1_base   + (i*t1->sub_elem_spec->elem_size)),  \
            (t2_base   + (i*t2->sub_elem_spec->elem_size)),  \
        );
    }
    return 0;
}

int sub_tensor_add(sub_tensor_t* dest, sub_tensor_t* t1 sub_tensor_t* t2)
{
int elem_size   = t1->sub_axes[0] * t1->sub_stride[0];
    char* dest_base = dest->sub_elems;
    char* t1_base   = t1->sub_elems;
    char* t2_base   = t2->sub_elems;

    for (int i=0; i<elem_size; ++i) {
        CALL_ELEM_OPT(\
            dest->sub_elem_spec,\
            elem_opt_add,\
            (dest_base + (i*dest->sub_elem_spec->elem_size)),\
            (t1_base   + (i*t1->sub_elem_spec->elem_size)),\
            (t2_base   + (i*t2->sub_elem_spec->elem_size)),\
        );
    }
    return 0;
}

int sub_tensor_multiply(sub_tensor_t* dest, sub_tensor_t* t1 sub_tensor_t* t2)
{
int elem_size   = t1->sub_axes[0] * t1->sub_stride[0];
    char* dest_base = dest->sub_elems;
    char* t1_base   = t1->sub_elems;
    char* t2_base   = t2->sub_elems;

    for (int i=0; i<elem_size; ++i) {
        CALL_ELEM_OPT(\
            dest->sub_elem_spec,\
            elem_opt_multiply,\
            (dest_base + (i*dest->sub_elem_spec->elem_size)),\
            (t1_base   + (i*t1->sub_elem_spec->elem_size)),\
            (t2_base   + (i*t2->sub_elem_spec->elem_size)),\
        );
    }
    return 0;
}


int sub_tensor_T(sub_tensor_t* dest, sub_tensor_t* t1)
{
    if (t1->sub_axes[2] == 1 || t1->sub_axes[1] == 1) {
        // 如果 x 或者 y 轴其中一轴为 1，那么可以直接将数据 copy 给 dest。
        return sub_tensor_to_sub(dest, t1);
    } else {
        // 
        char* dest_base = dest->sub_elems;
        char* t1_base   = t1->sub_elems;

        int dest_rows   = dest->sub_axes[2];
        int dest_cols   = dest->sub_axes[1];

        int t1_rows     = t1->sub_axes[2];
        int t1_cols     = t1->sub_axes[1];

        for (int i=0; i<t1_rows; ++i) {
            for (int j=0; j<t1_cols; ++j) {
                
                CALL_ELEM_OPT(\
                    dest->sub_elem_spec, \
                    elem_opt_assign,     \
                    (dest + (j*dest_cols+i) * dest->sub_elem_spec->eleme_size), \
                    (t1_base + (i*t1_cols+j) * t1->sub_elem_spec->eleme_size), \
                    NULL\
                );
            }
        }
    }
    return 0;
}

int sub_tensor_iterate(sub_tensor_t* dest, cg_ref_t opt1, cg_ref_t opt2, elem_opt_t elem_opt)
{
    int elem_num = dest->sub_dimens[0] * dest->sub_stride[0];
    for (int i=0; i<elem_num; ++i) {
        CALL_ELEM_OPT(dest->sub_elem_spec, elem_opt, (dest->sub_elems + (i * dest->sub_elem_spec->eleme_size)), opt1, opt2);
    }
    return 0;
}