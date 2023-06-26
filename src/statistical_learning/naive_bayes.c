/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-06-16 14:50:03
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-06-26 17:11:52
 * @FilePath: /boring-code/src/statistical_learning/naive_bayes.c
 * @Description: 
 */
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include "naive_bayes.h"


/**
 * @brief 统计每个列别下每种数据的数量。
 * 
 * @param train_mat 
 * @param train_label_mat 
 * @param Py_probability
 * @param Pxy_probability 
 * @return int 
 */
int navie_bayes_counting(matrix2_t* train_mat, matrix2_t* train_label_mat, void** Py_counting, void** Px_y_counting_table)
{
    // 获取 label 的统计。
    __mat2_count_element(train_label_mat->pool, train_label_mat->rows, Py_counting);
    int size_eleme = *MAT2_COUNTING_SIZE_PTR(*Py_counting);
    vfloat_t* elem_ptr  = MAT2_COUNTING_LIST_PTR(*Py_counting);
    MAT2_POOL_PTR(train_label_mat, label_mat_ptr);
    MAT2_POOL_PTR(train_mat, train_mat_ptr);

    // 多申请多两个单位，用于存储横列信息。
    size_t table_size = 2 * sizeof(int) + size_eleme * train_mat->cols * sizeof(char*);
    void* table1 = malloc( table_size );
    memset(table1, 0x0, table_size);
    ((int*)table1)[0] = size_eleme;
    ((int*)table1)[1] = train_mat->cols;

    char* (*table_ptr)[train_mat->cols] = &(((int*)table1)[2]);
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
            table_ptr[i][j] = out;
        }
    }

    *Px_y_counting_table = table1;
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
    char* (*Pxy_counting_table_ptr)[_X->cols] = &(((int*)Pxy_counting_table)[2]);
    
    for (int i=0; i<probability_size; ++i) {
        float pro_log = 0.f;
        vfloat_t y_label = Py_element_ptr[i];

        // 计算 _X 在每个 lable 中可能的概率
        int y_number = __mat2_get_element_number(Py_counting, y_label);

        pro_log += log( (float) y_number / y_total);

        for (int j=0; j<_X->cols; ++j) {

            vfloat_t x_value   = _X->pool[j];
            void* counting     = Pxy_counting_table_ptr[i][j];
            int si             = *MAT2_COUNTING_SIZE_PTR(counting);
            int x_value_number = __mat2_get_element_number(counting, x_value);

            // 需要做拉普拉斯平滑
            float px_y = (float)(x_value_number + lambda) / (float)(y_number + si * lambda);
            pro_log += log(px_y);
           
        }

        probability_logs[i] = pro_log;
    }

    // 最后返回概率最大的那个类别。

    float max_prob = probability_logs[0];
    int   max_indx = 0;
    for (int k=1; k<probability_size; ++k) {
        if (max_prob < probability_logs[k]){
            max_prob = probability_logs[k];
            max_indx = k;
        }
    }

    *predict = Py_element_ptr[max_indx];

    return 0;
}

int navie_bayes_train_MGD_edit(matrix2_t* train_mat, matrix2_t* train_label_mat, void** mus_table, void** sigma_table)
{

    MAT2_POOL_PTR(train_mat, train_mat_ptr);
    MAT2_POOL_PTR(train_label_mat, train_label_ptr);

    void* label_counting; void* pxy_counting_table;

    navie_bayes_counting(train_mat, train_label_mat, &label_counting, &pxy_counting_table);

    
    int label_count       = *MAT2_COUNTING_SIZE_PTR(label_counting);
    vfloat_t* labels      = MAT2_COUNTING_LIST_PTR(label_counting);
    int* label_numbers    = MAT2_COUNTING_NUMBERS_PTR(label_counting);

    int pxy_rows = ((int*)pxy_counting_table)[0];
    int pxy_cols = ((int*)pxy_counting_table)[1];
    char* (*pxy_tab_ptr)[pxy_cols] = &(((int*)pxy_counting_table)[2]);
    
    int mus_rows = label_count;
    int mus_cols = train_mat->cols;
    int mus_size = 2*sizeof(int)+mus_rows*mus_cols*sizeof(vfloat_t);
    void* mus_block = malloc(mus_size);
    memset(mus_block, 0x0, mus_size);

    // 初始化数据
    ((int*)mus_block)[0] = mus_rows;
    ((int*)mus_block)[1] = mus_cols;
    vfloat_t (*mus_ptr)[mus_cols] = &(((int*)mus_block)[2]);
    
    

    int sigma_z    = label_count;
    int sigma_rows = train_mat->cols;
    int sigma_cols = train_mat->cols;
    int sigma_size = 3*sizeof(int)+sigma_z*sigma_rows*sigma_cols*sizeof(vfloat_t);
    void* sigma_block = malloc(sigma_size);
    // 全部置 0;
    memset(sigma_block, 0x0, sigma_size);

    // 初始化数据。
    ((int*)sigma_block)[0] = sigma_z;
    ((int*)sigma_block)[1] = sigma_rows;
    ((int*)sigma_block)[2] = sigma_cols;
    vfloat_t (*sigma_tab_ptr)[sigma_rows][sigma_cols] = &(((int*)sigma_block)[3]);


    // 开始计算均值。

    // 1 第一步先计算各个元素在各自的 label 下的总数。
    for (int i=0; i<train_mat->rows; ++i) {
        for (int j=0; j<train_mat->cols; ++j) {

            // 获取本行的 label 。
            vfloat_t label = train_label_ptr[i][0];
            
            // 根据 label 获取 label 在 label_counting 中序号。
            int l_index = __mat2_get_elemt_pos(label_counting, label);

            // 根据 label 序号，和第 j 个属性，获取在该 label 下 j 属性总数。
            int value_element_total = __mat2_get_element_number(label_counting, label);
            
            // 把该 label 下 j 属性的值 / 总数，再将他们加到一起便是 均值。放入均值表中
            mus_ptr[l_index][j] += train_mat_ptr[i][j] / (vfloat_t) value_element_total;
        }
    }
    

    // 计算协方差矩阵

    for (int i=0; i<sigma_z; ++i) {

        vfloat_t i_label = labels[i];

        for (int j=0; j<sigma_rows; ++j) {

            for (int k=0; k<sigma_cols; ++k) {

                int i_label_count = label_numbers[i];

                vfloat_t j_tmp_elem[i_label_count];
                vfloat_t* j_tmp_elem_ptr = j_tmp_elem;

                vfloat_t k_tmp_elem[i_label_count];
                vfloat_t* k_tmp_elem_ptr = k_tmp_elem;

                for (int l=0; l<train_mat->rows; ++l) {

                    if (i_label == train_label_ptr[l][0]) {
                        *j_tmp_elem_ptr++ = train_mat_ptr[l][j];
                        *k_tmp_elem_ptr++ = train_mat_ptr[l][k];
                    }
                }

                vfloat_t cov_j_k = 0.f;
                vfloat_t mu_j = mus_ptr[i][j];
                vfloat_t mu_k = mus_ptr[i][k];

                for (int p=0; p<i_label_count; ++p) {
                    vfloat_t _v = (j_tmp_elem[p] - mu_j) * (k_tmp_elem[p] - mu_k);
                    cov_j_k += _v;
                }
                
                sigma_tab_ptr[i][j][k] = cov_j_k / (i_label_count - 1);
            }
        }
    }

    *mus_table   = mus_block;
    *sigma_table = sigma_block;
    return 0;
}

int navie_bayes_release_counting(void* Py_counting, void* Pxy_count_table)
{
    int rows = ((int*)Pxy_count_table)[0];
    int cols = ((int*)Pxy_count_table)[1];
    char* (*Pxy_count_table_ptr)[cols] = &(((int*)Pxy_count_table)[2]);

    for (int i=0; i<rows; ++i) {
        for (int j=0; j<cols; ++j) {
            void* counting = Pxy_count_table_ptr[i][j];
            free(counting);
        }
    }
    free(Pxy_count_table);
    free(Py_counting);
    return 0;
}
