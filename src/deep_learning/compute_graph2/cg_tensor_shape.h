/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-06-07 22:30:37
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-06-19 14:07:25
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_tensor_dimensions.h
 * @Description: 这是一个统一处理 tensor 的 shape 的处理函数。
 */
#ifndef __CG_TENSRO_SHAPE_H__
#define __CG_TENSRO_SHAPE_H__

typedef struct cg_tensor_axis {

    int axes;
    int dimens;
    int stride;
    struct cg_tensor_axis* next;

} cg_tensor_axis_t;

static inline int cg_tensor_shape_dimens(cg_tensor_axis_t* shape, int i) 
{
    while (i--) 
        shape = shape->next;
    return shape->dimens;
}

static inline int cg_tensor_shape_stride(cg_tensor_axis_t* shape, int i) 
{
    while (i--) 
        shape = shape->next;
    return shape->stride;
}

static inline int cg_tensor_shape_number(cg_tensor_axis_t* shape) 
{
    return cg_tensor_shape_dimens(shhape, 0) * cg_tensor_shape_stride(shape, 0);
} 

int cg_tensor_shape_create(cg_tensor_axis_t** first, int input_axes, int input_dimens[]);
int cg_tensor_shape_recycle(cg_tensor_axis_t* first);

int cg_tensor_shape_split_out(cg_tensor_axis_t** axis, int* cut_out, int coord_axes, int coordinate[]);
int cg_tensor_shape_inspect(cg_tensor_axis_t*);
#endif