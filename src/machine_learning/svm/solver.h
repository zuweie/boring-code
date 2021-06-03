/*
 * @Author: your name
 * @Date: 2021-06-03 13:43:31
 * @LastEditTime: 2021-06-03 14:52:18
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/machine_learning/svm/solver.h
 */

#ifndef _SVM_SOLVER_H_
#define _SVM_SOLVER_H_
#include "vtype/vfloat_type.h"

typedef struct _u_array u_array_t;
typedef struct _solver {
    
    int (*select_working_set)(int* out_i, int* out_j, int* );
    int (*calculate_rho)(vfloat_t, vfloat_t);
    int (*kernel_function)(vfloat_t*, vfloat_t*);
    int* alpha_status;
    int* C;
    
} solver_t;

#define Solver(select_func, cal_rho_func, len_alpha) \
    {( \
        solver_t s = { \
            .select_working_set = select_func, \
            .calculate_rho = cal_rho_func, \
            .alpha_staus = malloc (sizeof(int) * len_alpha) \
        }; \
        s; \
    )}

#define Solver_(psolver) \
    { \
        free((psolver)->alpha_status); \
    }



int update_alpha_status(solver_t*, u_array_t*);


#endif