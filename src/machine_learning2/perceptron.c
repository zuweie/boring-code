/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-03-28 16:58:02
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-03-31 00:40:50
 * @FilePath: /boring-code/src/machine_learning2/perceptron.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */

#include "perceptron.h"

/**
 * @brief 感知机算法，使用梯度下降实现模型的训练，
 * 
 * @param _Data 输入的训练数据, 最后一列为 Y, 其他的X
 * @param _W_b 输出的模型参数 W 以及偏置量 b，向量最后一位为偏置量。
 * @param lr learn rate 学习步进
 * @param max_iter 最大迭代次数
 * @return int 返回结果。
 */
int pct_train(matrix2_t* _Data, matrix2_t** _W_b, float lr, int max_iter)
{
    matrix2_t* _X_1 = Mat2_create(1,1);

    *_W_b = Mat2_create(1, _Data->cols);
    Mat2_fill(*_W_b, 0.f);

    int iter = 0;
    int mistake = 0;

    do {

        mistake = 0;
        for (int i=0; i<_Data->rows; ++i) {

            // 从 _Data 中截取一行的 _X 与 _y
            //Mat_rescale_mat( _Data, _X_1, 0, i, 0, -(_Data->rows-(i+1)), 0);
            Mat2_slice_row_to(_Data, _X_1, i);
            // 提取 _y
            vfloat_t yi = Mat2_get(_X_1, 0, _X_1->cols-1);
            
            // 把最后一位置成 1.f => [x1, x2, x3, ..., 1.f]
            Mat2_put(_X_1, 0, _X_1->cols-1, 1.f);
            
            // 将 [w1, w2, w3, ..., b] dot [x1, x2, x3, ..., 1.f] => out
            vfloat_t out;
            Mat2_vect_dot(*_W_b, _X_1, &out);

            if ( (out * yi) <= 0.f ) {
                //  分类错误,对 _W 与 b 进行调整。
                // TODO : fix _W and b
                
                // 把 yi 与 [x1, x2, x3, ..., 1] 相乘！
                Mat2_scalar_multiply(_X_1, yi);
                // 然后相加到 [w1, w2, w3, ..., b] 上去。
                Mat2_add(*_W_b, _X_1);
                
                mistake = 1;
                break;
                
            }
        }
        iter++;

    } while (iter <= max_iter && mistake);

    return 0;
}

int pct_perdict(matrix2_t* _X, matrix2_t* _W_b, vfloat_t* indicator)
{
    return 0;
}