/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-03-28 17:28:49
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-06-28 08:24:22
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_sub_tensor.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdlib.h>
#include <string.h>
#include "cg_ref.h"
#include "cg_debug.h"
#include "cg_tensor_shape.h"
#include "cg_sub_tensor.h"

/**
 * @brief tensor 内部函数，请勿直接调用
 */
static int __do_slice(sub_tensor_t* dest, sub_tensor_t* src, const int slice_axes, const int slice[], int working_axis)
{
    int i,j;
    int ret1, ret2, ret;

    if ( working_axis == slice_axes - 1 ) {
        // 到了指定的 axis，直接 copy
        return sub_tensor_to_sub(dest, src);

    } else {

        sub_tensor_t sub_dest;
        sub_tensor_t sub_src; 

        for (i=slice[working_axis*2], j=0; i<slice[working_axis*2+1]; ++i, ++j) {

            sub_tensor_get_sub(&sub_dest, dest, 1, (int[]){j});
            sub_tensor_get_sub(&sub_src, src, 1, (int[]){i});

            if ((ret = __do_slice(&sub_dest, &sub_src, slice_axes, slice, working_axis + 1))) return ret;
        }

        return 0;
    }
}

/**
 * @brief tensor 内部函数，请勿直接调用
 */
static int __do_padding(sub_tensor_t* dest, sub_tensor_t* src, const int padding_axes, const int padding[], int working_axis, padding_mode_t mode, cg_tensor_elem_type to_fill) 
{
    int i, j;

    int padding_left_start = 0;
    int padding_left_end   = padding[working_axis * 2];

    int padding_middle_start = padding_left_end;
    int padding_middle_end   = padding_left_end + AXIS_DIMENS(src->shape);

    int padding_right_start = padding_middle_end;
    int padding_right_end   = padding_middle_end + padding[working_axis * 2 + 1];

    sub_tensor_t sub_dest;
    sub_tensor_t sub_src;

    for (i=padding_left_start; i<padding_left_end; ++i) {

        sub_tensor_get_sub(&sub_dest, dest, 1, (int[]){i});

        if (mode == pd_mode_fill) {
            //sub_tenson_iterate(&dest, to_fill, NULL, elem_opt_assign);
            sub_tensor_fill(&sub_dest, to_fill);

        } else if (mode == pd_mode_edge) {
            // 只拿 src 的第一个作为 填充物
            sub_tensor_get_sub(&sub_src, src, 1, (int[]){0});
            sub_tensor_to_sub(&sub_dest, &sub_src);
        } else {
            CG_DEBUG("ERROR <%d@%s>: unknown padding mode(%d)\n", __LINE__, __FILE__, mode);
            return -1;
        }
    }
    
    for (i=padding_middle_start, j=0; i<padding_middle_end; ++i, ++j) {

        sub_tensor_get_sub(&sub_dest, dest, 1, (int[]){i});
        sub_tensor_get_sub(&sub_src,  src,  1, (int[]){j});

        if (working_axis == padding_axes - 1) {
            // value copy 
            sub_tensor_to_sub(&sub_dest, &sub_src);

        } else {
            // 继续下一个维度的填充。
            __do_padding(&sub_dest, &sub_src, padding_axes, padding, working_axis+1, mode, to_fill);
        }
    }

    for (i=padding_right_start; i<padding_middle_end; ++i) {

        //dest_coord[working_axis] = i;
        sub_tensor_get_sub(&sub_dest, dest, 1, (int[]){i});
        
        if (mode == pd_mode_fill) {
            sub_tensor_fill(&sub_dest, to_fill);
        } else if (mode == pd_mode_edge) {
            // 只拿最后一个。
            sub_tensor_get_sub(&sub_src, src, 1, (int[]){AXIS_DIMENS(src->shape)-1});
            sub_tensor_to_sub(&sub_dest, &sub_src);
        } else {
            CG_DEBUG("Error <%d@%s>: unknown padding mode(%d)\n", __LINE__, __FILE__, mode);
        }
    }
    return 0;
} 

/**
 * @brief 这个函数必须写上注释。这是一个关于两个操作数 t1、t2 结合后赋予 dest 的操作函数。opt_axis 是指结合操作数的维度。 batch_gap 是两个操作数之间 batch 维度的差
 * 
 * @param dest 存储结果
 * @param t1 操作数1
 * @param t2 操作数2
 * @param working_axis 工作的维度，从 0 开始
 * @param opt_axis 操作数维度，例如 t1 {0，1，2，3 | 4，5}. 如果四五是 opt 的维度， 那么 opt_axis 就是 4
 * @param batch_gap 两个操作数的 batch 维度差，例如 t1 {0,1,2,3|4,5} 那么它的 batch_axes = 4, 例如 t2 {0,1,2|3,4,5,6} 它的 batch_axes = 3. 那么 batch_gap = 4 - 3。 
 * @param batch_opt 操作数 sub_t1 {4,5} opt sub_t2 {3,4,5,6} --> sub_dest
 * @return int 
 */
static int __do_binary_opt(sub_tensor_t* dest, sub_tensor_t* t1, sub_tensor_t* t2, int working_axis, int opt_axis, int batch_gap, int (*batch_opt)(sub_tensor_t* sub_dest, sub_tensor_t* sub_t1, sub_tensor_t* sub_t2))
{
    int i;
    int axis_match = 1;
    sub_tensor_t sub_dest;
    sub_tensor_t sub_t1;
    sub_tensor_t sub_t2;

    if (working_axis == opt_axis) {

        return batch_opt(&sub_dest, &sub_t1, &sub_t2);

    } else {
        //int t2_axis = working_axis - (t1_batch_axes - t2_batch_axes);
        
        int axis_t2 = working_axis - batch_gap;

        if (axis_t2 >=0 && AXIS_AXES(t2->shape) != 1 && AXIS_AXES(t2->shape) != AXIS_AXES(t1->shape)) {
            // error 
            CG_DEBUG("Error <%d@%s>: t2 axes(%d) is not 1 or t2 axes(%d) not equql t1 axes(%d)\n",\
                __LINE__, __FILE__, \
                AXIS_AXES(t2->shape), AXIS_AXES(t2->shape), AXIS_AXES(t1->shape)
            );
            return -1;
        }

        for (i=0; i<AXIS_AXES(t1->shape); ++i) {

            sub_tensor_get_sub(&sub_t1,   t1,   1, (int[]){i});

            if (axis_t2>=0) {

                AXIS_AXES(t2->shape) == 1 ? sub_tensor_get_sub(&sub_t2, t2, 1, (int[]){0}) : sub_tensor_get_sub(&sub_t2, t2, 1, (int[]){i});

            } else {

                sub_tensor_get_sub(&sub_t2, t2, 0, (int[]){0} );
                
            }

            sub_tensor_get_sub(&sub_dest, dest, 1, (int[]){i});
            __do_binary_opt(&sub_dest, &sub_t1, &sub_t2, working_axis+1, opt_axis, batch_gap, batch_opt);
        }
    }
    return 0;
}



int sub_tensor_get_sub(sub_tensor_t* sub_tensor, sub_tensor_t* sub_src, int axes, int coord[]) 
{   
    //void* sub_elems = __coordinate_router(sub_src, axes, coord);
    cg_tensor_axis_t* sub_shape = sub_tensor->shape;
    int cut_out;
    int ret = cg_tensor_shape_split_out(&sub_shape, &cut_out, axes, coord);

    if (!ret) {
        *sub_tensor = (sub_tensor_t) {
            .sub_elems = cg_tensor_elem_offset_addr(sub_tensor->sub_elems, cut_out),
            .shape     = sub_shape
        };
        return 0;
    };

    *sub_tensor =  (sub_tensor_t) {
        .sub_elems  = NULL, 
        .shape      = sub_shape
    };
    return -1;
}

int sub_tensor_to_sub(sub_tensor_t* dest,  sub_tensor_t* src)
{
    int ret = 0;
    if ( AXIS_AXES(dest->shape) == 0 && AXIS_AXES(src->shape) == 0) {
        // 单元 elem，直接复制。
        cg_tensor_elem_ref_assign(dest->sub_elems, src->sub_elems);

    } else if (AXIS_NUMBER(dest->shape) == AXIS_NUMBER(src->shape)) {

        int cpy_size = AXIS_NUMBER(dest->shape) * cg_tensor_elem_size;
        memcpy(dest->sub_elems, src->sub_elems, cpy_size);

    } else {
        CG_DEBUG("Error <%d@%s>: dest shape does match src shape\n", __LINE__,__FILE__);
        ret = -1;
    }
    return ret;
}

/**
 * @brief tensor 内部函数，请勿直接调用
 */
int sub_tensor_slice(sub_tensor_t* dest, sub_tensor_t* src, int slice_axes, int slice[])
{
    return __do_slice(dest, src, slice_axes, slice, 0);
}

/**
 * @brief tensor 内部函数，请勿直接调用
 */
int sub_tensor_padding(sub_tensor_t* dest, sub_tensor_t* src, int padding_axes, int padding[], padding_mode_t mode, cg_tensor_elem_type to_fill)
{
    return __do_padding(dest, src, padding_axes, padding, 0, mode, to_fill);
}

/**
 * @brief tensor 内部函数，请勿直接调用
 */
int sub_tensor_binary_opt(sub_tensor_t* dest, sub_tensor_t* t1, sub_tensor_t *t2, int opt_axes_t1, int opt_axes_t2, int (*opt)(sub_tensor_t* sub_dest, sub_tensor_t* sub_t1, sub_tensor_t* sub_t2))
{
    int batch_gap = (AXIS_AXES(t1->shape) - opt_axes_t1) - (AXIS_AXES(t2->shape) - opt_axes_t2);
    return __do_binary_opt(dest, t1, t2, 0, (AXIS_AXES(t1->shape) - opt_axes_t1), batch_gap, opt);
}

/**
 * @brief tensor 内部函数，请勿直接调用
 */
int sub_tensor_dot(sub_tensor_t* dest, sub_tensor_t* t1, sub_tensor_t* t2) 
{

    char* p_dest = dest->sub_elems;
    char* p_m1   = t1->sub_elems;
    char* p_m2   = t2->sub_elems;

    int    r1     = SHAPE_DIMENS(t1->shape, 0);
    int    c1     = SHAPE_DIMENS(t1->shape, 1);
    int    c2     = SHAPE_DIMENS(t2->shape, 1);

    for (int i=0; i<r1; ++i) {
        for (int j=0; j<c2; ++j) {
            for (int k=0; k<c1; ++k) {
                // p_dest[i][j] += p_m1[i][k] * p_m2[k][j];
                cg_tensor_elem_ref_opt( \
                    cg_tensor_elem_offset_addr(p_dest, i*r1+j), \
                    cg_tensor_elem_offset_addr(p_m1, i*c1+k),   \
                    cg_tensor_elem_offset_addr(p_m2, k*c2+j),   \
                    *, +=\
                );
            }
        }
    }
    return 0;
}

/**
 * @brief tensor 内部函数，请勿直接调用
 */
int sub_tensor_subtract(sub_tensor_t* dest, sub_tensor_t* t1, sub_tensor_t* t2)
{
    int elem_number   = AXIS_NUMBER(t1->shape);
    char* dest_base = dest->sub_elems;
    char* t1_base   = t1->sub_elems;
    char* t2_base   = t2->sub_elems;

    for (int i=0; i<elem_number; ++i) {
        cg_tensor_elem_ref_opt( \
            cg_tensor_elem_offset_addr(dest_base, i), \
            cg_tensor_elem_offset_addr(t1_base, i),   \
            cg_tensor_elem_offset_addr(t2_base, i),  \
            -, =\
        );
    }
    return 0;
}

/**
 * @brief tensor 内部函数，请勿直接调用
 */
int sub_tensor_add(sub_tensor_t* dest, sub_tensor_t* t1, sub_tensor_t* t2)
{
    int elem_number   = AXIS_NUMBER(t1->shape); 
    char* dest_base = dest->sub_elems;
    char* t1_base   = t1->sub_elems;
    char* t2_base   = t2->sub_elems;

    for (int i=0; i<elem_number; ++i) {
        cg_tensor_elem_ref_opt( \
            cg_tensor_elem_offset_addr(dest_base, i), \
            cg_tensor_elem_offset_addr(t1_base, i), \
            cg_tensor_elem_offset_addr(t2_base, i), 
            +, =\
        );
    }
    return 0;
}

/**
 * @brief tensor 内部函数，请勿直接调用
 */
int sub_tensor_multiply(sub_tensor_t* dest, sub_tensor_t* t1, sub_tensor_t* t2)
{
    int elem_number = AXIS_NUMBER(t1->shape); 
    char* dest_base = dest->sub_elems;
    char* t1_base   = t1->sub_elems;
    char* t2_base   = t2->sub_elems;

    for (int i=0; i<elem_number; ++i) {
        //cg_tensor_elem_ref_opt(dest_base+i*tensor_elem_size, t1_base+i*tensor_elem_size, t2_base+i*tensor_elem_size, *);
        cg_tensor_elem_ref_opt( \
            cg_tensor_elem_offset_addr(dest_base, i), \
            cg_tensor_elem_offset_addr(t1_base, i),   \
            cg_tensor_elem_offset_addr(t2_base, i),  \
            *, = \
        );
    }
    return 0;
}

/**
 * @brief tensor 内部函数，请勿直接调用
 */
int sub_tensor_T(sub_tensor_t* dest, sub_tensor_t* t1)
{
    if ( SHAPE_DIMENS(t1->shape, 0) == 1 || SHAPE_DIMENS(t1->shape, 1) == 1) {
        // 如果 x 或者 y 轴其中一轴为 1，那么可以直接将数据 copy 给 dest。
        return sub_tensor_to_sub(dest, t1);
    } else {
        // 
        char* dest_base = dest->sub_elems;
        char* t1_base   = t1->sub_elems;

        int dest_rows   = SHAPE_DIMENS(dest->shape, 0);
        int dest_cols   = SHAPE_DIMENS(dest->shape, 1);

        int t1_rows     = SHAPE_DIMENS(t1->shape, 0);
        int t1_cols     = SHAPE_DIMENS(t1->shape, 1);

        for (int i=0; i<t1_rows; ++i) {
            for (int j=0; j<t1_cols; ++j) {
                //cg_tensor_elem_ref_assign(dest_base+(j*dest_cols+i)*tensor_elem_size, t1_base+(i*t1_cols+j)*tensor_elem_size);
                cg_tensor_elem_ref_assign(cg_tensor_elem_offset_addr(dest_base, j*dest_cols+i), cg_tensor_elem_offset_addr(t1_base, i*t1_cols+j));
            }
        }
    }
    return 0;
}

/**
 * @brief tensor 内部函数，请勿直接调用
 */
int sub_tensor_fill(sub_tensor_t* dest, cg_tensor_elem_type fill)
{
    int elem_number = AXIS_NUMBER(dest->shape); //dest->sub_dimens[0] * dest->sub_stride[0];
    for (int i=0; i<elem_number; ++i) {
        //cg_tensor_elem_number_assign(((char*)(dest->sub_elems))+(i*tensor_elem_size), fill);
        cg_tensor_elem_number_assign(cg_tensor_elem_offset_addr(dest->sub_elems, i), fill);
    }
    return 0;
}

/**
 * @brief tensor 内部函数，请勿直接调用
 */
int sub_tensor_arange(sub_tensor_t* dest, cg_tensor_elem_type from, cg_tensor_elem_type to)
{
    char* p_t = dest->sub_elems;
    //float  per = (to - from) / TENSOR_NUM(t);
    cg_tensor_elem_var_def(per);
    cg_tensor_elem_var_def(gap);

    int sub_tensor_num = AXIS_NUMBER(dest->shape); //dest->sub_dimens[0] * dest->sub_stride[0];

    // per = to - from
    cg_tensor_elem_number_opt(per, to, from, -, =);
    // per = pre / TENSOR_NUM(t)
    cg_tensor_elem_rn_opt(per, per, sub_tensor_num, /, =);
    
    for (int i=0; i<sub_tensor_num; ++i) {
        //p_t[i] = i*per + from;
        // gap = pre * i
        cg_tensor_elem_rn_opt(gap, per, i, *, =);
        // p_t[i] = gap + from
        //cg_tensor_elem_rn_opt(p_t + (i*cg_tensor_elem_size), gap, from, +);
        cg_tensor_elem_rn_opt(cg_tensor_elem_offset_addr(p_t, i), gap, from, +, =);
    }
    return 0;
}

/**
 * @brief tensor 内部函数，请勿直接调用
 */
int sub_tensor_setzero(sub_tensor_t* dest)
{
    int size = AXIS_NUMBER(dest->shape) * cg_tensor_elem_size;
    memset(dest->sub_elems, 0, size);
    return 0;
}

