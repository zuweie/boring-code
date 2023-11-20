#ifndef __ANN_H__
#define __ANN_H__

#include "vtype/vfloat_type.h"
#include "matrix2/matrix2.h"


typedef struct {

    double epsilon;
    int (*active)(matrix2_t*, double* );
    int (*d_active)(matrix2_t* double* );
    double* active_params;
    double* d_active_params;

} ann_param_t;

int ann_train(matrix2_t* data, matrix2_t* label, int* hidden_layers, int hidden_layers_number, ann_param_t* ann_params, matrix2_t** out_Wbs);
int ann_predict(matrix2_t* _Input, matrix2_t* Wbs, int hidden_layers_number, vfloat_t* predict);
int ann_recycle(matrix2_t* Wbs);


#endif