/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-11-26 08:07:16
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-11-27 15:17:34
 * @FilePath: /boring-code/src/reinforce_learning/nn2.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __NN_2_H__
#define __NN_2_H__
#include "nn2_znode.h"

typedef struct nn2 {

    int      znode_counter;
    znode2_t znode_head;

} nn2_t;

static inline znode2_t* layer_first(nn2_t* nn) 
{
    return nn->znode_head.next;
}

static inline znode_t* layer_last(nn2_t* nn) 
{
    return nn->znode_head.prev;
}

static inline znode_t* layer_head(nn2_t* nn) {
    return &nn->znode_head;
}

static inline znode_t* layer_tail(nn2_t* nn) {
    return &nn->znode_head;
}

static inline int layer_insert(znode2_t* pos, znode2_t* z)
{
    z->prev = pos->prev;
    z->next = pos;

    pos->prev->next = z;
    pos->prev       = z;
    return 0;
}

static inline int layer_remove(znode_t* z) 
{
    z->prev->next = z->next;
    z->next->prev = z->prev;
    return 0;
}

static inline int znode

int nn2_init(nn2_t*);
int nn2_reset(nn2_t*);
int nn2_linear(nn2_t*, int in_dimens, int out_dimens);
int nn2_relu(nn2_t*);
int nn2_softmax(nn2_t*);
int nn2_mse(nn2_t*);
int nn2_crossentropy(nn2_t*);
int nn2_forward(nn2_t*, matrix2_t);
int nn2_backward(nn2_t*, matrix2_t);
int nn2_fit(nn2_t*);



#endif