/*
 * @Author: your name
 * @Date: 2020-10-22 13:30:59
 * @LastEditTime: 2020-10-22 16:19:44
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/matrix/CooMatrix.c
 */
#include "CooMatrix.h"

static inline 
float get(CooMatrix* matrix, size_t x, size_t y) 
{
    
}

static inline 
int set(CooMatrix* matrix, size_t x, size_t y, float v) 
{
    
}

CooMatrix* CooMatrix_create(size_t x, size_t y) 
{
    CooMatrix* matrix = (CooMatrix*) malloc (sizeof (CooMatrix) );
    Hashmap_init(matrix->matrix_table, NULL, NULL);
    initialize_matrix(matrix, get, set);
    return matrix;
}

CooMatrix* CooMatrix_load(size_t x, size_t y, float* data)
{
    
}

int CooMatrix_destroy(CooMatrix* matrix) {
    Hashmap_uninit(matrix->matrix_table, NULL);
    free(matrix);
}
