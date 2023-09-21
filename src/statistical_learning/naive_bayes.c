
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <float.h>
#include <math.h>
#include "counting.h"
#include "naive_bayes.h"

#define PI 3.1415926
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
    //__mat2_count_element(train_label_mat->pool, train_label_mat->rows, Py_counting);
    counting_Y(train_label_mat, Py_counting);

    if (Px_y_counting_table) {

        counting_XY(train_label_mat, train_mat, Px_y_counting_table);

        // int size_eleme = *MAT2_COUNTING_SIZE_PTR(*Py_counting);
        // vfloat_t* elem_ptr  = MAT2_COUNTING_LIST_PTR(*Py_counting);
        // MAT2_POOL_PTR(train_label_mat, label_mat_ptr);
        // MAT2_POOL_PTR(train_mat, train_mat_ptr);

        // // 多申请多两个单位，用于存储横列信息。
        // size_t table_size = 2 * sizeof(int) + size_eleme * train_mat->cols * sizeof(char*);
        // void* table1 = malloc( table_size );
        // memset(table1, 0x0, table_size);
        // ((int*)table1)[0] = size_eleme;
        // ((int*)table1)[1] = train_mat->cols;

        // char* (*table_ptr)[train_mat->cols] = &(((int*)table1)[2]);
        // // 
    
        // // 建立统计表。
        // vfloat_t elem_value[train_mat->rows];
        // int elem_size;

        // for (int i=0; i<size_eleme; ++i) {
        //     int label = (int)elem_ptr[i];

        //     for (int j=0; j<train_mat->cols; ++j) {

        //         elem_size = 0;

        //         for (int k=0; k<train_mat->rows; ++k) {
        //             //
        //             int Y_label = label_mat_ptr[k][0];

        //             if (label == Y_label) {
        //                 elem_value[elem_size++] = train_mat_ptr[k][j];
        //             }
        //         } 
        //         void* out;
        //         __mat2_count_element(elem_value, elem_size, &out);
        //         table_ptr[i][j] = out;
        //     }
        // }
        // *Px_y_counting_table = table1;
    }

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
    int       Py_counting_size        = CTY_size(Py_counting);//*MAT2_COUNTING_SIZE_PTR(Py_counting);
    int*      Py_counting_numbers_ptr = CTY_elems_number_ptr(Py_counting);//MAT2_COUNTING_NUMBERS_PTR(Py_counting);
    vfloat_t* Py_element_ptr          = CTY_elems_ptr(Py_counting);//MAT2_COUNTING_LIST_PTR(Py_counting);

    int y_total = 0;

    for (int i=0; i<Py_counting_size; ++i) {
        y_total += Py_counting_numbers_ptr[i];
    }

    int probability_size = CTY_size(Py_counting);//*MAT2_COUNTING_SIZE_PTR(Py_counting);

    float probability_logs[probability_size];
    char* (*Pxy_counting_table_ptr)[_X->cols] = &(((int*)Pxy_counting_table)[2]);
    
    for (int i=0; i<probability_size; ++i) {
        float pro_log = 0.f;
        vfloat_t y_label = Py_element_ptr[i];

        // 计算 _X 在每个 lable 中可能的概率
        int y_number = counting_get_elem_number(Py_counting, y_label);//__mat2_get_element_number(Py_counting, y_label);

        pro_log += log( (float) y_number / y_total);

        for (int j=0; j<_X->cols; ++j) {

            vfloat_t x_value   = _X->pool[j];
            void* counting     = Pxy_counting_table_ptr[i][j];
            int si             = CTY_size(counting);//*MAT2_COUNTING_SIZE_PTR(counting);
            int x_value_number = counting_get_elem_number(counting, x_value);//__mat2_get_element_number(counting, x_value);

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

int navie_bayes_train_MGD_edit(matrix2_t* train_mat, matrix2_t* train_label_mat, void** Py_counting, void** mus_table, void** sigma_table, void (*progress)(char*, unsigned long, unsigned long))
{

    MAT2_POOL_PTR(train_mat, train_mat_ptr);
    MAT2_POOL_PTR(train_label_mat, train_label_ptr);

    void* label_counting; 

    navie_bayes_counting(train_mat, train_label_mat, &label_counting, NULL);

    
    int label_count       = CTY_size(label_counting); //*MAT2_COUNTING_SIZE_PTR(label_counting);
    vfloat_t* labels      = CTY_elems_ptr(label_counting);//MAT2_COUNTING_LIST_PTR(label_counting);
    int* label_numbers    = CTY_elems_number_ptr(label_counting);// MAT2_COUNTING_NUMBERS_PTR(label_counting);

    // int pxy_rows = ((int*)pxy_counting_table)[0];
    // int pxy_cols = ((int*)pxy_counting_table)[1];
    // char* (*pxy_tab_ptr)[pxy_cols] = &(((int*)pxy_counting_table)[2]);
    
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
    if (progress) progress("计算均值向量", 0, 0);

    for (int i=0; i<train_mat->rows; ++i) {
        for (int j=0; j<train_mat->cols; ++j) {

            // 获取本行的 label 。
            vfloat_t label = train_label_ptr[i][0];
            
            // 根据 label 获取 label 在 label_counting 中序号。
            int l_index = counting_get_elem_pos(label_counting, label);//__mat2_get_elemt_pos(label_counting, label);

            // 根据 label 序号，和第 j 个属性，获取在该 label 下 j 属性总数。
            int value_element_total = label_numbers[l_index]; //__mat2_get_element_number(label_counting, label);
            
            // 把该 label 下 j 属性的值 / 总数，再将他们加到一起便是 均值。放入均值表中
            mus_ptr[l_index][j] += train_mat_ptr[i][j] / (vfloat_t) value_element_total;

            if (progress) progress("计算均值向量", (i*train_mat->cols+j), (train_mat->rows * train_mat->cols));
            
        }
    }
    

    // 计算协方差矩阵
    if (progress) progress("计算协方差", 0, 0);

    for (int i=0; i<sigma_z; ++i) {

        vfloat_t i_label = labels[i];

        for (int j=0; j<sigma_rows; ++j) {

            for (int k=0; k<sigma_cols; ++k) {
                // 因为协方差矩阵是对称矩阵，只求左上脚的数即可。
                if (k >= j) {

                    // 左上角进行计算。
                    int i_label_count = label_numbers[i];

                    vfloat_t j_tmp_elem[i_label_count];
                    vfloat_t* j_tmp_elem_ptr = j_tmp_elem;

                    vfloat_t k_tmp_elem[i_label_count];
                    vfloat_t* k_tmp_elem_ptr = k_tmp_elem;

                    // 这里可以优化一下。
                    for (int l=0; l<train_mat->rows; ++l) {

                        if (i_label == train_label_ptr[l][0]) {
                            *j_tmp_elem_ptr++ = train_mat_ptr[l][j];
                            *k_tmp_elem_ptr++ = train_mat_ptr[l][k];
                        }
                    
                        //  拿够数据就不用在拿了。
                        if (j_tmp_elem_ptr == &j_tmp_elem[i_label_count])
                            break;
                    
                    }

                    vfloat_t cov_j_k = 0.f;
                    vfloat_t mu_j = mus_ptr[i][j];
                    vfloat_t mu_k = mus_ptr[i][k];

                    for (int p=0; p<i_label_count; ++p) {
                        vfloat_t _v = (j_tmp_elem[p] - mu_j) * (k_tmp_elem[p] - mu_k);
                        cov_j_k += _v;
                    }

                    // 协方差矩阵书上第5页，还必须除以一个 (Nk-1) 但是如果 Nk 为一的话，就错了。
                    sigma_tab_ptr[i][j][k] = cov_j_k / (i_label_count != 1?  i_label_count - 1 : 1);

                } else {
                    sigma_tab_ptr[i][j][k] = sigma_tab_ptr[i][k][j];
                }

                if (progress) progress("计算协方差", i*sigma_rows*sigma_cols+j*sigma_cols+k, sigma_z*sigma_rows*sigma_cols);

            }
        }
    }

    *mus_table   = mus_block;
    *sigma_table = sigma_block;
    *Py_counting = label_counting;
    //navie_bayes_release_pxy_counting_table(pxy_counting_table);
    return 0;
}

// int navie_bayes_release_pxy_counting_table(void* Pxy_counting_table) {
//     int rows = ((int*)Pxy_counting_table)[0];
//     int cols = ((int*)Pxy_counting_table)[1];
//     char* (*Pxy_count_table_ptr)[cols] = &(((int*)Pxy_counting_table)[2]);

//     for (int i=0; i<rows; ++i) {
//         for (int j=0; j<cols; ++j) {
//             void* counting = Pxy_count_table_ptr[i][j];
//             free(counting);
//         }
//     }
//     free(Pxy_counting_table);
//     return 0;
// }

/**
 * @brief 
 * 
 * @param _X 
 * @param py_counting 
 * @param mus 
 * @param sigma_table 
 * @param predict 
 * @return int 
 */
int navie_bayes_predict_MGD_edit(matrix2_t* _X, void* py_counting, void* mus, void* sigma_table, vfloat_t* predict) 
{
    // matrix2_t* _X_cpy = Mat2_create_cpy(_X);

    // // 此处确保 _X 是列向量。
    // if (_X_cpy->rows < _X_cpy->cols) {
    //     // 如果是树立向量，将其立起来。
    //     Mat2_T(_X_cpy);
    // }

    int size_label  = CTY_size(py_counting);//*MAT2_COUNTING_SIZE_PTR(py_counting);
    vfloat_t* elems = CTY_elems_ptr(py_counting); // MAT2_COUNTING_LIST_PTR(py_counting);

    int mu_col = ((int*)mus)[1];
    vfloat_t (*mu_ptr)[mu_col] = &(((int*)mus)[2]);
    
    int sigma_row = ((int*)sigma_table)[1];
    int sigma_col = ((int*)sigma_table)[2];
    vfloat_t (*sigma_ptr)[sigma_row][sigma_col] = &(((int*)sigma_table)[3]);

    // for debug 
    // size_label = 1;

    vfloat_t probabilities[size_label];
    matrix2_t* sigma_mat = Mat2_create(1,1);
    matrix2_t* mu_mat    = Mat2_create(1,1);
    matrix2_t* _X_cpy    = Mat2_create(1,1);
    matrix2_t* _X_cpy_T  = Mat2_create(1,1);

    int n = sigma_row;

    // 1 首先计算各种 label 的概率
    for (int i=0; i<size_label; ++i) {

        // 每一轮的开始都要重新初始化各个 Mat !!!!!
        vfloat_t PROBABILITY = 0.f;
        // 初始化 
        Mat2_cpy(_X_cpy, _X);

        if (_X_cpy->rows < _X_cpy->cols) {
            // 如果是横向量，将其立起来，变成列向量
            Mat2_T(_X_cpy);
        }

        Mat2_load_on_shape(sigma_mat, sigma_ptr[i], sigma_row, sigma_col);
        Mat2_load_on_shape(mu_mat, mu_ptr[i], mu_col, 1);

        vfloat_t det;
        Mat2_det(sigma_mat, &det);

        PROBABILITY += log(det);

        Mat2_sub(_X_cpy, mu_mat);
        Mat2_cpy(_X_cpy_T, _X_cpy);

        Mat2_T(_X_cpy_T);


        Mat2_inverse(sigma_mat);
        
        Mat2_dot(_X_cpy_T, sigma_mat);
        

        Mat2_dot(_X_cpy_T, _X_cpy);


        PROBABILITY += _X_cpy_T->pool[0];
        PROBABILITY += n*log(2*PI);

        PROBABILITY = PROBABILITY * (-0.5f);

        probabilities[i] = PROBABILITY;
    }

    // 2 找出最大概率哪个。
    vfloat_t max_p = probabilities[0];
    int max        = 0;

    for (int i=1; i<size_label; ++i) {

        if (max_p < probabilities[i]) {
            max_p = probabilities[i];
            max = i;
        }
    }

    *predict = elems[max];

    Mat2_destroy(_X_cpy);
    Mat2_destroy(sigma_mat);
    Mat2_destroy(mu_mat);
    Mat2_destroy(_X_cpy_T);

    return 0;
}

/**
 * @brief 计算多维高斯分布。使用 QR 分解协方差矩阵。
 * 
 * @param _X 
 * @param py_counting 
 * @param mus 
 * @param sigma_table 
 * @param predict 
 * @return int 
 */
int navie_bayes_predict_MGD2_edit(matrix2_t* _X, void* py_counting, void* mus, void* sigma_table, vfloat_t* predict)
{
    int size_label  = CTY_size(py_counting);       //*MAT2_COUNTING_SIZE_PTR(py_counting);
    vfloat_t* elems = CTY_elems_ptr(py_counting);  // MAT2_COUNTING_LIST_PTR(py_counting);

    int mu_col = ((int*)mus)[1];
    vfloat_t (*mu_ptr)[mu_col] = &(((int*)mus)[2]);
    
    int sigma_row = ((int*)sigma_table)[1];
    int sigma_col = ((int*)sigma_table)[2];
    vfloat_t (*sigma_ptr)[sigma_row][sigma_col] = &(((int*)sigma_table)[3]);

    matrix2_t* sigma_mat = Mat2_create(1,1);
    matrix2_t* w_mat     = Mat2_create(1,1);
    matrix2_t* u_mat     = Mat2_create(1,1);
    matrix2_t* mu_mat    = Mat2_create(1,1);

    matrix2_t* _X_cpy    = Mat2_create(1,1);
    matrix2_t* _X_cpy_T  = Mat2_create(1,1);

    vfloat_t probabilities[size_label];
    int n = sigma_row;
    // 1 计算各种 label 的概率。
    for (int i=0; i<size_label; ++i) {

        vfloat_t PROBABILITY = 0.f;


        Mat2_cpy(_X_cpy, _X);

        if (_X_cpy->rows > _X_cpy->cols) {
            // 如果是列向量，将其立起来，变成行向量
            Mat2_T(_X_cpy);
        }

        Mat2_load_on_shape(sigma_mat, sigma_ptr[i], sigma_row, sigma_col);
        Mat2_load_on_shape(mu_mat, mu_ptr[i], 1, mu_col);

        
        Mat2_sub(_X_cpy, mu_mat);

        // 求 sigma 的反矩阵
        Mat2_inverse(sigma_mat);

        // 分解 sigma_mat
        Mat2_eig(w_mat, u_mat, sigma_mat);

        Mat2_dot(_X_cpy, u_mat);
        
        Mat2_cpy(_X_cpy_T, _X_cpy);

        Mat2_T(_X_cpy_T);

        Mat2_hadamard_product(_X_cpy, w_mat);
        
        // for (int j=0; j<w_mat->cols; ++j) {
        //     _X_cpy->pool[j] *= w_mat->pool[j];
        // }

        Mat2_dot(_X_cpy, _X_cpy_T);

        double det_sigma = 1.f;

        for (int k=0; k<w_mat->cols; ++k) {
            det_sigma *= 1.f / w_mat->pool[k];
        }

        PROBABILITY += log(det_sigma);
        PROBABILITY += _X_cpy->pool[0];
        PROBABILITY += n*log(2*PI);
        PROBABILITY *= (-0.5f);

        probabilities[i] = PROBABILITY;

    }
    
    // 2 找出最大可能哪一个
    vfloat_t max_p = probabilities[0];
    int      max   = 0;

    for (int i=1; i<size_label; ++i) {

        if (max_p < probabilities[i]) {
            max_p = probabilities[i];
            max = i;
        }
    }

    *predict = elems[max];

    // 3 清空内存
    Mat2_destroy(sigma_mat);
    Mat2_destroy(w_mat);     
    Mat2_destroy(u_mat);     
    Mat2_destroy(mu_mat);

    Mat2_destroy(_X_cpy);
    Mat2_destroy(_X_cpy_T);

    return 0;

}
