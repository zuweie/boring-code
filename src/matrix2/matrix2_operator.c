#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <string.h>
#include "matrix2_operator.h"
/**
 * @brief m2 dot m3，结果保存在 m1 指向的内存中。
 * 
 * @param m1 输出矩阵，参数须为 NULL 或者 malloc 的有效指针。
 * @param rows1 输出矩阵行数，参数须为有效指针
 * @param cols1 输出矩阵列数，参数须为有效指针
 * @param m2 点乘做矩阵
 * @param rows2 矩阵行数
 * @param cols2 矩阵列数
 * @param m3 点乘右矩阵
 * @param rows3 矩阵行数
 * @param cols3 矩阵列数
 * @return int 返回运行结果。
 */
int __mat2_dot(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, size_t rows2, size_t cols2, vfloat_t* m3, size_t rows3, size_t cols3)
{
    *rows1 = rows2;
    *cols1 = cols3;
    *m1 = (vfloat_t*)realloc(*m1, (*rows1) * (*cols1) * sizeof(vfloat_t));

    vfloat_t (*m1_ptr)[*cols1] = *m1;
    vfloat_t (*m2_ptr)[cols2] = m2;
    vfloat_t (*m3_ptr)[cols3] = m3;


    vfloat_t m3_col[rows3];

    for (int i=0; i<(*rows1); ++i) {
        for (int j=0; j<(*cols1); ++j) {

            m1_ptr[i][j] = 0.f;

            for (int k=0; k<cols2; ++k) {
                m1_ptr[i][j] += m2_ptr[i][k] * m3_ptr[k][j];
            }
        }
    }
    return 0;
}


/**
 * @brief m2 hadamard m3, 点积
 * 
 * @param m1 
 * @param rows1 
 * @param cols1 
 * @param m2 
 * @param rows2 
 * @param cols2 
 * @param m3 
 * @param rows3 
 * @param cols3 
 * @return int 
 */
int __mat2_hadamard_product(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, size_t rows2, size_t cols2, vfloat_t* m3, size_t rows3, size_t cols3)
{
    if ((*rows1) * (*cols1) < rows2 * cols2) {

        *m1 = (vfloat_t*) realloc (*m1, rows2 * cols2 * sizeof(vfloat_t));
    }

    *rows1 = rows2;
    *cols1 = cols2;

    vfloat_t (*m1_ptr)[*cols1] = *m1;
    vfloat_t (*m2_ptr)[cols2]  = m2;
    vfloat_t (*m3_ptr)[cols3]  = m3;

    for (int i=0; i<(*rows1); ++i) {
        for (int j=0; j<(*cols1); ++j) {
            m1_ptr[i][j] = m2_ptr[i][j] * m3_ptr[i][j];
        }
    }

    return 0;
    
}


/**
 * @brief 两个向量内积
 * 
 * @param v1 向量1
 * @param v2 向量2
 * @param n 向量维度
 * @return vfloat_t 返回点积的结果。
 */
vfloat_t __mat2_vect_dot(vfloat_t* v1, vfloat_t* v2, size_t n) 
{
    vfloat_t result = 0.f;
    for (int i=0; i<n; ++i) {
        result += v1[i] * v2[i];
    }
    return result;
}

/**
 * @brief m2 矩阵转置，输出置 m1
 * 
 * @param m1 输出矩阵，参数须为 NULL 或者 malloc 的有效指针
 * @param rows1 输出矩阵行数，参数须为有效指针
 * @param cols1 输出矩阵列数，参数须为有效指针
 * @param m2 输入矩阵
 * @param rows2 矩阵2的行数
 * @param cols2 矩阵2的列数
 * @return int 返回转置的结果
 */
int __mat2_T(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, size_t rows2, size_t cols2)
{
    *rows1 = cols2;
    *cols1 = rows2;
    *m1 = (vfloat_t*) realloc (*m1, (*rows1) * (*cols1) * sizeof(vfloat_t));
    vfloat_t (*m1_ptr)[*cols1] = *m1;
    vfloat_t (*m2_ptr)[cols2]  = m2;

    for (int i=0; i<(*rows1); ++i) {
        for (int j=0; j<(*cols1); ++j) {

            m1_ptr[i][j] = m2_ptr[j][i];
        }
    }
    return 0;
}

int __mat2_merge_rows(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, size_t row2, size_t cols2, vfloat_t* m3, size_t rows3, size_t cols3)
{

    size_t o_rows = *rows1;
    int padding_bottom = rows3;
    __mat2_rescale(
        m1,
        rows1,
        cols1,
        m2,
        row2,
        cols2,
        0,
        0,
        0,
        padding_bottom,
        0.f
    );
    
    vfloat_t (*m1_ptr)[*cols1] = *m1;
    vfloat_t (*m3_ptr)[cols3]  = m3;

    for (int i=o_rows, j=0; i<*rows1; ++i, ++j) {
        memcpy(m1_ptr[i], m3_ptr[j], *cols1 * sizeof(vfloat_t));
    }
    return 0;
}

int __mat2_merge_cols(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, size_t rows2, size_t cols2, vfloat_t* m3, size_t rows3, size_t cols3)
{
    size_t o_cols = *cols1;
    int padding_right = cols3;
    __mat2_rescale(
        m1,
        rows1,
        cols1,
        m2, 
        rows2,
        cols2,
        0, 
        0, 
        padding_right,
        0,
        0.f
    );

    vfloat_t (*m1_ptr)[*cols1] = *m1;
    vfloat_t (*m3_ptr)[cols3] = m3;

    for (int i=0; i<*rows1; ++i) {
        memcpy(&(m1_ptr[i][o_cols]), m3_ptr[i], cols3 * sizeof(vfloat_t));
    }
    return 0;
}

/**
 * @brief 以左上角(0,0)为原点坐标，向下与向右为正方向（也就是屏幕坐标系统），对矩阵进行形变，被压缩，则数据丢失，被拉伸则填入 fill 数据。这个是 slice 与 padding 的基础函数
 * 
 * @param left   0，则不动，-x，则往左扩大，+x，则往右缩小
 * @param top    0，则不动，-y，则往上扩大，+y，则往下缩小
 * @param right  0，则不动，-x，则往左缩小，+x，则往右扩大
 * @param bottom 0，则不动，-y，则往上缩小，+y，则往下扩大
 * @param fill 如果有扩大填入数据
 * @return int 返回操作结果
 */
int __mat2_rescale(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, size_t rows2, size_t cols2, int left, int top, int right, int bottom, vfloat_t fill)
{
    int o_rows = rows2;
    int o_cols = cols2;
    
    int n_rows = o_rows - top  + bottom;
    int n_cols = o_cols - left + right;

    // 检查变形后形状有没有问题。
    if (n_rows > 0 && n_cols > 0) {
        // 没有问题，继续编码
        
        // reshap m1 的内存。
        *rows1 = n_rows;
        *cols1 = n_cols;

        *m1 = (vfloat_t*)realloc(*m1, n_rows * n_cols * sizeof(vfloat_t));

        vfloat_t (*m1_ptr)[n_cols] = *m1;
        vfloat_t (*m2_ptr)[cols2]  = m2;
        
        /* 将 m2 的拆分与填充数据 */
        for (int i=0; i<n_rows; ++i) {
            for (int j=0; j<n_cols; ++j) {
                
                // 计算 i 与 j 是否能映射到旧数据的坐标，如果不能映射则就只能填入 fill
                int m2_i = top  + i; 
                int m2_j = left + j;

                if (( m2_i >= 0 && m2_i < o_rows) && ( m2_j >= 0 && m2_j < o_cols ))
                    m1_ptr[i][j] = m2_ptr[m2_i][m2_j];
                else 
                    m1_ptr[i][j] = fill;
            }
        }
        return 0;
    }
    return -1;
}


/**
 * @brief 两矩阵相加。
 * 
 * @param m1 矩阵1,相加后结果放入矩阵1。
 * @param rows1 矩阵1行数指针。
 * @param cols1 矩阵1列数指针。
 * @param m2 矩阵2,数据源。
 * @param rows2 矩阵2的行数
 * @param cols2 矩阵2的列数。
 * @return int 操作结果
 */
int __mat2_add(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, size_t rows2, size_t cols2)
{
   if (*rows1 == rows2 && *cols1 == cols2) {

        vfloat_t (*m1_ptr)[*cols1] = *m1;
        vfloat_t (*m2_ptr)[cols2]  = m2;
        for (int i=0; i<(*rows1); ++i) {
            for (int j=0; j<(*cols1); ++j) {
                m1_ptr[i][j] += m2_ptr[i][j];
            }
        }

        return 0;
   } 
   return -1;
}

/**
 * @brief 将矩阵相减
 * 
 * @param m1 矩阵1,相减后的结果放入矩阵1。
 * @param rows1 矩阵1行数指针。
 * @param cols1 矩阵1列数指针。
 * @param m2 矩阵2,数据源。
 * @param rows2 矩阵2行数。
 * @param cols2 矩阵2列数。
 * @return int 操作结果
 */
int __mat2_sub(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, size_t rows2, size_t cols2)
{
   if (*rows1 == rows2 && *cols1 == cols2) {

        vfloat_t (*m1_ptr)[*cols1] = *m1;
        vfloat_t (*m2_ptr)[cols2]  = m2;

        for (int i=0; i<(*rows1); ++i) {
            for (int j=0; j<(*cols1); ++j) {
                m1_ptr[i][j] -= m2_ptr[i][j];
            }
        }

        return 0;
   } 
   return -1;
}

/**
 * @brief 矩阵乘以标量。
 * 
 * @param m1 矩阵1,乘以标量后结果存入矩阵1。
 * @param rows1 矩阵1行数指针。
 * @param cols1 矩阵1列数指针。
 * @param m2 矩阵2,数据源。
 * @param rows2 矩阵2的行数。
 * @param cols2 矩阵2行数。
 * @param scalar 标量。
 * @return int 操作结果。
 */
int __mat2_scalar_multiply(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, size_t rows2, size_t cols2, vfloat_t scalar)
{
   if (*rows1 == rows2 && *cols1 == cols2) {
        
        vfloat_t (*m1_ptr)[*cols1] = *m1;
        vfloat_t (*m2_ptr)[cols2]  = m2;

        for (int i=0; i<(*rows1); ++i) {
            for (int j=0; j<(*cols1); ++j) {
                m1_ptr[i][j] = m2_ptr[i][j] * scalar;
            }
        }

        return 0;
   } 
   return -1;
}



/**
 * @brief 计算方阵的行列式，使用 Laplace 展开式，递归求行列式。
 * 
 * @param v1 输入的矩阵
 * @param n 矩阵的阶数
 * @return vfloat_t 
 */
vfloat_t __mat2_determinant(vfloat_t* v1, size_t n) 
{
    // 剩下一个数字就直接返回
    if (n == 1) return v1[0];

    vfloat_t det = 0.f;
    vfloat_t (*v1_ptr)[n] = v1;

    float sign = 1.f;
    for (int f=0; f<n; ++f) {
        vfloat_t* v2 = NULL;
        size_t v2_n;

        __mat2_co(&v2, &v2_n, &v2_n, v1, n, n, 0, f);
        // 参考 机器学习数学 84 页 2-29.
        det += sign * v1_ptr[0][f] * __mat2_determinant(v2, v2_n);
        sign = -sign;
        free(v2);
    }
    return det;
}

/**
 * @brief 计算一个矩阵的逆矩阵, 结果放入m1
 * @param m1 输出 m2 逆矩阵，参数为 NULL 或者 malloc 后的有效指针
 * @param rows1 输出矩阵的行数
 * @param cols1 输出矩阵的列数
 * @param m2 输入矩阵
 * @param n 输入矩阵的阶数
 * @return int 
 */
int __mat2_inverse(vfloat_t** m1, size_t* rows1, size_t* cols1,  vfloat_t* m2, size_t n)
{

    // 1 计算出 m2 的行列式
    vfloat_t det = __mat2_determinant(m2, n);

    // 2 计算 m2 的伴随矩阵。
    __mat2_adjoint(m1, rows1, cols1, m2, n);
    
    // 3 将行列式的倒数 X 伴随矩阵
    __mat2_scalar_multiply(m1, rows1, cols1, *m1, *rows1, *cols1, 1 / det);

    return 0;
}


/**
 * @brief 将 m1 去除 p 行 与 q 列剩下的矩阵，放入 m2
 * 
 * @param m1 输出矩阵，参数为 NULL 或者 malloc 后的有效指针
 * @param rows1 输出矩阵行数，参数为有效指针
 * @param cols1 输出矩阵列数，参数为有效指针
 * @param m2 输入矩阵
 * @param rows2 输入矩阵的行数
 * @param cols2 输入矩阵的列数
 * @param p 要剔除的 P 行
 * @param q 要剔除的 q 列
 * @return int 
 */
int __mat2_co(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, size_t rows2, size_t cols2, int p, int q)
{

    *rows1 = rows2 - 1;
    *cols1 = cols2 - 1;
    *m1 = (vfloat_t*) realloc (*m1, (*rows1) * (*cols1) * sizeof(vfloat_t));

    int m1_idx  = 0;
    vfloat_t (*m2_ptr)[cols2]  = m2;

    for (int i=0; i<rows2; ++i) {

        for (int j=0; j<cols2; ++j) {

            if (i != p && j != q) {

                (*m1)[m1_idx++] = m2_ptr[i][j];

            }

        }
    }

    return 0;
}

/**
 * @brief 返回 m2 的伴随矩阵，结果放入 m1 
 * 
 * @param m1 输出 m2 的伴随矩阵，NULL 或者 malloc 后的有效指针。
 * @param m2 输入矩阵
 * @param n 阶数
 * @return int 计算结果
 */
int __mat2_adjoint(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, int n) 
{
    *rows1 = n;
    *cols1 = n;
    *m1 = (vfloat_t*)realloc(*m1, (*rows1) * (*cols1) * sizeof(vfloat_t));
    
    vfloat_t (*m1_ptr)[n] = *m1;
    vfloat_t (*m2_ptr)[n] = m2;
    
    vfloat_t det = 0.f;
    vfloat_t* co_mat = NULL;
    size_t co_n;
    float sign;
    for (int i=0; i<n; ++i) {
        // 在计算代数余子式时, 其符号跟他的行号也有关系，具体为：第一行，开始的 sign 为 1，第二行，开始的 sign 为 -1， 1，-1，1 .....
        sign = (i%2 == 0)? 1.f : -1.f;
        
        for (int j=0; j<n; ++j) {
        
            // 计算去除 i 行与 j 列的矩阵。
            __mat2_co(&co_mat, &co_n, &co_n, m2, n, n, i, j);
            // 然后计算 这个矩阵的行列式。
            det = sign * __mat2_determinant(co_mat, co_n);
            // 将计算结果赋值给 m1
            m1_ptr[i][j] = det;
            sign = -sign;
        }
    }
    free(co_mat);
    return 0;
}

/**
 * @brief 矩阵的奇异值分解。
 * 
 * @param u 
 * @param u_rows 
 * @param u_cols 
 * @param sigma 
 * @param sigma_rows 
 * @param sigma_cols 
 * @param vt 
 * @param vt_rows 
 * @param vt_cols 
 * @param mat 
 * @param mat_rows 
 * @param mat_cols 
 * @return int 
 */
int __mat2_svd(vfloat_t** u, size_t* u_rows, size_t* u_cols, vfloat_t** sigma, size_t* sigma_rows, size_t* sigma_cols, vfloat_t** vt, size_t* vt_rows, size_t* vt_cols, vfloat_t* mat, size_t mat_rows, size_t mat_cols)
{

}


/**
 * @brief 矩阵的 QR 分解，使用的是 House holder 变化进行 QR 分解。
 * 
 * @param q 输出分解后 q 的矩阵，参数为 NULL 或者 malloc 后的有效指针
 * @param q_rows 输出分解后 q 的行数，参数为有效指针
 * @param q_cols 输出分解后 q 的列数，参数为有效指针
 * @param r 输出分解后 r 的矩阵，参数为NULL 或者 malloc 后的有效指针 
 * @param r_rows 输出分解后 r 的行数，参数为有效指针
 * @param r_cols 输出分解后 r 的列数，参数为有效指针
 * @param mat 输入要分解矩阵
 * @param mat_rows 输入分解矩阵的行数
 * @param mat_cols 输入分解矩阵的列数
 * @return int 结果
 */
int __mat2_qr_decomp(vfloat_t** q, size_t* q_rows, size_t* q_cols, vfloat_t** r, size_t* r_rows, size_t* r_cols, vfloat_t* mat, size_t mat_rows, size_t mat_cols, int start, int end)
{
    
    // 取行数列数最小值作为上三角化的步数。

    int max_n = (mat_rows < mat_cols ? mat_rows : mat_cols) - 1;
    int n = end < max_n ? end : max_n;
    // 
    *q_rows = mat_rows;
    *q_cols = mat_rows;
    *q = (vfloat_t*)realloc(*q, (*q_rows)*(*q_cols)*sizeof(vfloat_t));
    vfloat_t (*q_ptr)[*q_cols] = *q;
    
    size_t q_cpy_rows = mat_rows;
    size_t q_cpy_cols = mat_rows;
    vfloat_t* q_cpy = (vfloat_t*) malloc (q_cpy_rows * q_cpy_cols * sizeof(vfloat_t));

    *r_rows = mat_rows;
    *r_cols = mat_cols;
    *r = (vfloat_t*)realloc(*r, (*r_rows) * (*r_cols) * sizeof(vfloat_t));
    vfloat_t (*r_ptr)[mat_cols] = *r;

    // p dot r 时，r 的副本。need to free
    size_t r_cpy_rows = mat_rows;
    size_t r_cpy_cols = mat_cols;
    vfloat_t* r_cpy = (vfloat_t*) malloc (r_cpy_rows * r_cpy_cols * sizeof(vfloat_t));

    size_t p_rows = mat_rows;
    size_t p_cols = mat_rows;
    // need to free
    vfloat_t* p = (vfloat_t*) malloc( p_rows * p_cols *sizeof(vfloat_t));
    vfloat_t (*p_ptr)[p_cols] = p;

    // need to free
    vfloat_t* h = NULL;
    size_t h_rows, h_cols;
    // 用于制作 householder 矩阵的反射向量 v。need to free
    vfloat_t* v = NULL;

    // 将 mat 复制给 r
    memcpy(*r, mat, mat_rows * mat_cols * sizeof(vfloat_t));

    // 讲 q 初始化为 单位矩阵
    memset(*q, 0x0, (*q_rows) * (*q_cols) * sizeof(vfloat_t));
    for (int i=0; i<(*q_rows); ++i) {
        q_ptr[i][i] = 1.f;
    }

    for (int step=start; step<n; step++) {

        // 每次只做 house holder 矩阵前先清空。
        memset(p, 0x0, p_rows * p_cols *sizeof(vfloat_t));
        
        // 
        for (int i=0; i<step; ++i) {
            p_ptr[i][i] = 1.f;
        }

        // 为反射向量申请内存。
        v = (vfloat_t*)realloc(v, ((*r_rows)-step) * sizeof(vfloat_t));

        // 将 r 的列向量放入 v 
        for (int j=0, k=step; k<(*r_rows); ++j, k++) {
            v[j] = r_ptr[k][step];
        }

        // 根据 v 计算 householder 矩阵。
        __mat2_householder_matrix(&h, &h_rows, &h_cols, v, (*r_rows)-step);
        
        // 定义 h 矩阵指针
        vfloat_t (*h_ptr)[h_cols] = h;
    
        // 把 h 复制到 p 中，
        for(int i=step; i<p_rows; ++i) {
            for (int j=step; j<p_cols; ++j) {
                p_ptr[i][j] = h_ptr[i-step][j-step];
            }
        }

        // 建立一个 r 的副本
        memcpy(r_cpy, *r, r_cpy_rows * r_cpy_cols * sizeof(vfloat_t));
        // 把 p dot r 的结构放入 r 中。
        __mat2_dot(r, r_rows, r_cols, p, p_rows, p_cols, r_cpy, r_cpy_rows, r_cpy_cols);

        memcpy(q_cpy, *q, q_cpy_rows * q_cpy_cols * sizeof(vfloat_t));

        __mat2_dot(q, q_rows, q_cols, p, p_rows, p_cols, q_cpy, q_cpy_rows, q_cpy_cols);
    }

    // 以上的结构为 q-1. q 为正交矩阵，把 q 转置便得到 q
    memcpy(q_cpy, *q, q_cpy_rows * q_cpy_cols * sizeof(vfloat_t));
    __mat2_T(q, q_rows, q_cols, q_cpy, q_cpy_rows, q_cpy_cols);

    free(r_cpy);
    free(q_cpy);
    free(p);
    free(h);
    free(v);
    return 0;

}

/**
 * @brief QR 算法，通过不断的交叉点乘 Q dot R 与 R dot Q 得到上三角，或者对角矩阵（当 m 为对称实矩阵时）。通常用于计算矩阵的特征值。
 * 
 * @param a 输出的上三角或者对角矩阵
 * @param q 正交矩阵 q
 * @param m 输入矩阵
 * @param n 输入阶数
 * @return int 
 */
int __mat2_qr_alg(vfloat_t** a, size_t* a_rows, size_t* a_cols, vfloat_t** q, size_t* q_rows, size_t* q_cols, vfloat_t* m, size_t n)
{

    double eps   = 1e-6;
    int max_iter = 100;
    int iter     = 0;
    double diff  = 1.f;
    
    vfloat_t* q1  = NULL;
    size_t q1_rows = 0;
    size_t q1_cols = 0;

    vfloat_t* r1 = NULL;
    size_t r1_rows = 0;
    size_t r1_cols = 0;

    *a = (vfloat_t*) realloc (*a, n*n*sizeof(vfloat_t));
    *a_rows = n;
    *a_cols = n;
    memcpy(*a, m, n*n*sizeof(vfloat_t));

    *q = (vfloat_t*)realloc(*q, n*n*sizeof(vfloat_t));
    *q_rows = n;
    *q_cols = n;

    memset(*q, 0x0, n*n*sizeof(vfloat_t));

    for (int i=0; i<n; ++i) {
        (*q)[i*n+i] = 1.f;
    }

    vfloat_t* q_cpy = (vfloat_t*)malloc(n * n *sizeof(vfloat_t));
    size_t q_cpy_rows = n;
    size_t q_cpy_cols = n;

    vfloat_t* q1_T = (vfloat_t*) malloc (n*n*sizeof(vfloat_t));
    size_t q1_T_rows = n;
    size_t q1_T_cols = n;


    vfloat_t last_diag[n];
    memset(last_diag, 0x0, sizeof(last_diag));

    //TODO: 此处将来要做优化，将 a 优化为上海森堡矩阵，

    while (iter <= max_iter && diff > eps) {

        // Ak-1 = Qk-1 dot Rk-1
        __mat2_qr_decomp(&q1, &q1_rows, &q1_cols, &r1, &r1_rows, &r1_cols, *a, n, n, 0, n);

        memcpy(q_cpy, *q, n*n*sizeof(vfloat_t));

        __mat2_T(&q1_T, &q1_T_rows, &q1_T_cols, q1, q1_rows, q1_cols);
        
        __mat2_dot(q, q_rows, q_cols, q1_T, q1_T_rows, q1_T_cols, q_cpy, q_cpy_rows, q_cpy_cols);

        // Ak = Rk-1 dot Qk-1
        __mat2_dot(a, a_rows, a_cols, r1, r1_rows, r1_cols, q1, q1_rows, q1_cols);
        // MAT2_RAW_INSPECT(*a, *a_rows, *a_cols);

        // 检查对角线是否有变化。
        diff = 0.f;

        for (int i=0; i<n; ++i) {
            // a 的对角线 a[i][i] 的另外一种写法。
            diff += fabs( (*a)[i*n+i] - last_diag[i] );
            // 
            last_diag[i] = (*a)[i*n+i];
            //printf("iter:%d, a[i][i]: %0.3f, pre[i]: %0.3f, diff: %0.10f, diff>esp: %d \n", iter, diff, a[i*n+i], (*eigen_values)[i], (diff > eps));
        }
        
        iter++;
    }

    memcpy(q_cpy, *q, n*n*sizeof(vfloat_t));
    __mat2_T(q, q_rows, q_cols, q_cpy, q_cpy_rows, q_cpy_cols);

    // 将特征值复制给 eigvalue
    free(q1);
    free(r1);
    free(q_cpy);
    free(q1_T);

    return 0;

}

/**
 * @brief 计算一个向量的模长
 * 
 * @param v1 向量
 * @param n 向量长度
 * @return vfloat_t 
 */
vfloat_t __mat2_vect_norm(vfloat_t* v1, size_t n)
{
    vfloat_t norm = 0.f;
    for(int i=0; i<n; ++i) {
        norm += v1[i]*v1[i];
    }
    return sqrt(norm);
}

/**
 * @brief 归一化向量各个分量
 * 
 * @param v1 
 * @param n 
 * @return int 
 */
int __mat2_vect_normalize(vfloat_t* v1, size_t n) 
{
    vfloat_t norm = __mat2_vect_norm(v1, n);
    for (int i=0; i<n; ++i) {
        v1[i] /= norm;
    }
    return 0;
}
/**
 * @brief 通过反射向量 v 计算 H (householder) 矩阵
 * 
 * @param p 输出 P（householder) 矩阵，参数为 NULL 或者 malloc 后的有效指针
 * @param m 输出 P 矩阵的行数，参数为有效指针
 * @param n 输出 P 矩阵的列数，参数为有效指针
 * @return int 
 */
int __mat2_householder_matrix(vfloat_t** p, size_t* p_rows, size_t* p_cols, vfloat_t* v, int n) 
{
    *p = (vfloat_t*) realloc (*p, n*n*sizeof(vfloat_t));
    *p_rows = n;
    *p_cols = n;
    memset(*p, 0x0, n*n*sizeof(vfloat_t));
    vfloat_t (*p_ptr)[n] = *p;
    

    // 把 p 转为单位向量。以及计算 向量 v 的长度。
    for(int i=0; i<n; ++i) {
        p_ptr[i][i] = 1.f;
    }

    vfloat_t x_norm = __mat2_vect_norm(v, n);
    v[0] = v[0] + (v[0] > 0 ? x_norm : -x_norm);

    vfloat_t v_norm = __mat2_vect_norm(v, n);

    float_t* vvT_mat = NULL;
    size_t vvT_rows;
    size_t vvT_cols;
    
    __mat2_dot(&vvT_mat, &vvT_rows, &vvT_cols, v, n, 1, v, 1, n);
    __mat2_scalar_multiply(&vvT_mat, &vvT_rows, &vvT_cols, vvT_mat, n, n, 2.f / (v_norm * v_norm));
    __mat2_sub(p, p_rows, p_cols, vvT_mat, vvT_rows, vvT_cols);

    free(vvT_mat);
    return 0;
}

/**
 * @brief 将矩阵进行 LU 分解，结果存放在 m1 中。
 * 
 * @param m1 
 * @param rows1 
 * @param cols1 
 * @param m2 
 * @param rows2 
 * @param cols2 
 * @return int 
 */
int __mat2_lu_decomp(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, size_t rows2, size_t cols2)
{
    *rows1 = rows2;
    *cols1 = cols2;

    // 如果 m1 与 m2 是不是同一块内存，给 m1 重新分配内存
    if (*m1 != m2) {
        *m1 = (vfloat_t*) realloc (*m1, (*rows1) * (*cols1) * sizeof(vfloat_t));
        memcpy(*m1, m2, (*rows1) * (*cols1) * sizeof(vfloat_t));
    }

    vfloat_t (*m1_ptr)[*cols1] = *m1;

    size_t s = rows2 < cols2 ? rows2 : cols2;

    // 若 m2_ptr[i][i] == 0.f 则分解失败。
    for (int k=0; k<s; ++k) {

        vfloat_t x = 1.f / m1_ptr[k][k];
        
        for (int i=k+1; i<*rows1; ++i) {
            m1_ptr[i][k] = m1_ptr[i][k] * x;
        }

        for (int i=k+1; i<*rows1; ++i) {
            for (int j=k+1; j<*cols1; ++j) {
                m1_ptr[i][j] = m1_ptr[i][j] - m1_ptr[i][k] * m1_ptr[k][j];
            }
        }
    }
    return 0;

}
/**
 * @brief 通过 n 阶矩阵 m1，解线性方程，结果放入 y 中。
 * 
 * @param m1 系数矩阵
 * @param y 输入的Y，解出X
 * @param n 矩阵阶数
 * @return int 返回结果
 */
int __mat2_solve(vfloat_t* m1, vfloat_t* y, size_t n)
{
    vfloat_t* lu = NULL;
    size_t lu_rows;
    size_t lu_cols;

    __mat2_lu_decomp(&lu, &lu_rows, &lu_cols, m1, n, n);

    __mat2_solve_l(lu, y, n);
    __mat2_solve_u(lu, y, n);

    free(lu);

    return 0;
}

/**
 * @brief 
 * 
 * @param ul 
 * @param z 
 * @param n 
 * @return int 
 */
int __mat2_solve_l(vfloat_t* ul, vfloat_t* z, size_t n)
{
    vfloat_t (*ul_ptr)[n] = ul;
    for (int i=1; i<n; ++i) {
        for (int j=0; j<i; ++j) {
            z[i] = z[i] - ul_ptr[i][j] * z[j];
        }
    }
    return 0;
}

/**
 * @brief 
 * 
 * @param ul 
 * @param x 
 * @param n 
 * @return int 
 */
int __mat2_solve_u(vfloat_t* ul, vfloat_t* x, size_t n)
{
    vfloat_t (*ul_ptr)[n];

    for (int k=0; k<n; ++k ) {
        int i = n-k-1;
        for (int j=i+1; j<n; ++j) {
            vfloat_t d1 = ul_ptr[i][j];
            x[i] = x[i] - ul_ptr[i][j] * x[j];
        }
        vfloat_t d2 = ul_ptr[i][i];
        x[i] = x[i] / ul_ptr[i][i];
    }
    return 0;
}

/**
 * @brief 计算矩阵 m1 的特征值, 使用 QR 分解法迭代，没经过优化，非常慢
 * 
 * @param m1 
 * @param n 
 * @param eigen_values 
 * @return int 
 */
int __mat2_eigenvalues(vfloat_t** eigen_values, vfloat_t* m1, size_t n)
{

    vfloat_t* q = NULL;
    size_t q_rows;
    size_t q_cols;

    vfloat_t* a = NULL;
    size_t a_rows;
    size_t a_cols;

    __mat2_qr_alg(&a, &a_rows, &a_cols, &q, &q_rows, &q_cols, m1, n);

    *eigen_values = realloc (*eigen_values, n * sizeof(vfloat_t));
    
    for (int i=0; i<n; ++i) {

        (*eigen_values)[i] = a[i*n+i];

    }
    // q a 没什么用， 把它释放了。
    free(a);
    free(q);

    return 0;

}

/**
 * @brief 通过单个特征值，使用反幂迭代法，计算得到一个单位化的特征向量。
 * 
 * @param eigen_vector 
 * @param m1 
 * @param eigen_values 
 * @param n 
 * @return int 
 */
int __mat2_eigenvector(vfloat_t** eigen_vector, vfloat_t* a,  vfloat_t eigen_value, size_t n)
{

    double eps   = 1e-5;
    int max_iter = 100;
    int iter     = 0;
    double last_diff = 0.f;
    double diff      = 1.f;
    vfloat_t* lu = NULL;
    size_t lu_rows;
    size_t lu_cols;

    
    // 为了不修改修改原来 a 的数据。
    vfloat_t* a_cpy = malloc (n*n*sizeof(vfloat_t));
    size_t a_cpy_rows = n;
    size_t a_cpy_cols = n;
    memcpy(a_cpy, a, n * n  * sizeof(vfloat_t));

    *eigen_vector = (vfloat_t*) realloc (*eigen_vector, n * sizeof(vfloat_t));

    /** 1 使用的是反幂法  */
    vfloat_t eigen_vector2[n];

    for (int i=0; i<n; ++i) {
        (*eigen_vector)[i] = 1.f;
        // 构建以下线性方程 (A - lamada*I) x = eigen_vector
        a_cpy[i*n+i] -= eigen_value;
    }

    __mat2_lu_decomp(&lu, &lu_rows, &lu_cols, a_cpy, n, n);

    memcpy(eigen_vector2, *eigen_vector, n * sizeof(vfloat_t));
    
    __mat2_vect_normalize(eigen_vector2, n);

    while (iter++ <= max_iter && diff > eps) {
        
        __mat2_solve_l(lu, eigen_vector2, n);
        __mat2_solve_u(lu, eigen_vector2, n);
        __mat2_vect_normalize(eigen_vector2, n);

        double curr_diff = 0.f;
        for (int i=0; i<n; ++i) {
            curr_diff += fabs(eigen_vector2[i] - (*eigen_vector)[i]);
            (*eigen_vector)[i] = eigen_vector2[i]; 
        }

        diff = fabs(curr_diff - last_diff);
        last_diff = curr_diff;

        //printf(" diff: %e, e0: %e, e1: %e, e2: %e, e3: %e, e4: %e \n", diff,eigen_vector2[0], eigen_vector2[1],eigen_vector2[2], eigen_vector2[3], eigen_vector2[4]);
        //printf("(%d), diff: %e \n", iter, diff);
    }
    free(lu);

    /** 2 使用幂法只能求取最大特征值 **/
    free(a_cpy);
    return 0;
}

/**
 * @brief 从 m2 复制 cpy number 行数据到 m1
 * 
 * @param m1 
 * @param rows1 
 * @param cols1 
 * @param m2 
 * @param rows2 
 * @param cols2 
 * @param dest_row_idx 
 * @param src_row_idx 
 * @param cpy_number 
 * @return int 
 */
int __mat2_rows_cpy(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, size_t rows2, size_t cols2, int dest_row_idx, int src_row_idx, int src_row_open_end)
{

    int cpy_number = src_row_open_end - src_row_idx;
    int rows_need  = cpy_number - (*rows1 - (dest_row_idx)) ;

    if (rows_need > 0) {
        (*rows1) += rows_need;
        (*m1) = realloc((*m1), (*rows1) * (*cols1) * sizeof(vfloat_t));
    }

    vfloat_t (*m1_ptr) [*cols1] = (*m1);
    vfloat_t (*m2_ptr) [cols2]  = (m2);

    // 行复制，可以整批复制。
    memcpy(&m1_ptr[dest_row_idx][0],  &m2_ptr[src_row_idx][0], cpy_number * (*cols1) * sizeof(vfloat_t));

    return 0;

}

/**
 * @brief 从 m2 copy 某些列到 m1 指定位置，当黏贴数据超过 m1 容量，m1 会扩容，扩容会导致 m1 没被覆盖的数据出现错误。他妈的这个就很难搞。 
 * 
 * @param m1 
 * @param rows1 
 * @param cols1 
 * @param m2 
 * @param rows2 
 * @param cols2 
 * @param dest_col_idx 
 * @param src_row_idx 
 * @param cpy_number 
 * @return int 
 */
int __mat2_cols_cpy(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, size_t rows2, size_t cols2, int dest_col_idx, int src_col_idx, int src_col_open_end)
{

    int cpy_number = src_col_open_end - src_col_idx;
    int cols_need  = cpy_number - ( *cols1 - (dest_col_idx) ) ;

    if (cols_need > 0) {

        (*cols1) += cols_need;
        (*m1) = realloc ((*m1), (*rows1) * (*cols1) * sizeof(vfloat_t));

        // 在填入 m2 的数据前把， 扩容后，弄乱的数据整理好。

        int fix_up_cols = dest_col_idx < (*cols1) ? dest_col_idx : (*cols1);
        if (fix_up_cols > 0) {
            int  old_cols = (*cols1) - cols_need;

            vfloat_t (*ptr1)[*cols1]   = (*m1);
            vfloat_t (*ptr2)[old_cols] = (*m1);

            // 从最后一行开始 copy，copy 到第 2 行的位置，因为第一行不会 被改动。
            for (int i=(*rows1-1); i>=1; --i) {
                memcpy( &ptr1[i][0], &ptr2[i][0], fix_up_cols * sizeof(vfloat_t));
            }
        }

    }
    
    vfloat_t (*m1_ptr)[*cols1] = (*m1);
    vfloat_t (*m2_ptr)[cols2]  = (m2);
    
    // value copy
    int i,j;

    for (i=0; i<(*rows1); ++i) {
        
        if (cpy_number > 1) {
            // 多于一个我们使用整块内存复制。
            memcpy( &m1_ptr[i][dest_col_idx], &m2_ptr[i][src_col_idx], cpy_number * sizeof(vfloat_t));
        } else {
            // 只有一个 cpy number 直接赋值
            m1_ptr[i][dest_col_idx] = m2_ptr[i][src_col_idx];
        }
    }

    return 0;
}

