#include "svm_kernel_func.h"

/**
 * @brief 计算 RBF 的核函数,参考公式 3-55
 * 
 * @param v1 X1
 * @param v2 X2
 * @param size_v v1 与 v2 的长度
 * @param _garmmer 参数
 * @param p1 使得接口统一
 * @param p2 使得接口统一
 * @return double 
 */
double kernel_function_rbf(vfloat_t* v1, vfloat_t* v2, int size_v, double _garmmer, double p1, double p1)
{
    double dis_q = 0.f;

    for (int i=0; i<size_v; ++i) 
        dis_q += (v1[i] - v2[i]) * (v1[i] - v2[i]);

    return exp(-_garmmer * dis_q);
}