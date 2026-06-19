#include <stdlib.h>
#include "cg_debug.h"
#include "cg_tensor_shape.h"

int cg_tensor_shape_create(cg_tensor_axis_t** first, int input_axes, int input_dimens[])
{
    *first = NULL;
    cg_tensor_axis_t* axis0 = (cg_tensor_axis_t*) malloc (sizeof(cg_tensor_axis_t));

    *axis0 = (cg_tensor_axis_t) {
        .axes   = 0,
        .dimens = 1,
        .stride = 1,
        .next   = axis0
    };

    *first = axis0;

    int axes_count = input_axes;
    for (int i=0; i<input_axes; ++i) {
        axis0 = (cg_tensor_axis_t*) malloc (sizeof(cg_tensor_axis_t));
        *axis0 = (cg_tensor_axis_t) {
            .axes   = axes_count--,
            .dimens = input_dimens[i],
            .stride = (*first)->stride * (*first)->dimens,
            .next   = *first
        };
        *first = axis0;
    }
    return 0;
}

int cg_tensor_shape_recycle(cg_tensor_axis_t* first)
{
    cg_tensor_axis_t* del = first;

    while (del->axes) {
        
        first = del->next;
        free(del);
        del   = first;
    }
    free(del);
    return 0;
}

int cg_tensor_shape_split_out(cg_tensor_axis_t** axis, int* cut_out, int coord_axes, int coordinate[])
{
    int ret                 = 0;
    *cut_out                = 0;
    cg_tensor_axis_t* first = *axis;

    for (int i=0; i<coord_axes; ++i) {
        if (coordinate[i] >=0 && coordinate[i] < first->dimens) {
            
            *cut_out += coordinate[i] * first->stride;
            first     = first->next;

        } else {
            *cut_out = -1;
            return -1;
        }
    }
    *axis = first;
    return 0;
}

int cg_tensor_shape_inspect(cg_tensor_axis_t* axis)
{
    if (axis->axes == 0) {
        printf("\n axes: %d, dimens: %d, stride: %d\n", axis->axes, axis->dimens, axis->stride);
    } else {
        cg_tensor_axis_t* first = axis;
        printf("\n axes: %d\n", first->axes);
        printf("\n dimens: ");
        while (first->axes) {
            printf("%d, ", first->dimens);
            first = first->next;
        }
        first = axis;
        printf("\n stride: ");
        while (first->axes) {
            printf("%d, ", first->stride);
            first = first->next;
        }
        printf("\n");
        
    }
    
}
