/*
 * @Author: your name
 * @Date: 2021-06-22 14:54:10
 * @LastEditTime: 2021-07-09 15:56:05
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/machine_learning/svm/svm_model.c
 */
#include "svm_model.h"

int SVN_model_finalize(svm_model_t* model)
{
    UArray_(model->_star_alpha);
    UArray_(model->_star_X);
    UArray_(model->_star_Y);

}