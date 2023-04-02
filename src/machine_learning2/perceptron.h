#ifndef __PERCEPTRON_H__
#define __PERCEPTRON_H__

#include "matrix2.h"


int pct_train(matrix2_t* _Data, matrix2_t** _W_b, float lr, int max_iter);
int pct_perdict(matrix2_t* _X, matrix2_t* _W, vfloat_t* b);

#endif