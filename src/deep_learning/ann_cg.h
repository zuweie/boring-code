/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2024-09-04 12:22:25
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2024-09-05 10:52:43
 * @FilePath: /boring-code/src/deep_learning/ann_cg.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2024-09-04 12:22:25
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2024-09-05 10:08:15
 * @FilePath: /boring-code/src/deep_learning/ann_use_cg.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
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