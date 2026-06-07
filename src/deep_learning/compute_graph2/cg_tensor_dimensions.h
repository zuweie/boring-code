/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-06-07 22:30:37
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-06-08 00:58:57
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_tensor_dimensions.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __CG_TENSRO_DIMENSIONS_H__
#define __CG_TENSRO_DIMENSIONS_H__

#include <stdlib.h>

#define CG_TENSOR_AXES(dimens)      ((dimens)[0])
#define CG_TENSOR_DIMENS(dimens, i) (CG_TENSOR_AXES(dimens) > 0 ? (dimens)[1+(i)] : 1)
#define CG_TENSOR_STRIDE(dimens, i) (CG_TENSOR_AXES(dimens) > 0 ? (dimens)[CG_TENSOR_AXES(dimens)+1+(i)] : 1)
#define CG_TENSOR_NUM(dimens, i)    (CG_TENSOR_DIMENS(dimens, i) * CG_TENSOR_STRIDE(dimens, i))

static inline int* cg_tensor_create_dimensions(int axes, int dimens[]) 
{
    
}

static inline int cg_tensor_dimension_recycle(int* cg_tensor_dimens) 
{
    free(cg_tensor_dimens);
    return 0;
}

#endif