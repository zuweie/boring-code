#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include <math.h>
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

int Mat2_fill_random(matrix2_t* mat, double from, double to)
{
    srand(time(0));
    size_t mat_size = mat->rows * mat->cols;

    for (size_t i=0; i<mat_size; ++i)
        mat->pool[i] = from + (to - from) * (((double) rand()) / RAND_MAX);

    return 0;
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
    if (mat->rows * mat->cols != rows * cols) {
        mat->pool = realloc(mat->pool, rows * cols * sizeof(vfloat_t));
    }
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

int Mat2_is_vector(matrix2_t* mat) {
    return mat->cols == 1 || mat->rows == 1;
}

int Mat2_is_same_shape(matrix2_t* m1, matrix2_t* m2)
{
    return m1->rows == m2->rows && m1->cols == m2->cols;
}

int Mat2_is_symmetric(matrix2_t* mat) {

    if (mat->rows == mat->cols) {
        
        double eps = 1e-5;
        int n = mat->rows;
        
        MAT2_POOL_PTR(mat, mat_ptr);

        for (int i=0; i<n; ++i) {
            for (int j=i+1; j<n; ++j) {
                // 对角线的不用比
                if (fabs(mat_ptr[i][j] - mat_ptr[j][i]) > eps) return 0;
            }
        }
        return 1;
    }

    return 0;
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

int Mat2_hadamard_product(matrix2_t* mat1, matrix2_t* mat2)
{


    if (mat1->rows == mat2->rows && mat1->cols == mat2->cols) {
        return __mat2_hadamard_product(
            &(mat1->pool),
            &(mat1->rows),
            &(mat1->cols),
            mat1->pool,
            mat1->rows,
            mat1->cols,
            mat2->pool,
            mat2->rows,
            mat2->cols
        );
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

int Mat2_2I(matrix2_t* mat, size_t side)
{
    if (mat->rows * mat->cols != side * side) {
        mat->pool = realloc(mat->pool, side * side * sizeof(vfloat_t));
    }
    mat->rows = side;
    mat->cols = side;

    memset(mat->pool, 0, side * side * sizeof(vfloat_t));

    for (int i=0; i<side; ++i) {
        mat->pool[i*side + i] = 1.f;
    }
}

/**
 * @brief 融合两个矩阵，结果存放在 ma1 中多生成的行中
 * 
 * @param mat1 
 * @param mat2 
 * @return int 
 */
int Mat2_merge_rows(matrix2_t* mat1, matrix2_t* mat2)
{
    // 列数要相同才能合并。
    if (mat1->cols == mat2->cols) {
        // // 先扩充一下内存。
        // mat1->pool = realloc(mat1->pool, (mat1->rows + mat2->rows) * mat1->cols * sizeof(vfloat_t));
        // MAT2_POOL_PTR(mat1, m1_ptr);
        // memcpy(m1_ptr[mat1->rows], mat2->pool, (mat2->rows * mat2->cols * sizeof(vfloat_t)));
        // mat1->rows = mat1->rows + mat2->rows;
        // return 0;
        // vfloat_t* m_cpy = malloc (mat1->rows * mat1->cols * sizeof(vfloat_t));
        // size_t cpy_rows = mat1->rows;
        // size_t cpy_cols = mat1->cols;

        // memcpy(m_cpy, mat->pool, mat->rows * mat->cols * sizeof(vfloat_t));

        __mat2_merge_rows(
            &(mat1->pool),
            &(mat1->rows),
            &(mat1->cols),
            mat1->pool,
            mat1->rows,
            mat1->cols,
            mat2->pool,
            mat2->rows,
            mat2->cols
        );

        //free(m_cpy);
        return 0;
    }
    return -1;
}

/**
 * @brief 合并两个矩阵，把 mat2 合并到 mat1 多生成的列中
 * 
 * @param mat1 
 * @param mat2 
 * @return int 
 */
int Mat2_merge_cols(matrix2_t* mat1, matrix2_t* mat2) 
{
    if (mat1->rows == mat2->rows) {

        vfloat_t* m_cpy = malloc (mat1->rows * mat1->cols * sizeof(vfloat_t));
        size_t cpy_rows = mat1->rows;
        size_t cpy_cols = mat1->cols;

        memcpy(m_cpy, mat1->pool, mat1->rows * mat1->cols * sizeof(vfloat_t));

        __mat2_merge_cols(
            &(mat1->pool),
            &(mat1->rows),
            &(mat1->cols),
            m_cpy,
            cpy_rows,
            cpy_cols,
            mat2->pool,
            mat2->rows,
            mat2->cols
        );
        free(m_cpy);
        return 0;
    }
    return -1;
}

int Mat2_reshape(matrix2_t* mat, size_t new_rows, size_t new_cols)
{
    if (new_rows * new_cols > mat->rows * mat->cols) {
        mat->pool = realloc(mat->pool, new_rows * new_cols * sizeof(vfloat_t));
    } 
    mat->rows = new_rows;
    mat->cols = new_cols;
    return 0;
}

/**
 * @brief 矩阵的 QR 分解
 * 
 * @param q 正交矩阵
 * @param r 上三角矩阵
 * @param a 输入矩阵
 * @return int 结果
 */
int Mat2_qr(matrix2_t* q, matrix2_t* r, matrix2_t* a)
{
    return __mat2_qr_decomp(
        &(q->pool),
        &(q->rows),
        &(q->cols),
        &(r->pool),
        &(r->rows),
        &(r->cols),
        a->pool,
        a->rows,
        a->cols,
        0,
        a->rows < a->cols ? a->rows : a->cols
    );
}


/**
 * @brief 计算矩阵 m1 的特征向量
 * 
 * @param eigen_values 
 * @param eigen_vectors 
 * @param n 
 * @param m1 
 * @return int 
 */
int Mat2_eig(matrix2_t* eigvalue_mat, matrix2_t* eigvectors_mat, matrix2_t* m1)
{

    if (m1->rows == m1->cols) {

        // 如果是对称矩阵，直接使用 QR 算法，对角化矩阵，得到特征值，及特征向量。
        if (Mat2_is_symmetric(m1)) {

            int n = m1->rows;

            vfloat_t* a = NULL;
            size_t a_rows;
            size_t a_cols;

            __mat2_qr_alg(&a, &a_rows, &a_cols, &(eigvectors_mat->pool), &(eigvectors_mat->rows), &(eigvectors_mat->cols), m1->pool, m1->rows);

            // 组装特征值。
            eigvalue_mat->pool = realloc(eigvalue_mat->pool, n * sizeof(vfloat_t));
            eigvalue_mat->rows = 1;
            eigvalue_mat->cols = n;

            for (int i=0; i<n; ++i) {
                eigvalue_mat->pool[i] = a[i*n+i];
            }

            // printf("\nraw a:\n");
            // MAT2_RAW_INSPECT(a, a_rows, a_cols);
            free(a);

        } else {
            // 否则的话只能通过特征值一个个计算特征向量。
            //double eps = 1e-4;

            int n = m1->rows;

            //*eigvalue_mat = Mat2_create(1, n);

            __mat2_eigenvalues(&(eigvalue_mat->pool), m1->pool, n);
            eigvalue_mat->rows = 1;
            eigvalue_mat->cols = n;

            eigvectors_mat->pool = realloc(eigvectors_mat->pool, n*n*sizeof(vfloat_t));
            eigvectors_mat->rows = n;
            eigvectors_mat->cols = n;

            MAT2_POOL_PTR(eigvectors_mat, eigvectors_mat_ptr);

            vfloat_t* vec = NULL;

            for (int i=0; i<eigvalue_mat->cols; ++i) {
                
                __mat2_eigenvector(&vec, m1->pool, eigvalue_mat->pool[i], n);

                memcpy(eigvectors_mat_ptr[i], vec, n*sizeof(vfloat_t));
            }
            Mat2_T(eigvectors_mat);
            free(vec);
            return 0;
        }


    }
    return -1;
}

/**
 * @brief 计算 mat 的所有元素
 * 
 * @param mat 
 * @param axis 
 * @return double 
 */
int Mat2_sum(matrix2_t* mat, int axis)
{
    double sum = 0.f;
    MAT2_POOL_PTR(mat, mat_ptr);

    if (axis == 0) {

        for (int i=1; i<mat->rows; ++i) {
            for (int j=0; j<mat->cols; ++j) {
                mat_ptr[0][j] += mat_ptr[i][j];

            }
        }

        mat->rows = 1;
        
    } else if (axis == 1){

        for (int j=1; j<mat->cols; ++j) {
            for (int i=0; i<mat->rows; ++i) {
                mat_ptr[i][0] += mat_ptr[i][j];
            }
        }

        for (int k=1; k<mat->rows; ++k) {

            mat->pool[k] = mat_ptr[k][0];

        }
        mat->cols = 1;

    }
    return 0;
}

// 
double Mat2_norm(matrix2_t* mat)
{
    return __mat2_vect_norm(mat->pool, mat->rows * mat->cols);
}

int Mat2_export(matrix2_t* mat, void* buff)
{
    memcpy(buff, mat->pool, mat->rows * mat->cols * sizeof(vfloat_t));
    return 0;
}

int Mat2_import(matrix2_t* mat, void* buff)
{
    memcpy(mat->pool, buff, mat->rows * mat->cols * sizeof (vfloat_t));
    return 0;
}
