/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-11-26 08:17:43
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-11-27 13:35:26
 * @FilePath: /boring-code/src/reinforce_learning/nn2_znode.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __NN_2_ZNODE_H__

typedef struct matrix2 matrix2_t;
typedef struct znode2 {

    char name[128];
    matrix2_t* x;
    matrix2_t* Gx;
    struct znode2* prev;
    struct znode2* next;
    int (*calculate)(struct znode2*, matrix2_t*);
    int (*gradient)(struct znode2*, matrix2_t*);
    int (*recycle)(struct znode2*);
} znode2_t;

typedef struct linear {
    znode2_t base;
}linear_t;

typedef struct relu {
    znode_t base;
}relu_t;

typedef struct softmax {
    znode2_t base;
} softmax_t;

typedef struct mse {
    znode2_t base;
} mse_t;

typedef struct crossentropy {
    znode2_t base;
} crossentropy_t;

znode2_t* create_linear(const char* name);

znode2_t* create_relu(const char* name);

znode2_t* create_softmax(const char* name);

znode2_t* create_mse(const char* name);

znode2_t* create_crossentropy(const char* name);


#define __NN_2_ZNODE_H__

#endif