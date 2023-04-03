/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-03-28 16:58:02
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-04-03 14:53:59
 * @FilePath: /boring-code/src/machine_learning2/perceptron.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>
#include "perceptron.h"

/**
 * @brief 感知机算法，使用梯度下降实现模型的训练，
 * 
 * @param _Data 输入的训练数据, 最后一列为 Y, 其他的X
 * @param _W_b 输出的模型参数 W 以及偏置量 b，向量最后一位为偏置量。
 * @param learning_rate learn rate 学习步进
 * @param max_iter 最大迭代次数
 * @return int 返回结果。
 */
int pct_train(matrix2_t* _Data, matrix2_t** _W_b, float learning_rate, int max_iter)
{
    matrix2_t* _X_1 = Mat2_create(1,1);

    *_W_b = Mat2_create(1, _Data->cols);
    Mat2_fill(*_W_b, 0.f);

    int iter = 0;
    int mistake = 0;

    do {

        printf("\nround %d \n", iter+1);

        mistake = 0;
        for (int i=0; i<_Data->rows; ++i) {

            // 从 _Data 中截取一行的 _X 与 _y
            //Mat_rescale_mat( _Data, _X_1, 0, i, 0, -(_Data->rows-(i+1)), 0);
            Mat2_slice_row_to(_X_1, _Data, i);
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
                printf("point %d mistake \n", i+1);
                printf("before update W b:");
                MAT2_INSPACT(*_W_b);

                // 把 learning_rate yi 与 [x1, x2, x3, ..., 1] 相乘！
                Mat2_scalar_multiply(_X_1, (learning_rate * yi));
                // 然后相加到 [w1, w2, w3, ..., b] 上去。
                Mat2_add(*_W_b, _X_1);
                
                mistake = 1;

                printf("after update W b ");
                MAT2_INSPACT(*_W_b);
                printf("\n");
                
                break;

            }
        }
        iter++;

    } while (iter <= max_iter && mistake);

    Mat2_destroy(_X_1);

    return 0;
}

int pct_perdict(matrix2_t* _X, matrix2_t* _W_b, vfloat_t* perdict)
{
    Mat2_padding_right(_X, 1, 1);
    vfloat_t v;
    Mat2_vect_dot(_X, _W_b, &v);
    *perdict = v >= 0.f? 1: -1;
    return 0;
}