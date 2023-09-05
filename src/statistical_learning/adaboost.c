#include "counting.h"
#include "adaboost.h"

/**
 * @brief 计算误差率以及生成 Gx 的弱分类器
 * 
 * @param train_data 
 * @param train_label 
 * @param e 
 * @param gx 
 * @return int 
 */
int calculate_gx_e(matrix2_t* train_data, matrix2_t* train_label, double W[], double* e, adaboost_gx_t* gx)
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
