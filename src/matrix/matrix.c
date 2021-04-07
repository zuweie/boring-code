/*
 * @Author: your name
 * @Date: 2021-04-05 14:51:28
 * @LastEditTime: 2021-04-07 10:17:57
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/matrix/matrix.c
 */
#include <string.h>
#include <stdlib.h>
#include "matrix.h"

static float
__vet_dot_vet(float vet1[], float vet2[], size_t n) 
{
    float v = 0.f;
    for (size_t i=0; i<n; ++i) {
        v += vet1[i] * vet2[i];
    }
    return v;
}




matrix_t Mat_create(size_t rows, size_t cols)
{
    float* elems = (float*) malloc (sizeof(float) * rows * cols);
    matrix_t mat = {
        .elems = elems,
        .rows  = rows,
        .cols = cols
    };
    return mat;
}

matrix_t Mat_load(size_t rows, size_t cols, float elems[])
{
    float *_elems = (float*) malloc (sizeof(float) * rows * cols);
    memcpy(_elems, elems, sizeof(float) * rows * cols);

    matrix_t mat = {
        .elems = _elems,
        .rows  = rows,
        .cols  = cols
    };
    return mat;
}
int Mat_copy_elems(matrix_t* mat, float buffer[])
{
    memcpy(buffer, mat->elems, sizeof(float) * Mat_rows(mat) * Mat_cols(mat));
    return 0;
}

matrix_t Mat_copy(matrix_t* mat)
{
    size_t rows = Mat_rows(mat);  
    size_t cols = Mat_cols(mat);

    float *elems = (float*) malloc (sizeof(float) * rows * cols);
    memcpy( elems, mat->elems, sizeof(float) * rows * cols );
    matrix_t matrix = {
        .elems = elems,
        .rows = rows,
        .cols = cols
    };
    return matrix;
}



int Mat_destroy(matrix_t* mat)
{
    free(mat->elems);
}

int Mat_get_row(matrix_t* mat, size_t row_index, float row[])
{
    Mat_eptr(mat, ptr);
    memcpy(row, ptr[row_index], sizeof(float) * Mat_cols(mat));
    return 0;
}

int Mat_get_col(matrix_t* mat, size_t col_index, float col[])
{
    Mat_eptr(mat, ptr);
    for (size_t i=0; i<Mat_rows(mat); ++i) {
        col[i] = ptr[i][col_index];
    }
    return 0;
}

int Mat_insert_row_by_value(matrix_t* mat, size_t i, float v)
{

}

int Mat_insert_row_by_arr(matrix_t* mat, size_t i, float arr[])
{

}

int Mat_insert_col_by_value(matrix_t* mat, size_t i, float v)
{

}

int Mat_insert_col_by_arr(matrix_t* mat, size_t i, float arr[])
{

}

int Mat_inverse(matrix_t* mat)
{
    
}

int Mat_pseudo_inverse(matrix_t* mat)
{

}

int Mat_solve(matrix_t* mat, float Y[])
{
    
}
int Mat_lu(matrix_t* mat)
{

}
int Mat_dot(matrix_t* mat1, matrix_t* mat2)
{
    size_t rows_1 = Mat_rows(mat1);
    size_t cols_1 = Mat_cols(mat1);

    size_t rows_2  = Mat_rows(mat2);
    size_t cols_2  = Mat_cols(mat2);

    if (cols_1 == rows_2) {

        float buffer[rows_1][cols_1];

        Mat_copy_elems(mat1, buffer);

        return 0;
    }

    return -1;
}

int Mat_transpose(matrix_t* mat)
{
    size_t rows = Mat_rows(mat);
    size_t cols = Mat_cols(mat);
    float col_buffer[rows];
    float elems_buffer[cols][rows];
    for (size_t i=0; i<cols; ++i) {
        Mat_get_col(mat, i, col_buffer);
        memcpy(elems_buffer[i], col_buffer, sizeof(float) * rows);
    }
    memcpy(mat->elems, elems_buffer, sizeof(float) * rows * cols);
    Mat_rows(mat) = cols;
    Mat_cols(mat) = rows;
    return 0;
}

int Mat_move_rows(matrix_t* mat, int picked, int step)
{
    size_t rows = Mat_rows(mat);
    size_t cols = Mat_cols(mat);
    int start = picked < 0 ? rows + picked : picked;
    Mat_eptr(mat, ptr);
    if (step > 0) {
        size_t ext_size = step * sizeof(float) * cols + sizeof(float) * rows * cols;
        mat->elems = realloc(mat->elems, ext_size);
        for (int i = rows-1; i>=start; --i) {
            memcpy(ptr[i+step], ptr[i], sizeof(float) * cols);
        }        
    } else if (step < 0){
        // step < 0;
        int _step = step;
        if (_step + start < 0) {
            start = -1 * (start + _step);
            _step = start + 1;
        } else {
            _step *= -1;
        }
        int move_chunk = rows - start;

        for (int i=start; move_chunk > 0 && i<start+move_chunk; ++i) {
            memcpy(ptr[i-_step], ptr[i], sizeof(float) * cols);
        }
    }
    Mat_rows(mat) = step + (int)(Mat_rows(mat)) < 0 ? 0 : step + Mat_rows(mat);
    return 0;
}

int Mat_move_cols(matrix_t* mat, int picked, int step, int debug_move, int debug_transfrom)
{
    Mat_transpose(mat);
    if (debug_move)
        Mat_move_rows(mat, picked, step);
    if (debug_transfrom)
        Mat_transpose(mat);
    return 0;
}

int Mat_fill(matrix_t* mat, float fill) 
{
    Mat_eptr(mat, ptr);
    for (size_t i=0; i<Mat_rows(mat); ++i) {
        for (size_t j=0; j<Mat_cols(mat); ++j) {

            ptr[i][j] = fill;

        }
    }
    return 0;
}

int Mat_arange(matrix_t* mat, float from, float to)
{
    Mat_eptr(mat, ptr);
    size_t size_mat = Mat_rows(mat) * Mat_cols(mat);
    float per = (to - from ) / (float) (size_mat-1);

    for (size_t i=0; i<size_mat; ++i) {

        mat->elems[i] = from + i * per;

    }

    return 0;
}
