/*
 * @Author: your name
 * @Date: 2020-10-22 13:30:59
 * @LastEditTime: 2021-10-24 15:55:08
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/matrix/CooMatrix.c
 */
#include "imatrix.h"
#include "container/entity.h"
#include "container/cn.h"
#include "base/type_value/__built_in_type.h"
#include "CooMatrix.h"
static inline 
int COOMATRIX_Keyhasher(T* v, size_t slot_t) 
{
   entity_t* entity = v;
   unsigned int row = ef_uint(entity, 0);
   unsigned int col = ef_uint(entity, 1);
   size_t sum = row + col;
   sum = sum * (sum+1)/2 + row;
   return sum % slot_t;
}

static inline 
vfloat_t get(imatrix_t* matrix_ptr, unsigned int x, unsigned int y) 
{
    CooMatrix* cooMatrix = (CooMatrix*)matrix_ptr;

    vfloat_t* v = CN_get(cooMatrix->coo, x, y);
    return v ? *v : 0.f;
}

static inline 
int set(imatrix_t* matrix_ptr, unsigned int x, unsigned int y, vfloat_t v) 
{
    CooMatrix* cooMatrix = (CooMatrix*) matrix_ptr;
    CN_set(cooMatrix->coo, x, y, v);
    return 0;
}


static 
int trans(imatrix_t* matrix_ptr) 
{
    CooMatrix* cooMatrix = (CooMatrix*)matrix_ptr;
    CN new_coo = CN_create(HASH_MAP|customized_entity, 3, 2, uint_t, uint_t, vf_t);
    for(It first = CN_first(cooMatrix->coo);!It_equal(first, CN_tail(cooMatrix->coo)); It_next(first)) {
        entity_t* ent = It_ptr(first);
        CN_set(new_coo, ef_uint(ent, 1), ef_uint(ent, 0), ef_vft(ent, 2));
    }
    
    CN_finalize(cooMatrix->coo, NULL);
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
    matrix->coo = CN_create(HASH_MAP|customized_entity|customized_hasher, 3, 2,  uint_t, uint_t, vf_t, &COOMATRIX_Keyhasher);
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
    //Hashmap_(&matrix->coo);
    CN_finalize(matrix->coo, NULL);
    free(matrix);
    return 0;
}
