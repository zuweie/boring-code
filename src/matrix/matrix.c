/*
 * @Author: your name
 * @Date: 2021-04-05 14:51:28
 * @LastEditTime: 2023-03-31 00:24:35
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/matrix/matrix.c
 */
#include <string.h>
#include <stdlib.h>
#include <math.h>
#include "matrix.h"

static vfloat_t
__vet_dot_vet(vfloat_t vet1[], vfloat_t vet2[], size_t n) 
{
    vfloat_t v = 0.f;
    for (size_t i=0; i<n; ++i) {
        v += vet1[i] * vet2[i];
    }
    return v;
}

static int 
__solve_l(matrix_t* lu, vfloat_t* Z, size_t n)
{
    Mat_eptr(lu, ptr);
    for (size_t i=1; i<n; ++i) {
        for (size_t j=0; j<i; ++j) {
            Z[i] = Z[i] - ptr[i][j] * Z[j];
        }
    }
    return 0;
}

static int 
__solve_u(matrix_t* lu, vfloat_t* X, size_t n)
{   
    Mat_eptr(lu, ptr);
    
    for (size_t k=0; k<n; ++k ) {
        size_t i = n-k-1;
        for (size_t j=i+1; j<n; ++j) {
            vfloat_t d1 = ptr[i][j];
            X[i] = X[i] - ptr[i][j] * X[j];
        }
        vfloat_t d2 = ptr[i][i];
        X[i] = X[i] / ptr[i][i];
    }
    return 0;
}

static int 
__solve_lu(matrix_t *lu, vfloat_t* Y, size_t n) 
{
    __solve_l(lu, Y, n);
    __solve_u(lu, Y, n);
    return 0;
}

static void 
__resize_pool(matrix_t* mat, size_t new_size) 
{
    if  (mat->pool_size < new_size) {
        void* new_pool = realloc (mat->pool, new_size);
        mat->pool = new_pool;
        mat->pool_size = new_size;
    }
    return;
}

matrix_t Mat_create(size_t rows, size_t cols)
{
    vfloat_t* elems = (vfloat_t*) malloc (sizeof(vfloat_t) * rows * cols);
    matrix_t mat = {
        .pool = elems,
        .rows  = rows,
        .cols = cols,
        .pool_size = rows * cols * sizeof(vfloat_t)
    };
    return mat;
}

matrix_t Mat_load(size_t rows, size_t cols, vfloat_t elems[])
{
    vfloat_t *_elems = (vfloat_t*) malloc (sizeof(vfloat_t) * rows * cols);
    memcpy(_elems, elems, sizeof(vfloat_t) * rows * cols);

    matrix_t mat = {
        .pool = _elems,
        .rows  = rows,
        .cols  = cols,
        .pool_size = rows * cols * sizeof(vfloat_t)
    };
    return mat;
}
int Mat_export(matrix_t* mat, vfloat_t buffer[])
{
    memcpy(buffer, mat->pool, sizeof(vfloat_t) * Mat_rows(mat) * Mat_cols(mat));
    return 0;
}

matrix_t Mat_create_cpy(matrix_t* mat)
{
    size_t rows = Mat_rows(mat);  
    size_t cols = Mat_cols(mat);

    vfloat_t *elems = (vfloat_t*) malloc (sizeof(vfloat_t) * rows * cols);
    memcpy( elems, mat->pool, sizeof(vfloat_t) * rows * cols );
    matrix_t matrix = {
        .pool = elems,
        .rows = rows,
        .cols = cols,
        .pool_size = rows * cols * sizeof(vfloat_t)
    };
    return matrix;
}

void Mat_set(matrix_t* mat, size_t _rows, size_t _cols, vfloat_t* _pool)
{
    mat->cols = _cols;
    mat->rows = _rows;
    mat->pool = _pool;
    mat->pool_size = _rows * _cols * sizeof(vfloat_t);
    return;
}

int Mat_destroy(matrix_t* mat)
{
    free(mat->pool);
}

int Mat_get_row(matrix_t* mat, size_t row_index, vfloat_t row[])
{
    Mat_eptr(mat, ptr);
    memcpy(row, ptr[row_index], sizeof(vfloat_t) * Mat_cols(mat));
    return 0;
}

int Mat_get_col(matrix_t* mat, size_t col_index, vfloat_t col[])
{
    Mat_eptr(mat, ptr);
    for (size_t i=0; i<Mat_rows(mat); ++i) {
        col[i] = ptr[i][col_index];
    }
    return 0;
}

int Mat_insert_row_by_value(matrix_t* mat, int picked, vfloat_t v)
{
    // 往下移动一行
    Mat_move_rows(mat, picked, 1);
    Mat_eptr(mat, ptr);
    for (size_t i=0; i<Mat_cols(mat); ++i) {
        ptr[picked][i] = v;
    }
}

int Mat_insert_row_by_arr(matrix_t* mat, int picked, vfloat_t arr[])
{
    Mat_move_rows(mat, picked, 1);
    Mat_eptr(mat, ptr);
    for (size_t i=0; i<Mat_cols(mat); ++i) {
        ptr[picked][i] = arr[i];
    }
}

int Mat_insert_col_by_value(matrix_t* mat, int picked, vfloat_t v)
{
    Mat_move_cols(mat, picked, 1);
    Mat_eptr(mat, ptr);
    for (size_t i=0; i<Mat_rows(mat); ++i){
        ptr[i][picked] = v;
    }
}

int Mat_insert_col_by_arr(matrix_t* mat, int picked, vfloat_t arr[])
{
    Mat_move_cols(mat, picked, 1);
    Mat_eptr(mat, ptr);
    for (size_t i=0; i<Mat_rows(mat); ++i) {
        ptr[i][picked] = arr[i];
    }
}

int Mat_inverse(matrix_t* mat)
{
    size_t rows = Mat_rows(mat);
    size_t cols = Mat_cols(mat);

    vfloat_t inverse[rows][cols];
    Mat_lu(mat);

    for (size_t i=0; i<rows; ++i) {

        for (size_t j=0; j<cols; ++j) {
            if (i == j) inverse[i][j] = 1.f;
            else inverse[i][j] = 0.f;
        }
        
        __solve_lu(mat, inverse[i], cols);
    }
    // 计算完结果将其覆盖到原来的矩阵当中去
    memcpy(mat->pool, inverse, rows * cols * sizeof(vfloat_t));
    // 必须做转置才能还原逆矩阵
    Mat_transpose(mat);
    return 0;
}

int Mat_pseudo_inverse(matrix_t* mat)
{
    matrix_t mat1 = Mat_create_cpy(mat);
    Mat_transpose(mat);
    matrix_t mat_t_cpy = Mat_create_cpy(mat);
    Mat_dot(mat, &mat1);
    Mat_inverse(mat);
    Mat_dot(mat, &mat_t_cpy);
    Mat_destroy(&mat1);
    Mat_destroy(&mat_t_cpy);
    return 0;
}

int Mat_solve(matrix_t* mat, vfloat_t Y[], size_t n)
{
    Mat_lu(mat);
    __solve_lu(mat, Y, n);
    return 0;
}

int Mat_lu(matrix_t* mat)
{
    size_t rows = Mat_rows(mat);
    size_t cols = Mat_cols(mat);

    size_t s = rows < cols ? rows : cols;

    Mat_eptr(mat, ptr);

    for (size_t k=0; k<s; ++k) {
        vfloat_t x = 1.0f / ptr[k][k];
        
        for (size_t i=k+1; i<rows; ++i) {
            ptr[i][k] = ptr[i][k] * x;
        }

        for (size_t i=k+1; i<rows; ++i) {
            for (size_t j=k+1; j<cols; ++j) {
                ptr[i][j] = ptr[i][j] - ptr[i][k] * ptr[k][j];
            }
        }
    }
    return 0;
}

int Mat_dot(matrix_t* mat1, matrix_t* mat2)
{
    size_t rows_1 = Mat_rows(mat1);
    size_t cols_1 = Mat_cols(mat1);

    size_t rows_2  = Mat_rows(mat2);
    size_t cols_2  = Mat_cols(mat2);

    if (cols_1 == rows_2) {

        vfloat_t elems_1[rows_1][cols_1];
        Mat_export(mat1, elems_1);

        if (cols_2 > cols_1) {
            // 需要扩大内存。
            size_t new_size = rows_1 * cols_2;
            __resize_pool(mat1, new_size);
        }
        Mat_cols(mat1) = cols_2;

        Mat_eptr(mat1, ptr1);
        for (size_t i=0; i<rows_1; ++i) {

            for (size_t j=0; j<cols_2; ++j) {

                vfloat_t col_2[rows_2];
                Mat_get_col(mat2, j, col_2);
                ptr1[i][j] = __vet_dot_vet(elems_1[i], col_2, rows_2);

            }
        }
        return 0;
    }
    return -1;
}

int Mat_transpose(matrix_t* mat)
{
    size_t rows = Mat_rows(mat);
    size_t cols = Mat_cols(mat);
    vfloat_t col_buffer[rows];
    vfloat_t elems_buffer[cols][rows];
    for (size_t i=0; i<cols; ++i) {
        Mat_get_col(mat, i, col_buffer);
        memcpy(elems_buffer[i], col_buffer, sizeof(vfloat_t) * rows);
    }
    memcpy(mat->pool, elems_buffer, sizeof(vfloat_t) * rows * cols);
    Mat_rows(mat) = cols;
    Mat_cols(mat) = rows;
    return 0;
}

int Mat_move_rows(matrix_t* mat, int picked, int step)
{
    size_t rows = Mat_rows(mat);
    size_t cols = Mat_cols(mat);
    int start = picked < 0 ? rows + picked : picked;
    
    if (step > 0) {

        size_t new_size  = step * sizeof(vfloat_t) * cols + sizeof(vfloat_t) * rows * cols;
        __resize_pool(mat, new_size);
        
        Mat_eptr(mat, ptr);

        for (int i = rows-1; i>=start; --i) {
            memcpy(ptr[i+step], ptr[i], sizeof(vfloat_t) * cols);
        } 

    } else if (step < 0){
        // step < 0;
        int _step = step;
        if (_step + start < 0) {
            // |start| < |step|
            start = abs(_step);
        } 
        // |start| >= |step|
        _step = abs(_step);
        int move_chunk = (int)rows - start;
        
        Mat_eptr(mat, ptr);
        
        for (int i=start; move_chunk > 0 && i<start+move_chunk; ++i) {
           memcpy(ptr[i-_step], ptr[i], sizeof(vfloat_t) * cols);
        }
    }
    Mat_rows(mat) = step + (int)(Mat_rows(mat)) < 0 ? 0 : step + Mat_rows(mat);
    return 0;
}

int Mat_move_cols(matrix_t* mat, int picked, int step)
{
    Mat_transpose(mat);
    Mat_move_rows(mat, picked, step);
    Mat_transpose(mat);
    return 0;
}

/**
 * @brief 以左上角(0,0)为原点坐标，向下与向右为正方向（也就是屏幕坐标系统），对矩阵进行形变，被压缩，则数据丢失，被拉伸则填入 fill 数据。
 * 
 * @param mat 
 * @param left   0，则不动，-x，则往左扩大，+x，则往右缩小
 * @param top    0，则不动，-y，则往上扩大，+y，则往下缩小
 * @param right  0，则不动，-x，则往左缩小，+x，则往右扩大
 * @param bottom 0，则不动，-y，则往上缩小，+y，则往下扩大
 * @param fill 如果有扩大填入数据
 * @return int 
 */
int Mat_rescale(matrix_t* mat, int left, int top, int right, int bottom, vfloat_t fill)
{
    int o_rows = mat->rows;
    int o_cols = mat->cols;
    
    int n_rows = o_rows - top  + bottom;
    int n_cols = o_cols - left + right;

    // 检查变形后形状有没有问题。
    if (n_rows > 0 && n_cols > 0) {
        // 没有问题，继续编码
        
        vfloat_t* dat_cpy = malloc(o_rows * o_cols * sizeof(vfloat_t));
        vfloat_t (*dat_cpy_ptr)[o_cols] = dat_cpy;

        Mat_export(mat, dat_cpy);
        Mat_reshape(mat, n_rows, n_cols);

        Mat_eptr(mat, mat_ptr);

        
        /* 拆分与填充数据 */
        for (int i=0; i<n_rows; ++i) {
            for (int j=0; j<n_cols; ++j) {
                
                // 计算 i 与 j 是否能映射到旧数据的坐标，如果不能映射则就只能填入 fill
                int dat_cpy_i = top   + i; 
                int dat_cpy_j = left + j;

                if ((dat_cpy_i>=0 && dat_cpy_i < o_rows) && (dat_cpy_j >=0 && dat_cpy_j < o_cols))
                    mat_ptr[i][j] = dat_cpy_ptr[dat_cpy_i][dat_cpy_j];
                else 
                    mat_ptr[i][j] = fill;
            }
        }

        free(dat_cpy);
        return 0;
    }
    
    return -1;
}



int Mat_fill(matrix_t* mat, vfloat_t fill) 
{
    Mat_eptr(mat, ptr);
    for (size_t i=0; i<Mat_rows(mat); ++i) {
        for (size_t j=0; j<Mat_cols(mat); ++j) {
            ptr[i][j] = fill;
        }
    }
    return 0;
}

int Mat_arange(matrix_t* mat, vfloat_t from, vfloat_t to)
{
    Mat_eptr(mat, ptr);
    size_t size_mat = Mat_rows(mat) * Mat_cols(mat);
    vfloat_t per = (to - from ) / (vfloat_t) (size_mat-1);

    for (size_t i=0; i<size_mat; ++i) {

        mat->pool[i] = from + i * per;

    }

    return 0;
}

int Mat_reshape(matrix_t* mat, size_t new_rows, size_t new_cols)
{
    if (new_rows <=0 || new_cols <=0) return -1;
    size_t new_size = new_rows * new_cols * sizeof(vfloat_t);
    __resize_pool(mat, new_size);
    mat->rows = new_rows;
    mat->cols = new_cols;
    return 0;
}

int Mat_reload(matrix_t* mat, size_t new_rows, size_t new_cols, vfloat_t* data) 
{
    new_rows = new_rows >0 ? new_rows: mat->rows;
    new_cols = new_cols >0 ? new_cols: mat->cols;

    size_t new_size = new_rows * new_cols * sizeof(vfloat_t);
    __resize_pool(mat, new_size);
    memcpy(mat->pool, data, new_size);
    mat->rows = new_rows;
    mat->cols = new_cols;
    return 0;
}

int Mat_save(matrix_t* mat, void* buf) 
{
    size_t size = mat->rows * mat->cols * sizeof(vfloat_t);
    memcpy(buf, mat->pool, size);
    return 0;
}
int Mat_op_mat(matrix_t* mat1, matrix_t* mat2, mat_op_t op)
{
    int len_mat1 = mat1->rows * mat1->cols;
    int len_mat2 = mat2->rows * mat2->cols;
    if (len_mat1 % len_mat2 == 0) {
        for (int i=0; i<len_mat1; ++i) {
            mat_op(mat1->pool[i], mat2->pool[i%len_mat2], op);
        }
        return 0;
    }
    return -1;
}

int Mat_deflate(matrix_t* mat, mat_dimen_t dimen, mat_op_t op)
{
    vfloat_t (*ptr)[mat->cols] = mat->pool;

    if (dimen == dimen_row) {

        for (int i=0; i<mat->cols; ++i) {
            for (int j=1; j<mat->rows; j++) {
                mat_op(ptr[0][i], ptr[j][i], op);
            }
        }
        mat->rows = 1;
    } else {
        for (int i=0; i<mat->rows; ++i) {
            for (int j=1; j<mat->cols; ++j) {
                mat_op(ptr[i][0], ptr[i][j], op);
            } 
        }
        int old_row = mat->rows;
        Mat_transpose(mat);
        mat->rows = old_row;
        mat->cols = 1;
    }
    return 0;
}

int Mat_op_numberic(matrix_t* mat, vfloat_t v, mat_op_t op)
{
    int len = mat->rows * mat->cols;
    for (int i=0; i<len; ++i) {
        mat_op(mat->pool[i], v, op);
    }
    return 0;
}


int Mat_vector_dot(matrix_t* m1, matrix_t* m2, vfloat_t* out) 
{
    if (m1->rows != 1 || m1->cols != 1)
        return -1;

    if (m2->rows != 1 || m2->cols != 1)
        return -1;
    
    int m1_longest_dimens = m1->rows > m1->cols ? m1->rows : m1->cols;
    int m2_longest_dimens = m2->rows > m2->cols ? m2->rows : m2->cols;

    if (m1_longest_dimens != m2_longest_dimens)
        return -1;

    *out = __vet_dot_vet(m1->pool, m2->pool, m1_longest_dimens);
    
    return 0;
}

int Mat_copy(matrix_t* dest, matrix_t* src)
{
    return Mat_reload(dest, src->rows, src->cols, src->pool);
}

