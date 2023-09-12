/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-06-15 16:10:10
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-09-12 10:47:46
 * @FilePath: /boring-code/src/statistical_learning/svm.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include "svm.h"

/**
 * @brief 根据 SVM 类型来初始化，几个重要的矩阵或者向量
 * 
 */
static int svm_initialize( matrix2_t* Bate, matrix2_t* Q, matrix2_t* P,   \
                        matrix2_t* _X, matrix2_t* _Y, svm_type_t svm_type, \
                       vfloat_t (*kernel_func)(vfloat_t*, int))
{

    if (svm_type == c_svc) {
        
        

    } 

    // else if ( ... ) { 
    // 还有其他四种算法，先弄一种
    // }
    return 0;

}

/**
 * @brief SVM 训练过程，使用广义 SMO 计算支持向量。
 * 
 * @param train_data 
 * @param train_label 
 * @param svm_type 
 * @param kernel_func 
 * @param alpha 
 * @return int 
 */
int svm_train(matrix2_t* train_data, matrix2_t* train_label, svm_type_t svm_type,  vfloat_t (*kernel_func)(vfloat_t*, int), matrix2_t* alpha)
{
    // 通用的计算公式：f(Beta) = 1/2 BetaT dot Q bate + pT dot Beta (1.1)
    // Deta f(Beta) = Q dot Beta + P (1.2)
    
    // 在 Q 为对称矩阵的时候，以上的 微分公式才能成立。

    // TODO: 1 根据 SVM 类型，初始化 Beta、Q、P 的矩阵或者向量。
    
    // TODO: 2 根据 SVM 类型，选出要优化的两个 Beta，

    // TODO: 3 根据 SVM 类型，算出 Beta 的差分值，然后更新公式 (1.2)

    
    matrix2_t* BeatT = Mat2_create(1,1);
    matrix2_t* Beta  = Mat2_create(1,1);
    matrix2_t* Q     = Mat2_create(1,1);
    matrix2_t* P     = Mat2_create(1,1);

    
    svm_initialize

    // 释放内存
    Mat2_destroy(BeatT);
    Mat2_destroy(Beta);
    Mat2_destroy(Q);
    Mat2_destroy(P);
    return 0;
}

int svm_predict(matrix2_t* _Input, matrix2_t* alpha, vfloat_t* predict);