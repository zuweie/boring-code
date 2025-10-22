/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-10-20 09:53:47
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-10-22 11:06:12
 * @FilePath: /boring-code/src/reinforce_learning/neural_network_functions.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <float.h>
#include <math.h>
#include "matrix2/matrix2.h"
#include "neural_network_functions.h" 


static const float relu_alpha = 1.f;

static inline float __relu (float v, float alpha) 
{
    double ev = exp(v);
    float res = alpha * (ev - 1);
    return res;
}

static inline float __sigmoid(float x) {
    return 1.f / (1 + exp( -x ));
}

int relu(matrix2_t* m)
{
    int number = m->rows * m->cols;
    for (int i=0; i<number; ++i) {
        if (m->pool[i] < 0) {
            float res = __relu(m->pool[i], relu_alpha);
            m->pool[i] = res;
        }
    }
    return 0;
}
int gradient_relu(matrix2_t* m)
{
    int number = m->rows * m->cols;
    for (int i=0; i<number; ++i) {
        if (m->pool[i] < 0) {
            float v    = __relu(m->pool[i], relu_alpha) + relu_alpha;
            m->pool[i] = v;
        }
    }
    return 0;
}

int sigmoid1 (matrix2_t* vec)
{
    int length = vec->rows * vec->cols;

    for (int i=0; i<length; ++i) {
        vec->pool[i] = __sigmoid(vec->pool[i]);
    }
    return 0;
}

int gradient_sigmoid1 (matrix2_t* vec)
{
    int length = vec->rows * vec->cols;

    for (int i=0; i<length; ++i) {
        double sigmoid = __sigmoid(vec->pool[i]);
        vec->pool[i] = sigmoid * ( 1 - sigmoid );
    }
    return 0;
}


int useless_output(matrix2_t* m)
{
    return 0;
}
int gradient_useless_output(matrix2_t* m)
{
    Mat2_fill(m, 1.f);
    return 0;
}

float mse(matrix2_t* m1, matrix2_t* m2)
{
    MAT2_POOL_PTR(m1, m1_ptr);
    MAT2_POOL_PTR(m2, m2_ptr);

    float result = 0.f;
    float mod ;
    for (int j=0; j<m1->cols; ++j) {

        mod = 0.f;

        for (int i=0; i<m1->rows; ++i) {
            mod += (m1_ptr[i][j] - m2_ptr[i][j]) * (m1_ptr[i][j] - m2_ptr[i][j]);
        }

        result += sqrt(mod);
    }
    return (2.f / (float) m1->cols) * result;
}

int gradient_mse(matrix2_t* m1, matrix2_t* m2)
{
    // int number = m1->rows * m1->cols;

    Mat2_sub(m1, m2);
    Mat2_scalar_multiply(m1, 1.f / ((double) (m1->cols)));    
    return 0;
}

int softmax1(matrix2_t* m1)
{
    
    float total = 0.f;
    float max_v;
    MAT2_POOL_PTR(m1, m1_ptr);

    int i,j;
    for (j=0; j<m1->cols; ++j) {
        total = 0.f;
        max_v = -FLT_MAX;

        for (i=0; i<m1->rows; ++i) {
            if (max_v < m1_ptr[i][j]) {
                max_v = m1_ptr[i][j];
            }
        }

        for (i=0; i<m1->rows; ++i) {
            double v = m1_ptr[i][j] - max_v;
            double ev = exp(v);
            total += ev;
            //total += exp(m1_ptr[i][j]);
        }

        for (i=0; i<m1->rows; ++i) {
            double v = m1_ptr[i][j] - max_v;
            double ev = exp(v);
            double res = ev / total;
            m1_ptr[i][j] = res < 1e-10 ? 1e-10 : res;
        }
    }

    return 0;
}

int gradient_softmax1(matrix2_t* m) 
{
    Mat2_fill(m, 1);
    return 0;
}

float crossentropy (matrix2_t* yk, matrix2_t* ak) 
{
    
    float total = 0.f;
    float cross  = 0.f;

    MAT2_POOL_PTR(yk, yk_ptr);
    MAT2_POOL_PTR(ak, ak_ptr);

    for (int j=0; j<yk->cols; ++j) {

        cross = 0.f;

        for (int i=0; i<yk->rows; ++i) {
            double log_ak_v = log(ak_ptr[i][j]);
            cross += yk_ptr[i][j] * log_ak_v;
        } 

        total += -cross;
    }
    return (1 / ((float) yk->rows)) * total;
}

int gradient_corssentropy(matrix2_t* ak, matrix2_t* yk)
{
    Mat2_sub(ak, yk);
    return 0;
}

