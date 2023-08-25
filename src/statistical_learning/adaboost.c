#include "adaboost.h"

/**
 * @brief adaboost tree 使用的是分类树
 * 
 * @param train_data 训练的数据
 * @param train_label {-1, 1} 的标签数据
 * @param M 弱分类树的数量
 * @param term 弱分类树的结束条件
 * @param alpahs 弱分类树的权值
 * @param trees 返回弱分类树列表
 * @return int 
 */
int adaboost_tree_train(matrix2_t* train_data, matrix2_t* train_label, int M, dct_term_t* term, double* alpahs,  dc_tree_t** trees)
{

    int N = train_label->rows;

    matrix2_t* W = Mat2_create(train_label)
    

}

int adaboost_tree_predict(vfloat_t* predict)
{

}
