#ifndef __MATRIX2_H__
#define __MATRIX2_H__
#include <stdlib.h>


typedef enum {
    mat2_int=0, // 整型
    mat2_flt,   // 浮点
    mat2_dbe,   // 双浮点
    mat2_cpx    // float 的复数。
} mat_dat_type_t;

typedef struct  {
    int elm_type;
    int pool_size;
    char** mat_data;
    int rows;
    int cols;
} mat2_t;

// 初始化函数。
#define __mat2(_rows, _cols, _elm_type) ({mat2_t mat = {.pool_size=0, .mat_data=NULL, .rows=(_rows), .cols=(_cols)}, .elm_type=(_elm_type)}; mat)

int mat2_alloc(mat2_t* mat2_ptr);
int mat2_attach(mat2_t* mat2_ptr, char** data);
int mat2_release(mat2_t* mat2_ptr);
int mat2_opt(mat2_t* mat2_ptr);

#endif