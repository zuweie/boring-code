#include <math.h>
#include "active.h"

static double _calculate_sigmoid(double x) {
    return 1.f / (1 + exp( -x ));
}

int ann_sigmoid (matrix2_t* vec, double* params)
{
    int length = vec->rows * vec->cols;

    for (int i=0; i<length; ++i) {
        vec->pool[i] = _calculate_sigmoid(vec->pool[i]);
    }
    return 0;
}

int ann_d_sigmoid (matrix2_t* vec, double* params)
{
    int length = vec->rows * vec->cols;

    for (int i=0; i<length; ++i) {
        double sigmoid = _calculate_sigmoid(vec->pool[i]);
        vec->pool[i] = sigmoid * ( 1 - sigmoid );
    }
    return 0;
}


int ann_symmetric_sigmoid (matrix2_t* vec, double* params)
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
int ann_d_symmetric_sigmoid(matrix2_t* vec, double* params)
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