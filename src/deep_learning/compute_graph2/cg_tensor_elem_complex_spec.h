#ifndef __CG_TENSOR_ELEM_COMPLEX_SPEC_H__
#define __CG_TENSOR_ELEM_COMPLEX_SPEC_H__

typedef struct cg_complex {
    float real;
    float img;
} cg_complex_t;

typedef cg_complex_t cg_tensor_elem_type;

#define cg_tensor_elem_size (sizeof(cg_complex_t))

#endif