/*
 * @Author: your name
 * @Date: 2020-10-22 13:30:59
 * @LastEditTime: 2020-11-11 15:15:37
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/matrix/CooMatrix.c
 */
#include "CooMatrix.h"
#include "container/HashMap.h"

static inline 
int Keyhasher(Tv v, size_t slot_t) 
{
   Entity* entity = t2p(v);
   size_t row = t2i(entity->tv[0]);
   size_t col = t2i(entity->tv[1]);
   size_t sum = row + col;
   sum = sum * (sum+1)/2 + row;
   return sum;
}

static inline 
float get(CooMatrix* matrix, size_t x, size_t y) 
{
    Tv v;
    if (Map_get2(matrix->coo, x, y, v) ==0)
    {
        return t2f(v);
    }
    return 0.0f;
    
}

static inline 
int set(CooMatrix* matrix, size_t x, size_t y, float v) 
{
    Map_set2(matrix->coo, x, y, v);
    return 0;
}

CooMatrix* CooMatrix_create(size_t col, size_t row) 
{
    CooMatrix* matrix = malloc(sizeof(CooMatrix));
    matrix->columns = col;
    matrix->rows    = row;
    matrix->coo     = _Hashmap(Keyhasher);
    initialize_matrix(matrix, get, set);
    return matrix;
}

CooMatrix* CooMatrix_load(size_t x, size_t y, float* data)
{
    CooMatrix* matrix = CooMatrix_create(x, y);
    float(*raw)[y] = data;

    for (int i=0; i<x; ++i) {
        for (int j=0; j<y; ++j) {
            if (raw[i][j] != 0.0) {
                Matrix_set(matrix, x, y, raw[i][j]);
            }
        }
    }
    return matrix;
}

int CooMatrix_destroy(CooMatrix* matrix) 
{
    Hashmap_(matrix->coo);
    free(matrix);
}
