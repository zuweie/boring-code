/*
 * @Author: your name
 * @Date: 2021-01-14 08:46:10
 * @LastEditTime: 2021-01-29 12:29:04
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/Cosine_Transform/cosine_transform.c
 */
#include <math.h>
#include "cosine_transform.h"

// double K_cosine_transform(int k, size_t N, double x[])
// {
//     double A, a, sum = 0.0f;

//     if (k == 0) {
//         A = sqrt(1.0 / N);
//     }else{
//         A = sqrt(2.0 / N);
//     }

//     for (int i=0; i<N; ++i) {
//         a = x[i] * cos( 3.1415926535898*(i-0.5)*k/N );
//         sum += a;
//     }
    
//     return A*sum;
// }

double K_cosine_transform_i(int k, size_t N, double sequence[], dct_norm_t norm) 
{
    double sum = 0.f;
    for (int n=1; n<N-1; ++n) {
        sum += sequence[n] * cos( 3.1415926535898 * k * n / (N-1) );
    }
    return sequence[0] + pow(-1, k) * sequence[N-1] + 2 * sum;
}

double K_cosine_transform_ii(int k, size_t N, double sequence[], dct_norm_t norm) 
{
    double sum = 0.f;
    for (int n=0; n<N; ++n) {
        sum += sequence[n] * cos( 3.1415926535898 * k * (2*n+1) / (2 * N) );
    }

    if (norm == dct_ortho) {
        double f = k ==0 ? (sqrt( 1.f / (4 * N) )) : (sqrt( 1.f / (2 * N) )) ;
        sum *= f;
    }
    return sum;
}   

double k_cosine_transform_iii(int k, size_t N, double sequence[], dct_norm_t norm) 
{
    double sum = 0.f;
    for (int n=1; n<N; ++n) {
        sum += sequence[n] * cos( 3.1415926535898 * (k+0.5) * n / N );
    }
    if (norm == dct_none) {
        sum = sequence[0] + 2 * sum;
    }else if (norm == dct_ortho) {
        sum = sequence[0] / sqrt(N) + sqrt(2.f/N) * sum;
    }
    return sum;
}

int Discrete_cosine_transform(double sequence[], size_t N, int cal_n, double out[], dct_norm_t dct_norm, dct_type_t dct_type) 
{
    for (int k=0; k<cal_n; ++k) {
        if (dct_type == dct_i) {
            out[k] = K_cosine_tranform_i(k, N, sequence, dct_norm);
        }else if (dct_type == dct_ii) {
            out[k] = K_cosine_transform_ii(k, N, sequence, dct_norm);
        }else if (dct_type == dct_iii ) {
            out[k] = k_cosine_transform_iii(k, N, sequence, dct_norm);
        }
    }
    return 0;
}