#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "matrix2_operator.h"
#include "matrix2_count.h"
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
    return Mat2_slice_rows_to(dest, src, row_idx, row_idx+1);
}

int Mat2_slice_rows_to(matrix2_t* dest, matrix2_t* src, int begin, int open_end)
{
    return __mat2_rescale(
        &(dest->pool),
        &(dest->rows),
        &(dest->cols),
        src->pool,
        src->rows,
        src->cols,
        0,
        begin,
        0,
        -(src->rows - open_end),
        0
    );
}

int Mat2_slice_col_to(matrix2_t* dest, matrix2_t* src, int col_idx)
{
    return Mat2_slice_cols_to(dest, src, col_idx, col_idx+1);
}

int Mat2_slice_cols_to(matrix2_t* dest, matrix2_t* src, int begin, int open_end)
{
    return __mat2_rescale(
        &(dest->pool),
        &(dest->rows),
        &(dest->cols),
        src->pool,
        src->rows,
        src->cols,
        begin,
        0,
        -(src->cols - (open_end)),
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

int Mat2_load_csv(matrix2_t* mat, char* file_csv)
{
    FILE *file = fopen(file_csv, "r");
    if (!file) return -1;
    
    // TODO: 1 读取行数
    int row_count = 0;
    int col_count = 1;

    char line[4096];
    memset(line, 0, sizeof(line));

    char* read = fgets(line, sizeof(line), file);
    if (read) {

        int line_len = strlen(read);
        for (int i=0; i<line_len; ++i) {
            if (read[i] == ',') col_count++;
        }
        row_count +=1;
    }

    while (fgets(line, sizeof(line), file) ) row_count++;

    fseek(file, 0, SEEK_SET);

    
    // 改变内存大小。
    mat->rows = row_count;
    mat->cols = col_count;
    mat->pool = (vfloat_t*) realloc(mat->pool, row_count * col_count * sizeof(vfloat_t));
    int row_index = 0;
    int col_index = 0;

    while(fgets(line, sizeof(line), file) && row_index < row_count) {
        col_index = 0;
        
        char* token = strtok(line, ",");
        while (token && col_index < col_count) {
            float value = atof(token);
            mat->pool[row_index * col_count + col_index] = value;
            token = strtok(NULL, ",");
            col_index++;
        }
        row_index++;
    }

    fclose(file);

    // TODO: 2 读取列数。
    return 0;
}

int Mat2_list_difference_in_col(matrix2_t* mat, int col, void** out)
{
    matrix2_t* mat2 = Mat2_create(1,1);
    Mat2_slice_col_to(mat2, mat, col);

    __mat2_count_element(mat2->pool, mat2->rows, out);
    Mat2_destroy(mat2);
    return 0;
}

int Mat2_get_difference_number(void* diff, vfloat_t target) 
{
    return __mat2_get_element_number(diff, target);
}

int Mat2_is_vector(matrix2_t* mat) {
    return mat->cols == 1 || mat->rows == 1;
}


int Mat2_get_co_to(matrix2_t* dest, matrix2_t* src, int p, int q)
{
    return __mat2_co(
        &(dest->pool),
        &(dest->rows),
        &(dest->cols),
        src->pool,
        src->cols, 
        src->cols,
        p, 
        q
    );
}


int Mat2_det(matrix2_t* mat, vfloat_t* out) {

    if (mat->rows == mat->cols) {

        *out =  __mat2_determinant(mat->pool, mat->rows);
        return 0;
    }
    return -1;
}

int Mat2_get_adjoint_to(matrix2_t* dest, matrix2_t* src)
{
    if (src->rows == src->cols) {

        __mat2_adjoint(
            &(dest->pool),
            &(dest->rows),
            &(dest->cols),
            src->pool, 
            src->rows
        );
        return 0;
    }
    return -1;
}

/**
 * @brief 原地 inverse
 * 
 * @param mat 
 * @return int 
 */
int Mat2_inverse(matrix2_t* mat) {

    if (mat->rows == mat->cols) {

        vfloat_t* mat2 = malloc (mat->rows * mat->cols * sizeof(vfloat_t));
        memcpy(mat2, mat->pool, mat->rows * mat->cols * sizeof(vfloat_t));

        __mat2_inverse(
            &(mat->pool),
            &(mat->rows),
            &(mat->cols),
            mat2,
            mat->rows
        );
        free(mat2);
        return 0;
    }
    return -1;
}

int Mat2_dot(matrix2_t* mat1, matrix2_t* mat2)
{
    if (mat1->cols == mat2->rows) {

        vfloat_t* m1_cpy = malloc(mat1->rows * mat1->cols * sizeof(vfloat_t));
        size_t m1_cpy_rows = mat1->rows;
        size_t m1_cpy_cols = mat1->cols;

        memcpy(m1_cpy, mat1->pool, mat1->rows * mat1->cols * sizeof(vfloat_t));

        __mat2_dot(
            &(mat1->pool),
            &(mat1->rows),
            &(mat1->cols),
            m1_cpy,
            m1_cpy_rows,
            m1_cpy_cols,
            mat2->pool,
            mat2->rows,
            mat2->cols
        );

        free(m1_cpy);
        return 0;
    }
    return -1;
}

int Mat2_T(matrix2_t* mat) 
{
    if (mat->rows == 1 || mat->cols == 1) {
        // 若是只有一行或者一列。直接把行列数调转即可。
        size_t tmp = mat->rows;
        mat->rows  = mat->cols;
        mat->cols  = tmp;
        return 0;
    }

    vfloat_t* m_cpy = malloc (mat->rows * mat->cols * sizeof(vfloat_t));
    size_t cpy_rows = mat->rows;
    size_t cpy_cols = mat->cols;

    memcpy(m_cpy, mat->pool, mat->rows * mat->cols * sizeof(vfloat_t));

    __mat2_T(
        &(mat->pool),
        &(mat->rows),
        &(mat->cols),
        m_cpy,
        cpy_rows,
        cpy_cols
    );
    free(m_cpy);
    return 0;
}

// int Mat2_householder_transform(matrix2_t* mat1, matrix2_t* mat2)
// {

//     if (mat2->rows != mat2->cols && mat2->rows < 2) return -1;

//     return __mat2_householder_transform(
//         &(mat1->pool),
//         &(mat1->rows),
//         &(mat1->cols),
//         mat2->pool,
//         mat2->rows
//     );

// }