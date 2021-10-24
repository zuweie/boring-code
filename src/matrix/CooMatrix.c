/*
 * @Author: your name
 * @Date: 2020-10-22 13:30:59
 * @LastEditTime: 2021-10-24 09:43:25
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/matrix/CooMatrix.c
 */
#include "imatrix.h"
#include "CooMatrix.h"
#include "container/cn.h"
static inline 
int COOMATRIX_Keyhasher(Tv v, size_t slot_t) 
{
   Entity* entity = t2p(v);
   size_t row = t2i(entity->tv[0]);
   size_t col = t2i(entity->tv[1]);
   size_t sum = row + col;
   sum = sum * (sum+1)/2 + row;
   return sum % slot_t;
}

static inline 
vfloat_t get(imatrix_t* matrix_ptr, size_t x, size_t y) 
{
    CooMatrix* cooMatrix = (CooMatrix*)matrix_ptr;
    Tv v;
    if (Map_get2(&cooMatrix->coo, x, y, v) ==0)
    {
        return t2f(v);
    }
    return 0.0f;
    
}

static inline 
int set(imatrix_t* matrix_ptr, size_t x, size_t y, vfloat_t v) 
{
    CooMatrix* cooMatrix = (CooMatrix*) matrix_ptr;
    Map_set2(&cooMatrix->coo, i2t(x), i2t(y), f2t(v));
    return 0;
}


static 
int trans(imatrix_t* matrix_ptr) 
{
    CooMatrix* cooMatrix = (CooMatrix*)matrix_ptr;
    Map new_coo = _Hashmap(COOMATRIX_Keyhasher);
    
    for(It first = CN_first(&cooMatrix->coo);!It_equal(first, CN_tail(&cooMatrix->coo));first=It_next(first)) {
        Entity* pentity = Map_get_entity(&cooMatrix->coo, first);
        Map_set2(&new_coo, pentity->tv[1], pentity->tv[0], pentity->tv[2]);
    }
    
    // release the old coo
    Hashmap_(&cooMatrix->coo);
    cooMatrix->coo = new_coo;
    size_t o_row = matrix_ptr->rows;
    matrix_ptr->rows = matrix_ptr->cols;
    matrix_ptr->cols = o_row;
    return 0;
}

static 
void get_row(imatrix_t* matrix_ptr, size_t row_index, vfloat_t data[]) 
{
    return;
}

static 
void get_col(imatrix_t* matrix_ptr, size_t col_index, vfloat_t data[]) 
{
    return;
}

CooMatrix* CooMatrix_create(size_t rows, size_t cols) 
{
    CooMatrix* matrix = malloc(sizeof(CooMatrix));
    matrix->coo     = CN_create(HASH_MAP|);//_Hashmap(COOMATRIX_Keyhasher);
    initialize_matrix(matrix, get, set, trans, get_row, get_col, rows, cols);
    return matrix;
}

CooMatrix* CooMatrix_load(size_t rows, size_t cols, vfloat_t* data)
{
    CooMatrix* matrix = CooMatrix_create(rows, cols);
    vfloat_t(*raw)[Matrix_cols(matrix)] = data;

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
    Hashmap_(&matrix->coo);
    free(matrix);
    return 0;
}
