/*
 * @Author: your name
 * @Date: 2021-01-14 08:46:10
 * @LastEditTime: 2021-01-15 09:00:23
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/Cosine_Transform/cosine_transform.c
 */
#include <math.h>
#include "cosine_transform.h"

double K_cosine_transform(int k, size_t N, double x[])
{
    double A, a, sum = 0.0f;

    if (k == 0) {
        A = sqrt(1.0 / N);
    }else{
        A = sqrt(2.0 / N);
    }

    for (int n=0; n<N; ++n) {
        a = x[n] * cos(3.1415926535898*(n+0.5f)*k/N);
        sum += a;
    }
    
    return A*sum;
}

int Discrete_cosine_transform(double sequence[], size_t N, double out[]) 
{
    for (k=0; k<N; ++k) {
        out[k] = K_cosine_transform(k, N, sequence);
    }
    return 0;
}