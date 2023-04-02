#include "matrix2.h"

int __mat2_dot(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, size_t rows2, size_t cols2)
{
    return 0;
}

vfloat_t __mat2_vect_dot(vfloat_t* v1, vfloat_t* v2, size_t n) 
{
    vfloat_t result = 0.f;
    for (int i=0; i<n; ++i) {
        result += v1[i] * v2[i];
    }
    return result;
}


int __mat2_T(vfloat_t** m1, size_t* rows1, size_t* cols1, vfloat_t* m2, size_t rows2, size_t cols2)
{
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
 * @return int 
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