/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-11-24 12:03:36
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-12-08 16:11:04
 * @FilePath: /boring-code/src/deep_learning/active.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <math.h>
#include "active.h"

static double _calculate_sigmoid(double x) {
    return 1.f / (1 + exp( -x ));
}

int sigmoid (matrix2_t* vec, double* params)
{
    int length = vec->rows * vec->cols;

    for (int i=0; i<length; ++i) {
        vec->pool[i] = _calculate_sigmoid(vec->pool[i]);
    }
    return 0;
}

int d_sigmoid (matrix2_t* vec, double* params)
{
    int length = vec->rows * vec->cols;

    for (int i=0; i<length; ++i) {
        double sigmoid = _calculate_sigmoid(vec->pool[i]);
        vec->pool[i] = sigmoid * ( 1 - sigmoid );
    }
    return 0;
}


int symmetric_sigmoid (matrix2_t* vec, double* params)
{
    double alpha = params[0];
    double beta  = params[1];

    int length = vec->rows * vec->cols;
    double x = 0.f;
    for (int i=0; i<length; ++i) {
        x = vec->pool[i];
        vec->pool[i] = beta * ( (1-exp(-alpha * x)) / (1 + exp(-alpha * x)) );
    }
    return 0;
}
int d_symmetric_sigmoid(matrix2_t* vec, double* params)
{
    double alpha = params[0];
    double beta  = params[1];

    int length = vec->rows * vec->cols;
    double x = 0.f;
    for (int i=0; i<length; ++i) {
        x = vec->pool[i];
        vec->pool[i] = 2 * alpha * beta * (  exp(-alpha * x) / ( (1 + exp(-alpha * x)) * ( 1 + exp(-alpha * x )) ) );
    }
    return 0;
}

int tanh (matrix2_t* vec, double* params)
{
    
}

int d_tanh(matrix2_t* vec, double* params)
{

}

int softmax(matrix2_t* vec, double* params)
{

}

int d_softmax(matrix2_t* vec, double* params) 
{
    
}