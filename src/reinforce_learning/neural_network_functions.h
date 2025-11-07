/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-10-20 09:54:00
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-11-05 21:54:06
 * @FilePath: /boring-code/src/reinforce_learning/neural_network_function.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AED
 */
#ifndef __NEURAL_NETWORK_FUNCTIONS_H__
#define __NEURAL_NETWORK_FUNCTIONS_H__

typedef struct matrix2 matrix2_t;

int relu(matrix2_t*);
int gradient_relu(matrix2_t*);

int useless_output(matrix2_t*);
int gradient_useless_output(matrix2_t*);

float mse(matrix2_t*, matrix2_t*);
int gradient_mse(matrix2_t*, matrix2_t*);

int sigmoid1 (matrix2_t*);
int gradient_sigmoid1 (matrix2_t*);

int softmax1 (matrix2_t* m1);
int softmax2 (matrix2_t* m1);
int gradient_softmax1 (matrix2_t* m);

float crossentropy (matrix2_t* yk, matrix2_t* ak);
int gradient_corssentropy(matrix2_t* ak, matrix2_t* yk);

int useless_loss(matrix2_t* y_hat, matrix2_t* y);
int gradient_useless_loss(matrix2_t* y_hat, matrix2_t* y);

#endif