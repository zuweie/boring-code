/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2025-10-17 12:36:24
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-11-03 12:20:48
 * @FilePath: /boring-code/src/reinforce_learning/neural_network.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __NEURAL_NETWORK_H__
#define __NEURAL_NETWORK_H__

typedef struct matrix2 matrix2_t;

typedef struct znode {

    int id;
    int in_dimens;
    int out_dimens;
    int is_output;
    matrix2_t* z;
    matrix2_t* W;
    matrix2_t* b;
    matrix2_t* x;
    struct znode* next;
    struct znode* prev;

} znode_t;

typedef struct nn {

    int     batch;
    int     max_iter;
    float   epsilon;
    int     err_stable;

    // 学习曲率
    float   alpha;

    matrix2_t* trains_datas;
    matrix2_t* labels;

    znode_t znode_head;


    int (*active)(matrix2_t*);
    int (*gradient_active)(matrix2_t*);
    int (*output)(matrix2_t*);
    int (*gradient_output)(matrix2_t*);
    float (*loss)(matrix2_t*, matrix2_t*);
    int (*gradient_loss)(matrix2_t*, matrix2_t*);
    
} nn_t;

static inline znode_t* znode_first(nn_t* nn) 
{
    return nn->znode_head.next;
}

static inline znode_t* znode_last(nn_t* nn) 
{
    return nn->znode_head.prev;
}

static inline znode_t* znode_head(nn_t* nn) {
    return &nn->znode_head;
}

static inline znode_t* znode_tail(nn_t* nn) {
    return &nn->znode_head;
}

int nn_build(nn_t* nn, \
    int input_dimens, int output_dimens, int batch, int max_iter, int err_stable, float alpha, float epsilon, \
    int layers, int neruals[], \
    int (*active)(matrix2_t*), int (*gradient_active)(matrix2_t*), \
    int (*output)(matrix2_t*), int (*gradient_output)(matrix2_t*), \
    float (*loss)(matrix2_t*, matrix2_t*), int (*gradient_loss)(matrix2_t*, matrix2_t*)
);

int nn_feed(nn_t *nn, matrix2_t* train_dates, matrix2_t* labels);
int nn_fit(nn_t* nn, void (*progress)(const char* log_str, int step, int stable, float err));
int nn_reset(nn_t* nn);
int nn_predict(nn_t*nn, matrix2_t* Input, matrix2_t* predict);
int nn_cpy(nn_t* dest, nn_t* src);
int nn_cpy_weight(nn_t* dest, nn_t* src);
int nn_show_weights(nn_t* nn);

#endif