#ifndef __EM_H__
#define __EM_H__

#include "matrix2.h"

int EM_train(matrix2_t* _X, int K, int Max_iter, double eps, double** alphas, matrix2_t** mus, matrix2_t** sigmas, void (*progress)(const char*, unsigned long, unsigned long));

#endif