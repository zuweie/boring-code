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
 * @param _Wb 输出的模型参数 W 以及偏置量 b，向量最后一位为偏置量。
 * @param lr learn rate 学习步进
 * @param max_iter 最大迭代次数
 * @return int 返回结果。
 */
int pct_train(matrix_t* _Data, matrix_t* _Wb, float lr, int max_iter)
{
    
    *_Wb = Mat_create(_Data->cols, 1);
    matrix_t _X1 = Mat_create(1,1);

    // 将 W 以及 b 初始化为 0.
    Mat_fill(_Wb, 0.f);

    int train_count = 0; 
    int ret         = 1;

    while (train_count <= max_iter) {
        
        // 检索在当前 W 与 b 下，有分类错误的数据。
        int wrong_i = -1, 
        for (int i=0; i<_Data->rows; ++i) {

            Mat_copy(&_X1, _Data);

            // 取第 i 行, 获得 1 x n 的向量
            Mat_rescale(&_X1, 0, i, 0, -(_X1.rows -(i+1)));

            // 1 x n 向量最后一位时 y
            vfloat_t yi = _X1->pool[_X1.cols-1];

            // 取出 y 后把最后一位置 1.
            _X1->pool[_X1.cols-1] = 1.f;

            vfloat_t result = Mat_vector_dot(_Wb, &_X1);

            if (result <= 0.f) {
                
            }

        }

        
        train_count++;

    }

}

int pct_perdict(matrix_t* _X, matrix_t* w, vfloat_t* b)
{
    
}