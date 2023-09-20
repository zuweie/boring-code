#include "em.h"

/**
 * @brief 
 * 
 * @param _X 
 * @param K 
 * @param mu 
 * @param sigema 
 * @param gamma 
 * @return int 
 */
static int __calculate_gauss(matrix2_t* _X, int K, matrix2_t** mus, matrix2_t** sigmas, matrix2_t** gammas)
{

    for (int i=0; i<K; ++i) {

        matrix2_t* mu = mus[i];
        matrix2_t* sigma = sigmas[i];
        

        for(int )

    }

}

//  计算 期望 步。

int E_step(int K, double* alphas, matrix2_t* mus, matrix2_t* sigemas)
{

}

// 计算 M 步
int M_step()
{

}

// EM 的训练。

/**
 * @brief 通过 EM 算法， 计算 K 个 alpha，K 个 mu，K 个 sigema
 * 
 * @param _X D 维向量输入数据。
 * @param K 高斯模型的个数，人工输入。
 * @param eps EM 训练的终止条件。
 * @param alphas 输出的 alpha，有 K 个。
 * @param mus 输出的 mu 向量，有 K 个，每个 D 维
 * @param sigemas 
 * @return int 
 */
int EM_train(matrix2_t* _X, int K, double eps, double* alphas, matrix2_t** mus, matrix2_t** sigemas)
{
    // TODO : 非常简单，就是不断的进行 E 步 与 M 的轮流计算，获取高斯分布的那几个关键的参数。nu sigma，与 alpha。
}