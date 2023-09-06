#include <float.h>
#include "counting.h"
#include "adaboost.h"

/**
 * @brief 一个集合中的基尼指数
 * 
 * @param label 
 * @return int 
 */
static int calculate_gain_D(matrix2_t* _Y, double* gain) 
{

    double GAIN = 0.f;

    double D = _Y->rows;

    void* count = NULL;
    counting_Y(_Y, &count);

    int count_number      = CTY_size(count);
    int* elems_number_ptr = CTY_elems_number_ptr(count);

    double prob = 0.f;

    for (int i=0; i<count_number; ++i) {
        prob = (double)elems_number_ptr[i] / D;
        GAIN += prob * prob;
    }

    *gain = 1 - GAIN;
    free(count);
    return 0;
}

/**
 * @brief 计算训练数据某一列的基尼系数，以及它的最佳分割数组。
 * 
 * @param _Xi 
 * @param _Y 
 * @param gain 
 * @param best_split 
 * @return int int 
 */
static int int calculate_gain_D_A(matrix2_t* _Xi, matrix2_t* _Y, double* gain, vfloat_t* best_split) 
{
    int group_size;
    matrix2_t** group_X;
    matrix2_t** group_Y;
    double gain = 0.f;
    int D = _Y->rows;

    counting_XY_group_by_x(_Xi, _Y, 0, &group_X, &group_Y, &group_size);
    
    *gain = FLT_MAX;

    if (group_size > 1) {
        // 分开了两组以上的数据。

        // 计算所有的
        for (int i=0; i<group_size; ++i) {

            int sub_d1_index  = 1;
            matrix2_t* sub_d1 = group_Y[sub_d1_index];
            
            int sub_d2_index = i==0? 1:i;
            matrix2_t* sub_d2 = Mat2_create_cpy(group_Y[sub_d2_index]);

            // 将 i 以外的所有的向量合并。
            for (int j=0; j<group_size; ++j) {
                if (j!=sub_d1_index && j!=sub_d2_index) {
                    Mat2_merge(sub_d2, group_Y[j]);
                }
            }

            double gain_d1 = 0.f;
            double gain_d2 = 0.f;

            calculate_gain_D(sub_d1, &gain_d1);
            calculate_gain_D(sub_d2, &gain_d2);

            double gain_d_a = (((double) sub_d1->rows / (double) D) * gain_d1) + (  ((double)sub_d1->rows / (double) D) * gain_d2 );

            if (gain_d_a < *gain ) {
                *gain = gain_d_a;
                *best_split = sub_d1->pool[0];
            }
            Mat2_destroy(sub_d2);
        }
    } else {
        // 只有一组的话
        calculate_gain_D(group_Y[0], gain);
        *best_split = group_X[0]->pool[0];
    }

    for (int i=0; i<group_size; ++i) {
        Mat2_destroy(group_X[i]);
        Mat2_destroy(group_Y[i]);
    }
    // 
    free(group_X);
    free(group_Y);

    return 0;
}

/**
 * @brief 计算误差率以及生成 Gx 的弱分类器
 * 
 * @param train_data 
 * @param train_label 
 * @param e 
 * @param gx 
 * @return int 
 */
static int calculate_gx_e(matrix2_t* train_data, matrix2_t* train_label, double W[], double* e, adaboost_gx_t* gx)
{
    int N = train_data->rows;

    matrix2_t* Xi = Mat2_create(1,1);
    
    for (int i=0; i<train_data->cols; i++) {

        void* counting = NULL;
        Mat2_slice_col_to(Xi, train_data, i);
    }
}

/**
 * @brief adaboost tree 使用的是分类树，what the fuck! 搞不懂怎么弄这个破玩意啊。
 * 
 * @param train_data 训练的数据
 * @param train_label {-1, 1} 的标签数据
 * @param M 弱分类树的数量
 * @param term 弱分类树的结束条件
 * @param alpahs 弱分类树的权值
 * @param trees 返回弱分类树列表
 * @return int 
 */
int adaboost_tree_train(matrix2_t* train_data, matrix2_t* train_label, int M, double* alpahs,  adaboost_gx_t* tree)
{
    // 准备好参数 W
    int N = train_label->rows;
    double W[N];


    for (int j=0; j<N; ++j){
        W_mat[j] = 1 / (double) N;
    }
    

    // 计算每一个 alpha 以及 Gx。
    for (int i=0; i<M; ++i) {
        
        

    }


    return 0;

}

int adaboost_tree_predict(vfloat_t* predict)
{

}
