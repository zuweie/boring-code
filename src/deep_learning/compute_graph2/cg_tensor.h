/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-05-24 09:57:43
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-06-28 13:27:40
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_tensor.h
 * @Description: 好难
 */
#ifndef __CG_TENSOR_H__
#define __CG_TENSOR_H__
#include "cg_ref.h"
#include "cg_tensor_elem_spec.h"
#include "cg_sub_tensor.h"

#define TENSOR_SIZE(tensor)  (AXIS_NUMBER((tensor)->shape) * cg_tensor_elem_size)

typedef struct cg_allocator cg_allocator_t;
typedef struct cg_elem_spec cg_elem_spec_t;
typedef struct cg_tensor_axis cg_tensor_axis_t;
typedef struct cg_tensor {
    cg_allocator_t*   allocator;
    void*             elems;
    cg_tensor_axis_t* shape;
} cg_tensor_t;

cg_tensor_t* cg_tensor_create(cg_allocator_t* alloc, int axes, ...);
cg_tensor_t* cg_tensor_create_cpy(cg_tensor_t* thiz);
int cg_tensor_reshape(cg_tensor_t* thiz, int axes, int shape[]);
int cg_tensor_recycle(cg_tensor_t* thiz);

cg_tensor_t* cg_tensor_slice(cg_tensor_t* thiz, int slice_axes, ...);
cg_tensor_t* cg_tensor_padding(cg_tensor_t* thiz, padding_mode_t mode, cg_tensor_elem_type fill, int padding_axes, ...);
cg_tensor_elem_type* cg_tensor_get(cg_tensor_t* thiz, ...);

int cg_tensor_fill(cg_tensor_t*, cg_tensor_elem_type);
int cg_tensor_arange(cg_tensor_t*, cg_tensor_elem_type, cg_tensor_elem_type);
int cg_tensor_inspect(cg_tensor_t*);

sub_tensor_t cg_tensor_get_sub (cg_tensor_t* thiz, int axes, ...);
sub_tensor_t cg_tensor_to_sub_tensor(cg_tensor_t* thiz);

int cg_tensor_to_tensor(cg_tensor_t*, cg_tensor_t*);
int cg_tensor_T(cg_tensor_t*);
int cg_tensor_load(cg_tensor_t*, const void*);

int cg_tensor_dot(cg_tensor_t* dest, cg_tensor_t* op1);
int cg_tensor_add(cg_tensor_t* dest, cg_tensor_t* op1);
int cg_tensor_substract(cg_tensor_t* dest, cg_tensor_t* op1);
int cg_tensor_multiply(cg_tensor_t* dest, cg_tensor_t* op1);


#endif