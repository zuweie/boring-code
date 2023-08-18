#ifndef __SVM_H__
#define __SVM_H__

#include "matrix2.h"
typedef enum {
    c_scv,
    nu_svc,
    one_class,
    e_svr
}svm_type_t;

int svm_train(matrix2_t* train_data, matrix2_t* train_label, svm_type_t svm_type,  vfloat_t(*kernel_func)(vfloat_t*, int), matrix2_t* alpha);
int svm_predict(matrix2_t* _Input, matrix2_t* alpha, vfloat_t* predict);

#endif