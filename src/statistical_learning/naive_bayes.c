#include <stdlib.h>
#include <stdio.h>
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
int navie_bayes_train(matrix2_t* train_mat, matrix2_t* train_label_mat, void** Py_count, void** Pxy_count)
{
    // 获取 label 的统计。
    void *py;
    __mat2_list_different(train_label_mat->pool, train_label_mat->rows, &py);

    int size_diff_label = *MAX2_DIFF_SIZE_PTR(Py_count);
    int* label_ptr      = MAX2_DIFF_LIST_PTR(Py_count);
    MAT2_POOL_PTR(train_label_mat, label_mat_ptr);
    MAT2_POOL_PTR(train_mat, train_mat_ptr);

    // 
    size_t table_size = size_diff_label * train_mat->cols * sizeof(char*);
    char* (*table)[train_mat->cols] = malloc (table_size);

    memset(table, 0x0, table_size);
    // 
    
    // 建立统计表。
    vfloat_t temp_value[train_mat->cols];
    int temp_size;

    for (int i=0; i<size_diff_label; ++i) {
        vfloat_t label = label_ptr[i];

        for (int j=0; j<train_mat->cols; ++j) {

            temp_size = 0;
            for (int k=0; k<train_mat->rows; ++k) {
                if (label == label_mat_ptr[j][0]) {
                    temp_value[temp_size++] = train_mat_ptr[k][j];
                }
            } 
            void* diff;
            __mat2_list_different(temp_value, temp_size, &diff);
            table[i][j] = diff;
        }
    }

    *Py_count = py;
    *Pxy_count = table;
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
int navie_bayes_predict(matrix2_t* _X, void* Py_count, void* Pxy_count)
{



}