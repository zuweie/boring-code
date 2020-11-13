/*
 * @Author: your name
 * @Date: 2020-10-22 13:30:59
 * @LastEditTime: 2020-11-13 11:03:53
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
   return sum % slot_t;
}

static inline 
float get(imatrix_t* matrix_ptr, size_t x, size_t y) 
{
    CooMatrix* coomatrix = (CooMatrix*)matrix_ptr;
    Tv v;
    if (Map_get2(coomatrix->coo, x, y, v) ==0)
    {
        return t2f(v);
    }
    return 0.0f;
    
}

static inline 
int set(imatrix_t* matrix_ptr, size_t x, size_t y, float v) 
{
    CooMatrix* coomatrix = (CooMatrix*) matrix_ptr;
    Map_set2(coomatrix->coo, i2t(x), i2t(y), f2t(v));
    return 0;
}

static 
int trans(imatrix_t* matrix_ptr) 
{
    return 0;
}

static 
void get_row(imatrix_t* matrix_ptr, size_t row_index, float data[]) 
{
    return;
}

static 
void get_col(imatrix_t* matrix_ptr, size_t col_index, float data[]) 
{
    return;
}

CooMatrix* CooMatrix_create(size_t rows, size_t cols) 
{
    CooMatrix* matrix = malloc(sizeof(CooMatrix));
    matrix->coo     = _Hashmap(Keyhasher);
    initialize_matrix(matrix, get, set, get_row, get_col, trans, rows, cols);
    return matrix;
}

CooMatrix* CooMatrix_load(size_t rows, size_t cols, float* data)
{
    CooMatrix* matrix = CooMatrix_create(rows, cols);
    float(*raw)[Matrix_cols(matrix)] = data;

    for (int i=0; i<Matrix_rows(matrix); ++i) {
        for (int j=0; j<Matrix_cols(matrix); ++j) {
            if (raw[i][j] != 0.0) {
                Matrix_set(matrix, i, j, raw[i][j]);
            }
        }
    }
    return matrix;
}

int CooMatrix_destroy(CooMatrix* matrix) 
{
    Hashmap_(matrix->coo);
    free(matrix);
    return 0;
}
