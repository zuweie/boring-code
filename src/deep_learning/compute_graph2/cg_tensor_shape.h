/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-06-07 22:30:37
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-06-14 22:04:12
 * @FilePath: /boring-code/src/deep_learning/compute_graph2/cg_tensor_dimensions.h
 * @Description: 这是一个统一处理 tensor 的 shape 的处理函数。
 */
#ifndef __CG_TENSRO_SHAPE_H__
#define __CG_TENSRO_SHAPE_H__
#include <stdlib.h>
#include "cg_debug.h"


static inline int* cg_tensor_shape_create(int input_axes, int input_dimens[]) 
{
    int ret = 0;
    int* dimension = NULL;
    int i,j;
    if (input_axes > 0) {

        for (i=0; i<input_axes; ++i) {
            if (input_dimens[i] <=0) {
                ret = -1;
                CG_DEBUG("Error <%d@%s>: invalid input_dimens[%d](%d)\n", __LINE__, __FILE__, i, input_dimens[i]);
                break;
            }
        }

        if (ret == 0) {

            int axes  = input_axes + 1;

            dimension         = (int*) malloc ( (axes*2+1) * sizeof (int) );
            dimension[0]      = axes;
            dimension[axes]   = 1;
            dimension[axes*2] = 1;

            for (i=axes-1, j=input_axes-1; i>=1; --i, --j) {
                dimension[i]      = input_dimens[j];
                dimension[axes+i] = dimension[axes+i+1] * dimension[i+1];
            }
        }

    } else {
        CG_DEBUG("Error <%d@%s>: invalid input_axes %d\n", __LINE__, __FILE__, input_axes);
    }
    
    return dimension;
}

static inline int cg_tensor_shape_recycle(int* cg_shape) 
{
    free(cg_shape);
    return 0;
}

static inline int cg_tensor_shape_axes(int* cg_shape, int axis_cursor)
{
    if (axis_cursor >= 0 && axis_cursor < cg_shape[0]) {
        return axis_cursor == cg_shape[0] - 1 ? 1 : cg_shape[0] - axis_cursor - 1;
    }

    if (axis_cursor < 0)
        CG_DEBUG("Error <%d@%s>: axis_cursor(%d) is invalid\n", __LINE__, __FILE__, axis_cursor);
    
    if (axis_cursor >= cg_shape[0])
        CG_DEBUG("Error <%d@%s>: axis_cursor(%d) is out of axes(%d) \n", __LINE__, __FILE__, axis_cursor, cg_shape[0]);

    return -1;
}

static inline int cg_tensor_shape_dimen(int* cg_shape, int axis_cursor, int axis_offset) 
{   
    if (axis_cursor >=0 && axis_offset >=0 && axis_cursor + axis_offset < cg_shape[0]) {
        return cg_shape[axis_cursor + axis_offset + 1]; 
    }
    
    if (axis_cursor < 0)
        CG_DEBUG("Error <%d@%s>: axis_cursor(%d) is invalid\n", __LINE__, __FILE__, axis_cursor);

    if (axis_cursor + axis_offset >= cg_shape[0])
        CG_DEBUG(\
            "Error <%d@%s>: explore scope(axis_cursor(%d) + axis_offset(%d) is out of axes(%d))\n", \
            __LINE__, __FILE__, axis_cursor, axis_offset, cg_shape[0]\
        );
   return -1;
}

static inline int cg_tensor_shape_stride(int* cg_shape, int axis_cursor, int axis_offset) 
{
    if (axis_cursor >=0 && axis_offset >=0 && axis_cursor + axis_offset < cg_shape[0]) 
        return cg_shape[ cg_shape[0] + axis_cursor + axis_offset + 1 ];
    
    if (axis_cursor < 0)
        CG_DEBUG("Error <%d@%s>: axis_cursor(%d) is invalid\n", __LINE__, __FILE__, axis_cursor);

    if (axis_cursor + axis_offset >= cg_shape[0])
        CG_DEBUG(\
            "Error <%d@%s>: explore scope(axis_cursor(%d) + axis_offset(%d) is out of axes(%d))\n", \
            __LINE__, __FILE__, axis_cursor, axis_offset, cg_shape[0]\
        );

    return -1;
}

static inline int cg_tensor_shape_split_out(int* cg_shape, int* axis_cursor, int* cut_out, int coord_axes, int coord[]) 
{
    // list all the ivalid case.

    if (*axis_cursor <0 || *axis_cursor >= cg_shape[0]) {
        CG_DEBUG("Error <%d@%s>: axis_cursor(%d) < 0 or axis_cursor(%d) >= axes(%d)>\n", __LINE__, __FILE__, *axis_cursor, *axis_cursor, cg_shape[0]);
        return -1;
    }

    if (coord_axes <= 0) {
        CG_DEBUG("Error <%d@%s>: coord_axes(%d) is invalid\n", __LINE__, __FILE__, coord_axes);
        return -1;
    }

    if (*axis_cursor + coord_axes > cg_shape[0])
    {
        CG_DEBUG("Error <%d@%s>: coord_axes(%d) out of scope \n", __LINE__, __FILE__, coord_axes);
        return -1;
    }

    for (int i=0; i<coord_axes; ++i) {

        
        if ( coord[i] < 0 || coord[i] >= ) {
            CG_DEBUG("Error <%d@%s>: coord[%d](%d) is out scope of axis(%d)(%d)\n",  \
                __LINE__, __FILE__, i, coord[i], i,  \
                
            );
            return -1
        }
    }

    // start to do the split job
    *cut_out = 0;
    
    for (int i=0; i<coord_axes; ++i) {
        *cut_out += coord[i] * cg_tensor_shape_stride(cg_shape, *axis_cursor, i);
    }

    *axis_cursor += coord_axes;
    
    if (*axis_cursor == )

    return -1;
    
}

static inline int cg_tensor_shape_inspact(int* cg_shape, int axis_cursor) 
{
    int axes = cg_tensor_shape_axes(cg_shape, axis_cursor);
    printf("\naxes: %d\n", axes);
    printf("dimens: ");
    for (int i=0; i<axes; ++i) {
        printf("%d, ", cg_tensor_shape_dimen(cg_shape, axis_cursor, i));
    }
    printf("\nstride: ");
    for (int i=0; i<axes; ++i) {
        printf("%d, ", cg_tensor_shape_stride(cg_shape, axis_cursor, i));
    }
    printf("\n");
}
#endif