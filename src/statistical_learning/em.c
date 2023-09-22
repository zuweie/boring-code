/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-08-15 14:48:47
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-09-22 13:12:13
 * @FilePath: /boring-code/src/statistical_learning/em.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "em.h"

static int __init_mu_data(matrix2_t* mu) 
{
    Mat2_fill(mu, 0.f);
    srand((unsigned) time (NULL));

    for (int i=0; i<mu->rows * mu->cols; ++i) {
        mu->pool[i] = (double) rand() / RAND_MAX;
    }
    return 0;
}

/**
 * @brief 
 * 
 * @param sigma_data 
 * @param size 
 * @return int 
 */
static int __init_sigma_data(matrix2_t* sigma) 
{
    //memset(sigma_data, 0, size*size*sizeof(vfloat_t));

    Mat2_fill(sigma, 0.f);

    MAT2_POOL_PTR(sigma, sigma_ptr);

    srand((unsigned) time (NULL));

    for (int i=0; i<sigma->rows; ++i) {
        sigma_ptr[i][i] = (double) rand() / RAND_MAX;
    }
    return 0;
}
/**
 * @brief 书本中 9-82 式, 计算第 K 模型下第 i 条数据的后验证概率。
 * 
 * @param _X 
 * @param K 
 * @param mu 
 * @param sigema 
 * @param alphaK 
 * @return int 
 */
static double __calculate_apK(matrix2_t* _Xi, matrix2_t* sigma, double alpha)
{
    double det_sigma = 1.f;
    double pK        = 0.f;

    matrix2_t* w_mat         = Mat2_create(1,1);
    matrix2_t* u_mat         = Mat2_create(1,1);
    matrix2_t* _Xi_T         = Mat2_create(1,1);
    matrix2_t* inverse_sigma = Mat2_create_cpy(sigma);
    
    Mat2_inverse(inverse_sigma);

    Mat2_eig(w_mat, u_mat, inverse_sigma);

    Mat2_dot(_Xi, u_mat);

    Mat2_cpy(_Xi_T, _Xi);

    Mat2_T(_Xi_T);

    Mat2_hadamard_product(_Xi, w_mat);
    
    Mat2_dot(_Xi, _Xi_T);

    for (int k=0; k<w_mat->cols; ++k) {
        det_sigma *= 1.f / w_mat->pool[k];
    }

    pK += log(alpha);
    pK -= 0.5 * log(det_sigma);
    pK -= _Xi->pool[0];

    Mat2_destroy(w_mat);
    Mat2_destroy(u_mat);
    Mat2_destroy(_Xi_T);
    Mat2_destroy(inverse_sigma);

    return exp(pK);
}

//  计算 期望 步。
/**
 * @brief 计算各个模型下，p(y|X) 的先验概率。
 * 
 * @param _X 
 * @param K 
 * @param alphas 
 * @param sigmas 
 * @param apK 
 * @return int 
 */
static int __e_step(matrix2_t* _X, int K, double* alphas, matrix2_t** sigmas, matrix2_t** apKs)
{
    matrix2_t* _Xi = Mat2_create(1,1);
    double alpha;
    matrix2_t* sigma;
    matrix2_t* apK;

    for (int i=0; i<_X->rows; ++i) {

        Mat2_slice_row_to(_Xi, _X, i);
        double p = 0.f;

        for (int j=0; j<K; ++j) {
            
            alpha  = alphas[j];
            sigma  = sigmas[j];
            apK    = apKs[j];

            apK->pool[i] = __calculate_apK(_Xi, sigma, alpha);
            p += apK->pool[i];
        }

        // 归一化。
        for (int l=0; l<K; ++l) {
            apK = apKs[l];
            apK->pool[i] /= p;
        }
    }
    Mat2_destroy(_Xi);


    return 0;
}

// 计算 M 步 
/**
 * @brief 计算 M 步，即更新 alpha、mu、sigma
 * 
 * @param _X 
 * @param K 
 * @param alphas 
 * @param mus 
 * @param sigmas 
 * @param apK 
 * @return int 
 */
static int __m_step(matrix2_t* _X, int K, double* alphas, matrix2_t** mus, matrix2_t** sigmas, matrix2_t** apKs)
{
    matrix2_t* apK_cpy = Mat2_create(1,1);
    matrix2_t* _Xi     = Mat2_create(1,1);
    matrix2_t* _Xi_T   = Mat2_create(1,1);
    double sum_apk     = 0.f;
    for (int i=0; i<K; ++i) {

        matrix2_t* apK   = apKs[i];
        matrix2_t* mu    = mus[i];
        matrix2_t* sigma = sigmas[i];
        
        Mat2_cpy(apK_cpy, apKs[i]);
        //int n = apK_cpy->rows;
        Mat2_sum(apK_cpy, 0);
        sum_apk = apK_cpy->pool[0];



        // TODO: 1 各个模型的 alpah 
        alphas[i] =  sum_apk / apK->rows;
        // end 1


        // TODO： 2 各个模型中的 mu 
        Mat2_cpy(mu, _X);

        MAT2_POOL_PTR(mu, mu_ptr);

        for (int m=0; m<mu->rows; ++m) {
            for (int l=0; l<mu->rows; ++l) {
                mu_ptr[m][l] *= apK->pool[m];
            }
        }
        
        Mat2_sum(mu, 0);
        
        Mat2_scalar_multiply(mu,  1 / sum_apk);

        Mat2_T(mu);

        // end 2


        // TODO： 3 各个模型中 sigma 计算 sigma
        Mat2_fill(sigma, 0.f);

        for (int r=0; r<_X->rows; ++r) {

            Mat2_slice_row_to(_Xi, _X, r);

            Mat2_T(_Xi);

            Mat2_sub(_Xi, mu);

            Mat2_cpy(_Xi_T, _Xi);
            
            Mat2_T(_Xi_T);

            Mat2_dot(_Xi, _Xi_T);

            Mat2_scalar_multiply(_Xi, apK->pool[r]);
            
            Mat2_add(sigma, _Xi);
        }

        Mat2_scalar_multiply(sigma, 1 / sum_apk);
        // end 3
    
    }

    Mat2_destroy(apK_cpy); 
    Mat2_destroy(_Xi);    
    Mat2_destroy(_Xi_T); 

    return 0;
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
int EM_train(matrix2_t* _X, int K, int Max_iter, double eps, double** alphas, matrix2_t** mus, matrix2_t** sigmas, void (*progress)(const char*, unsigned long, unsigned long))
{
    // TODO : 非常简单，就是不断的进行 E 步 与 M 的轮流计算，获取高斯分布的那几个关键的参数。nu sigma，与 alpha。
    matrix2_t** __apKs     = (matrix2_t**)malloc (K * sizeof(matrix2_t*));
    matrix2_t** __mus      = (matrix2_t**)malloc (K * sizeof(matrix2_t*));
    matrix2_t** __sigmas   = (matrix2_t**) malloc (K * sizeof(matrix2_t*));
    double*    __alphas    = (double*) malloc (K * sizeof(double));


    for (int i=0; i<K; ++i) {
        __mus[i]    = Mat2_create(_X->cols, 1);
        //Mat2_fill(__mus[i], 0.8);

        __init_mu_data(__mus[i]);

        //MAT2_INSPECT(__mus[i]);

        __sigmas[i] = Mat2_create(_X->cols, _X->cols);
        __init_sigma_data(__sigmas[i]);

        //MAT2_INSPECT(__sigmas[i]);

        __apKs[i]   = Mat2_create(_X->rows, 1);
        __alphas[i] = 1.f / (double) K;      
    }

    int iter = 0;
    while ( ++iter <= Max_iter)
    {
        if (progress)
            progress("E step ... ", iter, Max_iter);

        __e_step(_X, K, __alphas, __sigmas, __apKs);

        if (progress)
            progress("M step ... ", iter, Max_iter);

        __m_step(_X, K, __alphas, __mus, __sigmas, __apKs);
    }
    // 输出结果。
    *alphas = __alphas;
    *mus    = __mus;
    *sigmas = __sigmas;
    return 0;
}

