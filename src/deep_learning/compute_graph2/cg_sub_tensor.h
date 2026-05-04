/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-03-15 08:15:48
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-05-04 15:06:55
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_sub_tensor.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __CG_SUB_TENSOR_H__
#define __CG_SUB_TENSOR_H__

typedef struct cg_elem_spec cg_elem_spec_t;

typedef struct sub_tensor {
    void* sub_elems;
    int*  sub_stride;
    int*  sub_dimens;
    int   sub_axes;
    cg_elem_spec_t* sub_elem_spec;
} sub_tensor_t;

sub_tensor_t sub_tensor_get_sub(const sub_tensor_t* sub_t1, int axes, int* coord);
int sub_tensor_slice(sub_tensor_t* dist, sub_tensor_t* src, int slice_dimens[]);
int sub_tensor_padding(sub_tensor_t* dist, sub_tensor_t* src, int padding_dimens[], cg_ref_t fill);
int sub_tensor_foreach(sub_tensor_t* dest, cg_ref_t opt1, cg_ref_t opt2, elem_opt_t elem_opt)

int sub_tensor_elem_number(sub_tensor_t* sub_tensor);
int sub_tensor_to_sub(sub_tensor_t* dist, const sub_tensor_t* src);
// int sub_tensor_opt_scalar(const sub_tensor_t* sub, float scalar,  int (*opt)(cg_ref_t* dist, cg_ref_t e, float scalar));
// int sub_tensor_opt(const sub_tensor_t* dist, const sub_tensor_t* sub_t1, const sub_tensor_t* sub_t2, int (*opt)(cg_ref_t dist, cg_ref_t e1, cg_ref_t e2));
// int sub_tensor_dot(sub_tensor_t* dist, const sub_tensor_t* sub_t1, const sub_tensor_t* sub_t2, int (*opt)(cg_ref_t dist, cg_ref_t e1, cg_ref_t e2));
// int sub_tensor_T(sub_tensor_t* dist, const sub_tensor_t* sub_tensor);

#endif