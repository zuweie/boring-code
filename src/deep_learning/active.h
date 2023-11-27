#ifndef __ACTIVE_H__
#define __ACTIVE_H__
#include "matrix2/matrix2.h"

int ann_sigmoid (matrix2_t* vec, double* params);
int ann_d_sigmoid (matrix2_t* vec, double* params);
int ann_symmetric_sigmoid (matrix2_t* vec, double* params);
int ann_d_symmetric_sigmoid(matrix2_t* vec, double* params);

#endif