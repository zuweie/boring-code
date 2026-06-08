/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-06-07 22:30:37
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-06-08 16:02:49
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_tensor_dimensions.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __CG_TENSRO_DIMENSIONS_H__
#define __CG_TENSRO_DIMENSIONS_H__

#include <stdlib.h>

#define CG_TENSOR_AXES(dimens)      ((dimens)[0])
#define CG_TENSOR_DIMENS(dimens, axis) ((dimens)[1+(axis)])
#define CG_TENSOR_STRIDE(dimens, axis) ((dimens)[1+(CG_TENSOR_AXES(dimens)+1)+(axis)])
#define CG_TENSOR_NUM(dimens, axis)    (CG_TENSOR_DIMENS(dimens, (axis)) * CG_TENSOR_STRIDE(dimens, axis))

static inline int* cg_tensor_create_dimensions(int axes, int* dimens) 
{
    int* dimension = NULL;

    if (axes >=0 ) {

        int i,j;

        int* dimension = (int*) malloc ( ((axes+1)*2+1) * sizeof(int) );
    
        // setup dimension axes;
        CG_TENSOR_AXES(dimension)         = axes;
        // setup dimens of 0 axis to 1, means all over the world is only itself.
        CG_TENSOR_DIMENS(dimension, 0)    = 1;
        // setup stride of last axis to 1, means the smallest unit of array is 1
        CG_TENSOR_STRIDE(dimension, axes) = 1;

        for (i=axes, j=axes-1; i>0; --i, --j) {
            CG_TENSOR_DIMENS(dimension, i) = dimens[i-1];
            CG_TENSOR_STRIDE(dimension, j) = CG_TENSOR_DIMENS(dimension, i) * CG_TENSOR_STRIDE(dimension, j+1);
        }
    }

    return dimension;
}

static inline int cg_tensor_dimension_recycle(int* cg_tensor_dimens) 
{
    free(cg_tensor_dimens);
    return 0;
}


#endif