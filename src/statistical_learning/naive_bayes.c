/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-06-16 14:50:03
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-06-21 14:36:51
 * @FilePath: /boring-code/src/statistical_learning/naive_bayes.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include "naive_bayes.h"


/**
 * @brief 
 * 
 * @param train_mat 
 * @param train_label_mat 
 * @param Py_probability
 * @param Pxy_probability 
 * @return int 结果
 */
int navie_bayes_train(matrix2_t* train_mat, matrix2_t* train_label_mat, void** Py_counting, void** Px_y_counting_table)
{
    // 获取 label 的统计。
    __mat2_count_element(train_label_mat->pool, train_label_mat->rows, Py_counting);

    int size_eleme = *MAT2_COUNTING_SIZE_PTR(*Py_counting);
    int* elem_ptr  = MAT2_COUNTING_LIST_PTR(*Py_counting);
    MAT2_POOL_PTR(train_label_mat, label_mat_ptr);
    MAT2_POOL_PTR(train_mat, train_mat_ptr);

    // 
    size_t table_size = size_eleme * train_mat->cols * sizeof(char*);
    char* (*table)[train_mat->cols] = malloc (table_size);

    memset(table, 0x0, table_size);
    // 
    
    // 建立统计表。
    vfloat_t elem_value[train_mat->rows];
    int elem_size;

    for (int i=0; i<size_eleme; ++i) {
        int label = (int)elem_ptr[i];

        for (int j=0; j<train_mat->cols; ++j) {

            elem_size = 0;

            for (int k=0; k<train_mat->rows; ++k) {
                //
                int Y_label = label_mat_ptr[k][0];

                if (label == Y_label) {
                    elem_value[elem_size++] = train_mat_ptr[k][j];
                }
            } 
            void* out;
            __mat2_count_element(elem_value, elem_size, &out);
            table[i][j] = out;
        }
    }

    *Px_y_counting_table = table;
    return 0;
}

/**
 * @brief 通过统计表，做概率运算。
 * 
 * @param _X 
 * @param Py_probability 
 * @param Pxy_probability 
 * @return int 
 */
int navie_bayes_predict(matrix2_t* _X, void* Py_counting, void* Pxy_counting_table, int lambda, vfloat_t* predict)
{
    int       Py_counting_size        = *MAT2_COUNTING_SIZE_PTR(Py_counting);
    int*      Py_counting_numbers_ptr = MAT2_COUNTING_NUMBERS_PTR(Py_counting);
    vfloat_t* Py_element_ptr          = MAT2_COUNTING_LIST_PTR(Py_counting);

    int y_total = 0;

    for (int i=0; i<Py_counting_size; ++i) {
        y_total += Py_counting_numbers_ptr[i];
    }

    int probability_size = *MAT2_COUNTING_SIZE_PTR(Py_counting);

    float probability_logs[probability_size];
    char* (*Pxy_counting_table_ptr)[_X->cols] = Pxy_counting_table;
    
    for (int i=0; i<probability_size; ++i) {
        float pro_log = 0.f;
        vfloat_t y_label = Py_element_ptr[i];

        // 计算 _X 在每个 lable 中可能的概率
        int y_number = __mat2_get_element_number(Py_counting, y_label);

        pro_log += log( (float) y_number / y_total);

        for (int j=0; j<_X->cols; ++j) {
            vfloat_t x_value = _X->pool[j];
            void* counting = Pxy_counting_table_ptr[i][j];
            int si             = MAT2_COUNTING_SIZE_PTR(counting);
            int x_value_number = __mat2_get_element_number(counting, x_value);

            // 需要做拉普拉斯平滑
            float px_y = (float)(x_value_number + lambda) / (float)(y_number + si * lambda);
            pro_log += log(px_y);
        }

        probability_logs[i] = pro_log;
    }

    // 最后返回概率最大的那个类别。

    float max_prob = FLT_MIN;
    int   max_indx = 0;
    for (int k=0; k<probability_size; ++k) {
        if (max_prob < probability_logs[k]){
            max_prob = probability_logs[k];
            max_indx = k;
        }
    }

    *predict = Py_element_ptr[max_indx];

    return 0;
}

int navie_bayes_release_counting(void* Py_counting, void* Pxy_count_table, int table_cols)
{
    
    int py_element_size = MAT2_COUNTING_SIZE_PTR(Py_counting);
    char* (*Pxy_count_table_ptr)[table_cols] = Pxy_count_table;

    for (int i=0; i<py_element_size; ++i) {
        for (int j=0; j<table_cols; ++j) {
            free(Pxy_count_table_ptr[i][j]);
        }
    }
    free(Pxy_count_table);
    free(Py_counting);
    return 0;
}