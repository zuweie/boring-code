#ifndef __PERCEPTRON_H__
#define __PERCEPTRON_H__

#include "matrix/matrix.h"


int pct_train(matrix_t* _Data, matrix_t** _W, vfloat_t* b, float lr, int max_iter);
int pct_perdict(matrix_t* _X, matrix_t* _W, vfloat_t* b);

#endif