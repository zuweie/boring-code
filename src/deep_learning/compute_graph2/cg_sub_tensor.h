/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-03-15 08:15:48
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-06-27 18:19:35
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_sub_tensor.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __CG_SUB_TENSOR_H__
#define __CG_SUB_TENSOR_H__

#include "cg_ref.h"
#include "cg_tensor_elem_spec.h"

typedef struct cg_tensor_axis cg_tensor_axis_t;

typedef enum padding_mode {
    pd_mode_fill = 0,
    pd_mode_edge
} padding_mode_t;

typedef struct sub_tensor {
    void*             sub_elems;
    cg_tensor_axis_t* shape;
} sub_tensor_t;

int sub_tensor_get_sub(sub_tensor_t* sub_tensor, sub_tensor_t* sub_src, int axes, int coord[]);

int sub_tensor_slice(sub_tensor_t* dest, sub_tensor_t* src, int slice_axes, int slice[]);

int sub_tensor_padding(sub_tensor_t* dest, sub_tensor_t* src, int padding_axes, int padding[], padding_mode_t mode, cg_tensor_elem_type to_fill);

int sub_tensor_fill(sub_tensor_t* dest, cg_tensor_elem_type fill);

int sub_tensor_arange(sub_tensor_t* dest, cg_tensor_elem_type from, cg_tensor_elem_type to);

int sub_tensor_to_sub(sub_tensor_t* dest,  sub_tensor_t* src);

int sub_tensor_T(sub_tensor_t* dest, sub_tensor_t* t1);

int sub_tensor_binary_opt(sub_tensor_t* dest, sub_tensor_t* t1, sub_tensor_t *t2, int opt_axes_t1, int opt_axes_t2, int (*opt)(sub_tensor_t* sub_dest, sub_tensor_t* sub_t1, sub_tensor_t* sub_t2));

#endif