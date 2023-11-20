#ifndef __COUNTING_H__
#define __COUNTING_H__

#include "matrix2/matrix2.h"

#define CTY_size_ptr(countingY_ptr) ((int*)(countingY_ptr))
#define CTY_size(countingY_ptr) (*CTY_size_ptr(countingY_ptr))
#define CTY_elems_ptr(countingY_ptr) ((vfloat_t*)(&(CTY_size_ptr(countingY_ptr)[1])))
#define CTY_elems_number_ptr(countingY_ptr) ((int*)(&(CTY_elems_ptr(countingY_ptr)[CTY_size(countingY_ptr)])))

#define CTXY_row(countingXY_ptr) (((int*)(countingXY_ptr))[0])
#define CTXY_col(countingXY_ptr) (((int*)(countingXY_ptr))[1])
#define CTXY_counting_ptr(countingXY_ptr) ((char*)(&(((int*)(countingXY_ptr))[2])))

#define INSPECT_COUNTING(counting) \
({\
    int cty_size  = CTY_size(counting); \
    vfloat_t* elem_ptr = CTY_elems_ptr(counting);\
    int* number_ptr = CTY_elems_number_ptr(counting);\
    printf("\n <diff size: %d>: \n", cty_size);\
    for (int i=0; i<cty_size; ++i) {\
        vfloat_t target = elem_ptr[i];\
        int number = number_ptr[i];\
        printf("<%f: %d>\n", target, number);\
    }\
})


int counting_Y(matrix2_t* _y, void** countingY);
int counting_get_elem_number(void* counting, vfloat_t target);
int counting_get_elem_pos(void* counting, vfloat_t target);
vfloat_t counting_max_frequency(void* counting);

int counting_XY(matrix2_t* _y, matrix2_t* _X, void** countingXY_table);
int counting_free_XY_table(void** countingXY_table);

int counting_XY_group_by_x(matrix2_t* _X, matrix2_t* _y, int group_by, matrix2_t** group_X, matrix2_t** group_y, int* group_size);

#endif