#include <stdlib.h>
#include <math.h>
#include <string.h>
#include <stdio.h>
#include "matrix2_operator.h"
/**
 * @brief 两个矩阵点积,也叫内积,结果保存在 m1 指向的内存中。
 * 
 * @param m1 矩阵1数据内存地址的指针,为什么是数据地址的指针,是因为这个矩阵的内存有可能会被重新申请。
 * @param rows1 矩阵1行数的指针,此值有可能被修改。
 * @param cols1 矩阵1列数的指针,此值有可能被修改。
 * @param m2 矩阵2数据内存地址,逻辑上这块内存是原数据不会被修改,但如果 *m1 的地址与 m2 的地址相同,也会修改其值。
 * @param rows2 矩阵2的行数。
 * @param cols2 矩阵2的列数。
 * @param m3 
 * @param rows3
 * @param cols3
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
 * @brief 一个矩阵装置
 * 
 * @param m1 矩阵1的内存地址指针,接受 m2 装置后的结果。
 * @param rows1 矩阵1的行数指针
 * @param cols1 矩阵2的列数指针
 * @param m2 矩阵2的内存指针,逻辑上这块内存是原数据不会被修改,但 *m1 与 m2 相同也会被修改
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
 * @param v1 
 * @param n
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
 * 
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
 * @param m1 
 * @param row1 
 * @param cols1 
 * @param m2 
 * @param rows2 
 * @param cols2 
 * @param p 
 * @param q 
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
 * @param m1 m1 大小不会改变，所有不会改变内存，请确保 m1 的内存足够大。
 * @param m2 输入矩阵
 * @param n 阶数
 * @return int 
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
 * @param q 
 * @param q_rows 
 * @param q_cols 
 * @param r 
 * @param r_rows 
 * @param r_cols 
 * @param mat 
 * @param mat_rows 
 * @param mat_cols 
 * @return int 
 */
int __mat2_qr(vfloat_t** q, size_t* q_rows, size_t* q_cols, vfloat_t** r, size_t* r_rows, size_t* r_cols, vfloat_t* mat, size_t mat_rows, size_t* mat_cols)
{
    
    

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
 * @brief 通过反射向量 v 计算 H (householder) 矩阵
 * 
 * @param p 
 * @param m 
 * @param n 
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

    MAT2_RAW_INSPECT(vvT_mat, vvT_rows, vvT_cols);

    __mat2_scalar_multiply(&vvT_mat, &vvT_rows, &vvT_cols, vvT_mat, n, n, 2.f / (v_norm * v_norm));

    MAT2_RAW_INSPECT(vvT_mat, vvT_rows, vvT_cols);

    __mat2_sub(p, p_rows, p_cols, vvT_mat, vvT_rows, vvT_cols);

    MAT2_RAW_INSPECT((*p), *p_rows, *p_cols);
    free(vvT_mat);
    return 0;
}