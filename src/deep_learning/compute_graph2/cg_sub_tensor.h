/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-03-15 08:15:48
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-03-28 22:39:13
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_sub_tensor.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __CG_SUB_TENSOR_H__
#define __CG_SUB_TENSOR_H__

typedef struct sub_tensor {
    void* sub_elems;
    int*  sub_stride;
    int*  sub_dimens;
    int   sub_axes;
    int   sub_elem_size;
} sub_tensor_t;

sub_tensor_t sub_tensor_get_sub(const sub_tensor_t* sub_t1, int axes, int* coord);
int sub_tensor_opt_scalar(const sub_tensor_t* sub, float scalar,  int (*opt)(cg_ref_t* dist, cg_ref_t e, float scalar));
int sub_tensor_opt(const sub_tensor_t* dist, const sub_tensor_t* sub_t1, const sub_tensor_t* sub_t2, int (*opt)(cg_ref_t dist, cg_ref_t e1, cg_ref_t e2));
int sub_tensor_dot(sub_tensor_t* dist, const sub_tensor_t* sub_t1, const sub_tensor_t* sub_t2, int (*opt)(cg_ref_t dist, cg_ref_t e1, cg_ref_t e2));
int sub_tensor_T(sub_tensor_t* dist, const sub_tensor_t* sub_tensor);

#endif