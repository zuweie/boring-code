#ifndef __ADABOOST_H__
#define __ADABOOST_H__
#include "vtype/vfloat_type.h"
#include "matrix2/matrix2.h"

typedef struct {
    int split_A;
    vfloat_t best_split;
    vfloat_t predict;
} adaboost_gx_t;

int adaboost_tree_train(matrix2_t* train_data, matrix2_t* train_label, int M, double** alpahs_out,  adaboost_gx_t** Gx_out, void (*progress)(const char*, unsigned long, unsigned long));
int adaboost_tree_predict(matrix2_t* _Input, int M, adaboost_gx_t* Gxs, double* alphas, vfloat_t* predict);

#endif