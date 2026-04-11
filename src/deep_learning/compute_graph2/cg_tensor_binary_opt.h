#ifndef __CG_TENSOR_BINARY_OPT_H__
#define __CG_TENSOR_BINARY_OPT_H__

typedef struct cg_tensor cg_tensor_t;
typedef struct sub_tensor sub_tensor_t;

int cg_tensor_check_batch_dimens(cg_tensor_t* t1, cg_tensor_t* t2, int t1_batch_dimen_start);
int cg_tensor_binary_opt_resize_calculate(cg_tensor_t* t1, cg_tensor_t* t2, int* new_dimens, int* new_axes, int (*opt_resize_calculate)());
int cg_tensor_opt(cg_tensor_t* dist, cg_tensor_t* t1, cg_tensor_t* t2, int (*opt)())
#endif