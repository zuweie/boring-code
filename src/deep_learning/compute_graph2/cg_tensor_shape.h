/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-06-07 22:30:37
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-06-21 16:19:24
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_tensor_dimensions.h
 * @Description: 这是一个统一处理 tensor 的 shape 的处理函数。
 */
#ifndef __CG_TENSRO_SHAPE_H__
#define __CG_TENSRO_SHAPE_H__

// 从轴的角度看属性
#define AXIS_AXES(axis)   ((axis)->axes)
#define AXIS_DIMENS(axis) ((axis)->dimens)
#define AXIS_STRIDE(axis) ((axis)->stride)
#define AXIS_NUMBER(axis) (AXIS_DIMENS(axis) * AXIS_STRIDE(axis))

// 从shape的角度看属性
#define SHAPE_AXES(shape, i)   cg_tensor_shape_axes(shape, i)
#define SHAPE_DIMENS(shape, i) cg_tensor_shape_dimens(shape, i)
#define SHAPE_STRIDE(shape, i) cg_tensor_shape_stride(shape, i)
#define SHAPE_NUMBER(shape, i) (SHAPE_DIMENS(shape, i) * SHAPE_STRIDE(shape, i))

typedef struct cg_tensor_axis {

    int axes;
    int dimens;
    int stride;
    struct cg_tensor_axis* next;

} cg_tensor_axis_t;

static inline int cg_tensor_shape_axes(cg_tensor_axis_t* shape, int i) 
{
 while (i--) 
        shape = shape->next;
    return shape->axes;
}
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

static inline int cg_tensor_shape_get_dimens(cg_tensor_axis_t* shape, int axes, int dimension[]) 
{
    cg_tensor_axis_t* first = shape;
    for (int i=0; i<axes; ++i) {
        dimension[i] = shape->dimens;
        shape = shape->next;
    }
    return 0;
}

int cg_tensor_shape_create(cg_tensor_axis_t** first, int input_axes, int input_dimens[]);
int cg_tensor_shape_recycle(cg_tensor_axis_t* first);

int cg_tensor_shape_split_out(cg_tensor_axis_t** axis, int* cut_out, int coord_axes, int coordinate[]);
int cg_tensor_shape_inspect(cg_tensor_axis_t*);
#endif