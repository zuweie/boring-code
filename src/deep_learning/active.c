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
        double sigmo = _calculate_sigmoid(vec->pool[i]);
        vec->pool[i] = sigmo * ( 1 - sigmo );
    }
    return 0;
}