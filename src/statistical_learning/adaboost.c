#include <float.h>
#include <math.h>
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
static int calculate_gain_D_A(matrix2_t* _Xi, matrix2_t* _Y, double* gain, vfloat_t* best_split, vfloat_t* predict_label) 
{
    int group_size;
    matrix2_t** group_X;
    matrix2_t** group_Y;
    int D = _Y->rows;

    counting_XY_group_by_x(_Xi, _Y, 0, &group_X, &group_Y, &group_size);
    
    *gain = FLT_MAX;

    if (group_size > 1) {
        // 分开了两组以上的数据。

        // 计算所有的
        for (int i=0; i<group_size; ++i) {

            int sub_d1_index  = i;
            matrix2_t* sub_d1 = group_Y[sub_d1_index];
            
            int sub_d2_index = i==0? 1:0;
            matrix2_t* sub_d2 = Mat2_create_cpy(group_Y[sub_d2_index]);

            // 将 i 以外的所有的向量合并。
            for (int j=0; j<group_size; ++j) {
                if (j!=sub_d1_index && j!=sub_d2_index) {
                    Mat2_merge_rows(sub_d2, group_Y[j]);
                }
            }

            double gain_d1 = 0.f;
            double gain_d2 = 0.f;

            calculate_gain_D(sub_d1, &gain_d1);
            calculate_gain_D(sub_d2, &gain_d2);

            double gain_d_a = (((double) sub_d1->rows / (double) D) * gain_d1) + (  ((double)sub_d1->rows / (double) D) * gain_d2 );

            if (gain_d_a < *gain ) {
                
                void* counting = NULL;
                counting_Y(sub_d1, &counting);

                *gain = gain_d_a;
                // 因为已经分组了，group_X 中所欲的 x 的值都一样。
                *best_split = group_X[i]->pool[0];
                // 返回 sub_d1 中最多那个类别。
                *predict_label = counting_max_frequency(counting);
                free(counting);
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
 * @brief 计算弱分类器的误差，取最小的误差e，然后计算 Gx 的系数 alpha，并且吧 参数数组 W 进行更新。
 * 
 * @param train_data 
 * @param train_label 
 * @param e 
 * @param gx 
 * @return int 
 */
static int calculate_gx_e_alpha_w(matrix2_t* train_data, matrix2_t* train_label, adaboost_gx_t* gx, double W[], double* alpha, int* Gx_index)
{
    int C = train_data->cols;
    int N = train_data->rows;
    MAT2_POOL_PTR(train_data, train_data_ptr);

    double e = FLT_MAX;
    double E;
    int best_gx_index = -1;
    adaboost_gx_t Gx;
    // 遍历每一个弱选择器 Gx，结合 W 最终选出最适合的 Gx。
    for (int i=0; i<C; ++i) {

        Gx = gx[i];
        E = 0.f;
        for (int j=0; j<N; ++j) {
            // 统计统计错误率
            if ( (train_data_ptr[j][i] == Gx.best_split && train_label->pool[j] != Gx.predict ) || 
            (train_data_ptr[j][i] != Gx.best_split && train_label->pool[j] == Gx.predict)) {
                E += W[j];
            } 
        }

        if (E < e) {
            e = E;
            best_gx_index = i;
        }
        
    }

    // 更新新的 W。
    *alpha = 0.5 * log( (1-e) / (e) );

    // 计算规范化因子 Z
    double Z = 0.f;
    Gx = gx[best_gx_index];

    double yGx = 0.f;
    for (int k=0; k<N; ++k) {
        yGx =  train_label->pool[k] * (train_data_ptr[k][Gx.split_A] == Gx.best_split ? Gx.predict : -(Gx.predict));
        Z += W[k] * exp(-(*alpha) * yGx);
    }

    // 更新 W 为下一轮迭代做准备。
    for (int l=0; l<N; ++l) {
        yGx =  train_label->pool[l] * (train_data_ptr[l][Gx.split_A] == Gx.best_split ? Gx.predict : -(Gx.predict));
        W[l] = W[l] / Z * exp(-(*alpha) * yGx );
    }
    
    *Gx_index = best_gx_index;

    return 0;
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
int adaboost_tree_train(matrix2_t* train_data, matrix2_t* train_label, int M, double** alpahs_out,  adaboost_gx_t** Gx_out, void (*progress)(const char*, unsigned long, unsigned long))
{

    *Gx_out     = malloc (M * sizeof(adaboost_gx_t));
    *alpahs_out = malloc (M * sizeof(double));
    // 先生成所有弱 Gx。
    adaboost_gx_t Gxs[train_data->cols];
    matrix2_t* Xi = Mat2_create(1,1);

    for (int i=0; i<train_data->cols; ++i) {

        if (progress) 
            progress("计算 Gx 中... ", i+1, train_data->cols);

        Mat2_slice_col_to(Xi, train_data, i);
        double gain;
        Gxs[i].split_A = i;

        calculate_gain_D_A(Xi, train_label, &gain, &Gxs[i].best_split, &Gxs[i].predict);
    }

    // TODO: 计算数据所有列的 Gain 系数。
    int N = train_label->rows;
    double W[N];

    // 初始化带权系数。
    for (int j=0; j<N; ++j){
        W[j] = 1 / (double) N;
    }
    
    // 计算每一个 alpha 以及 Gx。
    // 遍历每一个 Gx 挑选合适的作为强选择器的组建。
    int best_gx_index;
    double alpha;

    for (int i=0; i<M; ++i) {

        if (progress)
            progress("计算 M * Gx 中...", i+1, M);

        calculate_gx_e_alpha_w(train_data, train_label, Gxs, W, &alpha, &best_gx_index);
        (*Gx_out)[i]     = Gxs[best_gx_index];
        (*alpahs_out)[i] = alpha;
    }


    return 0;

}

int adaboost_tree_predict(matrix2_t* _Input, int M, adaboost_gx_t* Gxs, double* alphas, vfloat_t* predict)
{

    double sign = 0.f;

    for (int i=0; i<M; ++i) {

        double result = alphas[i] * (_Input->pool[Gxs[i].split_A] == Gxs[i].best_split ? Gxs[i].predict : -Gxs[i].predict);
        sign += result;
    }

    *predict = sign > 0 ? 1 : -1;

    return 0;

}


