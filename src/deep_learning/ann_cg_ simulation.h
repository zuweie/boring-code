/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2024-09-04 12:22:25
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2024-09-06 11:02:10
 * @FilePath: /boring-code/src/deep_learning/ann_cg_simulation.h
 * @Description: ann cg simulation 是一个模拟神经网络使用计算图进行训练的程序，为什么要模拟呢，是因为 compute_graph 的实现还未完全完成，有几个细节的
 * 地方需要进行细节的处理。现在只实现了框架，如果真的要实现可能，那么有几个细节必须完成
 */
#ifndef __ANN_CG_H__
#define __ANN_CG_H__
#include "matrix2/matrix2.h"
#include "deep_learning/compute_graph/compute_graph.h"


typedef struct _ann_cg_params
{
    /// @brief 训练结束的精度
    double term_epsilon;
    /// @brief 最大迭代次数
    int max_iter;
    /// @brief 每次训练的量
    int batch;
    /// @brief 梯度下降的速率。
    double learning_rate;
    /// @brief 每一层的参数信息包括 输入层和输出层
    int* layers_dimension;
    /// @brief 层数
    int layers_size;
} ann_cg_params_t;


int ann_cg_train(matrix2_t* p_data, matrix2_t* p_label,  ann_cg_params_t* p_params, compute_graph_t* p_compute_graph_t);
int ann_cg_predict(matrix2_t* p_Input, compute_graph_t* p_compute_graph, matrix2_t* predict);
int ann_recycle();

#endif