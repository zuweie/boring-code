/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-08-15 14:48:47
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-09-25 11:22:46
 * @FilePath: /boring-code/src/statistical_learning/em.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <math.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <float.h>
#include "em.h"

static int __init_apK_data(matrix2_t* apK)
{
    //srand((unsigned) time (NULL));

    for (int i=0; i<apK->rows * apK->cols; ++i) {
        apK->pool[i] = (double)rand() / RAND_MAX;
    }
    return 0;
}

static int __init_mu_data(matrix2_t* mu) 
{
    //Mat2_fill(mu, 0.f);
    srand((unsigned) time (NULL));

    for (int i=0; i<mu->rows * mu->cols; ++i) {
        mu->pool[i] = 0.5;//(double) rand() / RAND_MAX;
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
        sigma_ptr[i][i] = 0.99f;//(double) rand() / RAND_MAX;
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
    pK -= 0.5 * _Xi->pool[0];

    Mat2_destroy(w_mat);
    Mat2_destroy(u_mat);
    Mat2_destroy(_Xi_T);
    Mat2_destroy(inverse_sigma);
    
    double ret = exp(pK);
    //printf("ret: %lf \n", ret);
    return ret;
}

/**
 * @brief 使用原始的算法试试。
 * 
 * @param _Xi 
 * @param sigma 
 * @param alpha 
 */
static double __calculate_apK_2(matrix2_t* _Xi, matrix2_t* mu,  matrix2_t* sigma, double alpha) 
{
    // 此处的 _Xi mu, sigma 对象不能被改变，因为在外面还要用到 
    matrix2_t* _Xi_cpy = Mat2_create_cpy(_Xi); 
    matrix2_t* _Xi_T   = Mat2_create(1,1);
    matrix2_t* mu_cpy  = Mat2_create_cpy(mu);
    matrix2_t* sigma_cpy = Mat2_create_cpy(sigma);

    int d = _Xi_cpy->cols;

    vfloat_t det_sigma;
    Mat2_det(sigma_cpy, &det_sigma);
    Mat2_inverse(sigma_cpy);

    Mat2_T(mu_cpy);
    Mat2_sub(_Xi_cpy, mu_cpy);
    
    Mat2_cpy(_Xi_T, _Xi_cpy);
    Mat2_T(_Xi_T);

    Mat2_dot(_Xi_cpy, sigma_cpy);


    Mat2_dot(_Xi_cpy, _Xi_T);

    double pk = 0.f;
    pk += log(alpha);
    //pk -= 0.5f * d * log(2*3.1415926);
    pk -= 0.5f * log(det_sigma);
    pk -= 0.5f * _Xi_cpy->pool[0];

    double ret = exp(pk);
    //printf("<alpha: %lf, det_sigm: %lf, xi[0]: %lf, pk: %lf, ret: %lf> \n", alpha, det_sigma, _Xi->pool[0], pk, ret);
    //printf("ret: %lf ", ret);

    Mat2_destroy(_Xi_T);
    Mat2_destroy(mu_cpy);
    Mat2_destroy(sigma_cpy);
    Mat2_destroy(_Xi_cpy);


    return ret;
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
static int __e_step(matrix2_t* _X, int K, double* alphas, matrix2_t** mus, matrix2_t** sigmas, matrix2_t** apKs)
{
    matrix2_t* _Xi = Mat2_create(1,1);
    double alpha;
    matrix2_t* sigma;
    matrix2_t* apK;
    matrix2_t* mu;

    for (int i=0; i<_X->rows; ++i) {

        Mat2_slice_row_to(_Xi, _X, i);
        double p = 0.f;

        for (int j=0; j<K; ++j) {
            
            alpha  = alphas[j];
            mu     = mus[j];
            sigma  = sigmas[j];
            apK    = apKs[j];

            //apK->pool[i] = __calculate_apK(_Xi, sigma, alpha);
            apK->pool[i] = __calculate_apK_2(_Xi, mu, sigma, alpha);
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
            for (int l=0; l<mu->cols; ++l) {
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
int EM_train(matrix2_t* _X, int K, int Max_iter, double epsilon, double** alphas, matrix2_t** mus, matrix2_t** sigmas, void (*progress)(const char*, double, unsigned long, unsigned long))
{
    // TODO : 非常简单，就是不断的进行 E 步 与 M 的轮流计算，获取高斯分布的那几个关键的参数。nu sigma，与 alpha。
    matrix2_t** __apKs     = (matrix2_t**)malloc (K * sizeof(matrix2_t*));
    matrix2_t** __mus      = (matrix2_t**)malloc (K * sizeof(matrix2_t*));
    matrix2_t** __sigmas   = (matrix2_t**) malloc (K * sizeof(matrix2_t*));
   

    double*    __alphas    = (double*) malloc (K * sizeof(double));

    double last_apks_norm[K];

    for (int i=0; i<K; ++i) {
        __mus[i]    = Mat2_create(_X->cols, 1);
        //__init_mu_data(__mus[i]);

        __sigmas[i] = Mat2_create(_X->cols, _X->cols);
        //__init_sigma_data(__sigmas[i]);

        //MAT2_INSPECT(__sigmas[i]);

        __apKs[i]   = Mat2_create(_X->rows, 1);
        __init_apK_data(__apKs[i]);

        __alphas[i] = 1.f / (double) K;     

        last_apks_norm[i] = FLT_MAX; 
    }

    int iter = 0;
    double sum_diff = FLT_MAX;
    double curr_norm = 0.f;
    double last_sum = 0.f;

    // 用于记录每次迭代后的 apk 的模长度，当长度不在变化，那么表明 apk 不再有变化于是收敛了。


    while ( ++iter <= Max_iter && sum_diff  > epsilon) {

        if (progress)
            progress("M step ... ", 0.f, iter, Max_iter);

        __m_step(_X, K, __alphas, __mus, __sigmas, __apKs);

        if (progress)
            progress("E step ... ", 0.f, iter, Max_iter);

        __e_step(_X, K, __alphas, __mus, __sigmas, __apKs);

        // __apks 被更新后，即可计算它的模长度，看看跟上一轮有没有变化，有变化即可继续优化。
        sum_diff = 0.f;

        for (int i=0; i<K; ++i) {
            
            curr_norm = Mat2_norm(__apKs[i]);
            sum_diff += fabs( curr_norm - last_apks_norm[i] );
            last_apks_norm[i] = curr_norm;

        }
        if (progress)
            progress(" epsilon ... ", sum_diff, iter, Max_iter);
    }
    // 输出结果。
    *alphas = __alphas;
    *mus    = __mus;
    *sigmas = __sigmas;
    return 0;
}

int EM_recycle(int K, double** alphas, matrix2_t** mus, matrix2_t** sigmas)
{
    for (int i=0; i<K; ++i) {
        matrix2_t* m = mus[i];
        Mat2_destroy(m);

        matrix2_t* s = sigmas[i];
        Mat2_destroy(s);
    }

    free(alphas);
    free(mus);
    free(sigmas);
    return 0;
}

