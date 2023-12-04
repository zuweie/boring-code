#ifndef __RNN_H__
#define __RNN_H__
#include "matrix2/matrix2.h"
#include "vtype/vfloat_type.h"

typedef struct {

} rnn_param_t;

int rnn_train(matrix2_t* data, matrix2_t* label, rnn_param_t* rnn_params, matrix2_t** out_W, matrix2_t** out_U);
int rnn_predict(matrix2_t* _Input, matrix2_t* U, matrix2_t* W);

#endif