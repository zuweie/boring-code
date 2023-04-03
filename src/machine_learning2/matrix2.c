#include <stdlib.h>
#include <string.h>
#include "matrix2_operator.h"
#include "matrix2.h"

matrix2_t* Mat2_create(size_t rows, size_t cols)
{
    matrix2_t* mat = (matrix2_t*)malloc(sizeof(matrix2_t));

    mat->rows = rows;
    mat->cols = cols;

    mat->pool = (vfloat_t**) malloc(rows*cols*sizeof(vfloat_t));
    return mat;
}

matrix2_t* Mat2_create_cpy(matrix2_t* mat)
{
    matrix2_t* mat2 = Mat2_create(mat->rows, mat->cols);
    memcpy(mat2->pool, mat->pool, mat->rows*mat->cols*sizeof(vfloat_t));
    return mat2;
}

int Mat2_destroy(matrix2_t* mat)
{
    free(mat->pool);
    free(mat);
    return 0;
}

int Mat2_cpy(matrix2_t* dest, matrix2_t* src)
{
    dest->rows = src->rows;
    dest->cols = src->cols;
    dest->pool = (vfloat_t*) realloc(dest->pool, src->rows * src->cols * sizeof(vfloat_t));
    memcpy(dest->pool, src->pool, dest->rows * dest->cols * sizeof(vfloat_t));
    return 0;
}

int Mat2_slice_row_to(matrix2_t* dest, matrix2_t* src, int row_idx)
{
    return __mat2_rescale(
        &(dest->pool),
        &(dest->rows),
        &(dest->cols),
        src->pool,
        src->rows,
        src->cols,
        0,
        row_idx,
        0,
        -(src->rows - (row_idx + 1)),
        0
    );
}

int Mat2_slice_col_to(matrix2_t* dest, matrix2_t* src, int col_idx)
{
    return __mat2_rescale(
        &(dest->pool),
        &(dest->rows),
        &(dest->cols),
        src->pool,
        src->rows,
        src->cols,
        col_idx,
        0,
        -(src->cols-(col_idx+1)),
        0,
        0
    );
}


int Mat2_arange(matrix2_t* mat, vfloat_t from, vfloat_t to)
{
    size_t size_mat = mat->rows * mat->cols;

    vfloat_t per = (to - from ) / (vfloat_t) (size_mat-1);

    for (size_t i=0; i<size_mat; ++i) {
        mat->pool[i] = from + i * per;
    }

    return 0;
}

int Mat2_fill(matrix2_t* mat, vfloat_t v)
{
    size_t size_mat = mat->rows * mat->cols;
    
    for (size_t i=0; i<size_mat; ++i) {
        mat->pool[i] = v;
    }
}

int Mat2_vect_dot(matrix2_t* mat1, matrix2_t* mat2, vfloat_t* out)
{
    if (mat1->rows !=1 && mat1->cols !=  1)
        return -1;
    
    if (mat2->rows != 1 && mat2->cols != 1)
        return -1;

    size_t n1 = mat1->rows > mat1->cols ? mat1->rows : mat1->cols;
    size_t n2 = mat2->rows > mat2->cols ? mat2->rows : mat2->cols;
    
    if (n1 != n2)
        return -1;
    
    *out = __mat2_vect_dot(mat1->pool, mat2->pool, n1);
    return 0;
}


int Mat2_add(matrix2_t* dest, matrix2_t* src)
{
    return __mat2_add(
        &(dest->pool),
        &(dest->rows),
        &(dest->cols),
        src->pool,
        src->rows,
        src->cols
    );
}

int Mat2_sub(matrix2_t* dest, matrix2_t* src)
{
    return __mat2_sub(
        &(dest->pool),
        &(dest->rows),
        &(dest->cols),
        src->pool,
        src->rows,
        src->cols
    );
}
int Mat2_scalar_multiply(matrix2_t* mat, vfloat_t scalar)
{
    return __mat2_scalar_multiply(
        &(mat->pool),
        &(mat->rows),
        &(mat->cols),
        mat->pool,
        mat->rows,
        mat->cols,
        scalar
    );
}

vfloat_t Mat2_get(matrix2_t* mat, size_t i, size_t j)
{
    MAT2_POOL_PTR(mat, ptr);
    return ptr[i][j];
}

int Mat2_put(matrix2_t* mat, size_t i, size_t j, vfloat_t v)
{
    MAT2_POOL_PTR(mat, ptr);
    ptr[i][j] = v;
}

int Mat2_load_on_shape(matrix2_t* mat, vfloat_t* data, size_t rows, size_t cols) 
{
    mat->pool = realloc(mat->pool, rows * cols * sizeof(vfloat_t));
    mat->rows = rows;
    mat->cols = cols;
    memcpy(mat->pool, data, rows * cols * sizeof(vfloat_t));
    return 0;
}

int Mat2_padding_left(matrix2_t* mat, int offset, vfloat_t fill)
{
    vfloat_t* pool1 = (vfloat_t*) malloc(mat->rows * mat->cols * sizeof(vfloat_t));
    memcpy(pool1, mat->pool, mat->rows * mat->cols * sizeof(vfloat_t));
    __mat2_rescale(
        &(mat->pool),
        &(mat->rows),
        &(mat->cols),
        pool1,
        mat->rows,
        mat->cols,
        -offset,
        0,
        0,
        0,
        fill
    );
    free(pool1);
    return 0;
}

int Mat2_padding_top(matrix2_t* mat, int offset, vfloat_t fill)
{
    vfloat_t* pool1 = (vfloat_t*) malloc(mat->rows * mat->cols * sizeof(vfloat_t));
    memcpy(pool1, mat->pool, mat->rows * mat->cols * sizeof(vfloat_t));
    __mat2_rescale(
        &(mat->pool),
        &(mat->rows),
        &(mat->cols),
        pool1,
        mat->rows,
        mat->cols,
        0,
        -offset,
        0,
        0,
        fill
    );
    free(pool1);
    return 0;
}
int Mat2_padding_right(matrix2_t* mat, int offset, vfloat_t fill)
{
    vfloat_t* pool1 = (vfloat_t*) malloc(mat->rows * mat->cols * sizeof(vfloat_t));
    memcpy(pool1, mat->pool, mat->rows * mat->cols * sizeof(vfloat_t));
    __mat2_rescale(
        &(mat->pool),
        &(mat->rows),
        &(mat->cols),
        pool1,
        mat->rows,
        mat->cols,
        0,
        0,
        offset,
        0,
        fill
    );
    free(pool1);
    return 0;
}
int Mat2_padding_bottom(matrix2_t* mat, int offset, vfloat_t fill)
{
    vfloat_t* pool1 = (vfloat_t*) malloc(mat->rows * mat->cols * sizeof(vfloat_t));
    memcpy(pool1, mat->pool, mat->rows * mat->cols * sizeof(vfloat_t));
    __mat2_rescale(
        &(mat->pool),
        &(mat->rows),
        &(mat->cols),
        pool1,
        mat->rows,
        mat->cols,
        0,
        0,
        0,
        offset,
        fill
    );
    free(pool1);
    return 0;
}
