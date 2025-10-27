#include <stdlib.h>
#include <stdio.h>
#include <float.h>
#include <math.h>
#include "matrix2/matrix2.h"
#include "neural_network.h"

static znode_t* __znode_create(int in_dimens, int out_dimens, int id, int is_output) 
{
    znode_t* z = (znode_t*) malloc (sizeof(znode_t));
    z->id = id;
    z->in_dimens  = in_dimens;
    z->out_dimens = out_dimens;
    z->is_output  = is_output;
    z->z = Mat2_create(1,1);
    z->W = Mat2_create(out_dimens, in_dimens);
    z->b = Mat2_create(out_dimens, 1);
    z->x = Mat2_create(1,1);

    Mat2_fill_random(z->W, 0, 1);
    Mat2_fill_random(z->b, 0, 1);

    // printf("%d,\n", id);
    // MAT2_INSPECT(z->W);
    // MAT2_INSPECT(z->b);

    return z;
}

static int __fetch_train_datas(matrix2_t* dest_datas, matrix2_t* dest_labels, matrix2_t* src_datas, matrix2_t* src_labels, int batch)
{
    int total_cols = src_datas->cols;
    int random_index;

    for (int i=0; i<batch; ++i) {

        random_index = rand() % total_cols;

        Mat2_cpy_cols_to(dest_datas,  i, src_datas,  random_index);
        Mat2_cpy_cols_to(dest_labels, i, src_labels, random_index);

    }
    return 0;
}

static int __do_forward_propagate(nn_t* nn, matrix2_t* inputs, matrix2_t* outputs)
{

    matrix2_t* z;
    matrix2_t* W;
    matrix2_t* x;
    matrix2_t* ones  = Mat2_create(1,1);
    matrix2_t* bias  = Mat2_create(1,1);

    znode_t* first = znode_first(nn);
    znode_t* tail  = znode_tail(nn);
    
    Mat2_cpy(first->x, inputs);
    // 把 outputs 挂在 tail x 上，最后算出的结果，直接输出到 outputs 上, 那么在最后一次运算的时候可以不用额外的判断了。
    tail->x  = outputs;

    while (first != znode_tail(nn))
    {
        z = first->z;
        W = first->W;
        x = first->x;

        // W
        Mat2_cpy(z, W);
        // W dot x
        Mat2_dot(z, x);
        
        // printf("--------------- layer %d -----------------------\n\n", first->id+1);

        printf("\n W");
        MAT2_INSPECT(W);
        printf("\n xxxxx");
        MAT2_INSPECT(x);
        

        // bias ：
        Mat2_cpy(bias, first->b);
        
        Mat2_reshape(ones, 1, x->cols);
        Mat2_fill(ones, 1.f);
        Mat2_dot(bias, ones);

        // if (x->cols > 1) {
        //     // 若是 x 多于一条 input，那么 bias 则需要扩容。
        //     Mat2_reshape(ones, 1, x->cols);
        //     Mat2_fill(ones, 1.f);
        //     // 将 bias 扩展多个相同的列。
        //     Mat2_dot(bias, ones);
        // } 

        printf("\n bbbb");
        MAT2_INSPECT(bias);


        // W dot X + bias
        Mat2_add(z, bias);

        // printf("\n zzzz");
        // MAT2_INSPECT(z);

        // printf("--------------- layer %d -----------------------\n\n", first->id+1);

        Mat2_cpy(first->next->x, z);

        printf("\n z:");
        MAT2_INSPECT(z);

        // do the active to z, z became x of next znode
        // 若果
        if (first->is_output){
            nn->output(first->next->x);
        } else {
            nn->active(first->next->x);
        }
        
        // calculate the next node.
        first = first->next;
    }
    

    Mat2_destroy(ones);
    Mat2_destroy(bias);
    return 0;
}


static int __do_backward_propagate(nn_t* nn, matrix2_t* labels, matrix2_t* outputs)
{
    // 这里做想后传播，
    // 向后传播包括两步，1 计算梯度，2更新 W 和 bais。
    // 非常困难。明天再搞吧。为了做好 value function approximation ，重新写一遍这个 nn 破玩意。丢距个嘿。

    znode_t* last      = znode_last(nn);
    matrix2_t* delta_y = Mat2_create(1,1);
    matrix2_t* delta_W = Mat2_create(1,1);
    matrix2_t* delta_b = Mat2_create(1,1);
    matrix2_t* x_T     = Mat2_create(1,1);
    matrix2_t* W_T     = Mat2_create(1,1);
    matrix2_t* ones    = Mat2_create(1,1);
    matrix2_t* delta_z;

    Mat2_cpy(delta_y, outputs);

    nn->gradient_loss(delta_y, labels);
    
    while (last != znode_head(nn)) 
    {
        //printf("\n----------------- bp layer %d ----------------\n", last->id);
        // 计算本节点的
        delta_z = last->z;
        // 激活函数的导数
        if (last->is_output) {
            // 输出层使用输出函数，例如 softmax， 但是softmax + crossentropy 的导数在上一层已经做好了，所以此处可以直接，
            // 可以直接将 delta_z 变成1，再圆叉进去，即可，1 圆叉到 delta_y 还是 delta_y 正常是没有任何动作，
            nn->gradient_output(delta_z);
        } else {
            // 非输出层，按照正常流程来计算。
            nn->gradient_active(delta_z);
            // y` 圆乘 z`    
        }

        // delta_y 圆叉 delta_active_function
        Mat2_hadamard_product(delta_y, delta_z);

        // 计算 delta_W
        Mat2_cpy(delta_W, delta_y);
        Mat2_cpy(x_T, last->x);
        Mat2_T(x_T);
        Mat2_dot(delta_W, x_T);

        // 计算 delta b
        Mat2_cpy(delta_b, delta_y);

        Mat2_reshape(ones, delta_b->cols, 1);
        Mat2_dot(delta_b, ones);

        
        // 计算 delta x，本层的 delta x 就是下层的 delta y
        Mat2_cpy(W_T, last->W);
        Mat2_T(W_T);


        Mat2_dot(W_T, delta_y);
        // 将结果返回给 delta_y,作为下一个的上级导数。
        Mat2_cpy(delta_y, W_T);

        // 更新 W 和 b
        Mat2_scalar_multiply(delta_W, nn->alpha);
        Mat2_scalar_multiply(delta_b, nn->alpha);


        Mat2_sub(last->W, delta_W);
        Mat2_sub(last->b, delta_b);


        last = last->prev;
    }

    Mat2_destroy(delta_y);
    Mat2_destroy(delta_W);
    Mat2_destroy(delta_b);
    Mat2_destroy(x_T);
    Mat2_destroy(W_T);
    Mat2_destroy(ones);
    
    return 0;

}

int nn_build(nn_t* nn, \
    int input_dimens, int output_dimens, int batch, int max_iter, int err_stable, float alpha, float epsilon, \
    int layers, int neurals[], \
    int (*active)(matrix2_t*), int (*gradient_active)(matrix2_t*), \
    int (*output)(matrix2_t*), int (*gradient_output)(matrix2_t*), \
    float (*loss)(matrix2_t*, matrix2_t*), int (*gradient_loss)(matrix2_t*, matrix2_t*)
)
{
    nn->znode_head.next = znode_tail(nn);
    nn->znode_head.prev = znode_head(nn);

    int in_dimens = input_dimens;
    int out_dimens;
    int i;
    znode_t* z;
    znode_t* tail;
    for (i=0; i<layers; ++i) {

        out_dimens = neurals[i];

        z = __znode_create(in_dimens, out_dimens, i, 0);
        // 将新建的插入双向链表的后面, tail 的前面
        tail = znode_tail(nn);

        z->prev = tail->prev;
        z->next = tail;

        tail->prev->next = z;
        tail->prev       = z;

        in_dimens = out_dimens;
    }

    out_dimens = output_dimens;
    
    z = __znode_create(in_dimens, out_dimens, i, 1);

    tail = znode_tail(nn);

    z->prev = tail->prev;
    z->next = tail;

    tail->prev->next = z;
    tail->prev       = z;
    
    // 训练时的 特性。
    nn->max_iter      = max_iter;
    nn->epsilon       = epsilon;
    nn->alpha         = alpha;
    nn->batch         = batch;
    nn->err_stable    = err_stable;

    // 注册各种所需要的函数。
    nn->active           = active;
    nn->gradient_active  = gradient_active;

    nn->output           = output;
    nn->gradient_output  = gradient_output;

    nn->loss             = loss;
    nn->gradient_loss    = gradient_loss;

    return 0;
}

int nn_reset(nn_t* nn)
{
    znode_t* first = znode_first(nn);
    znode_t* del;

    while (first != znode_tail(nn)) 
    {
        // 一个个拔出来然后销毁。
        first->prev->next = first->next;
        first->next->prev = first->prev;
        
        del = first;
        first = first->next;

        // 是否内存。
        Mat2_destroy(del->W);
        Mat2_destroy(del->b);
        Mat2_destroy(del->z);
        Mat2_destroy(del->x);

        free(del);
    }

    nn->max_iter = 0;
    nn->epsilon  = FLT_MAX;

    nn->active = NULL;
    nn->gradient_active = NULL;

    nn->output = NULL;
    nn->gradient_output = NULL;

    nn->loss = NULL;
    nn->gradient_loss = NULL;

    return 0;
}

int nn_feed(nn_t* nn, matrix2_t* train_datas, matrix2_t* labels)
{   
    // 检查输入的数据维度是否准确。
    znode_t* first = znode_first(nn);
    znode_t* last  = znode_last(nn);

    if (first->W->cols == train_datas->rows && last->W->rows == labels->rows) {
        nn->trains_datas = train_datas;
        nn->labels       = labels;
        return 0;
    } 

    // 输入与输出规格不一致
    return -1;
}

int nn_fit(nn_t* nn, void (*progress)(const char* log_str, int step,  int stable, float err ))
{
    // 第一步，申请 batch 多个列的数据。

    float curr_error     = FLT_MAX;
    float last_error     = FLT_MAX;
    float error          = FLT_MAX;
    int   step           = 0;
    int   stable_state   = 0;

    matrix2_t* m_inputs  = Mat2_create(nn->trains_datas->rows, nn->batch);
    matrix2_t* m_outputs = Mat2_create(1,1);
    matrix2_t* m_labels  = Mat2_create(nn->labels->rows, nn->batch);

    while ( stable_state <= nn->err_stable && step++ < nn->max_iter ) {

        __fetch_train_datas(m_inputs, m_labels, nn->trains_datas, nn->labels, nn->batch);

        __do_forward_propagate(nn, m_inputs, m_outputs);
        
        curr_error = nn->loss(m_labels, m_outputs);

        // 计算每次训练后得到的 error 数值是否趋于稳定，当 error 的值趋于稳定，那么证明训练已经完成。
        error = sqrtf( (curr_error - last_error) * (curr_error - last_error));
        last_error = curr_error;

        if (error < nn->epsilon) {
            stable_state++;
        } else if (stable_state > 0) {
            stable_state--;
        }

        if (progress)
            progress("fitting ... ", step, stable_state, error);

        __do_backward_propagate(nn, m_labels, m_outputs);
    }

    Mat2_destroy(m_inputs);
    Mat2_destroy(m_labels);
    return 0;
    
}

int nn_perdict(nn_t* nn, matrix2_t* Input, matrix2_t* perdict)
{
    return __do_forward_propagate(nn, Input, perdict);
}

/**
 * @brief 复制一个一摸一样的 神经网络
 * 
 * @param nn 
 * @return int
 */
int nn_cpy(nn_t* dest, nn_t* src)
{
    // znode_t znode_head;
    // 训练时的结束条件。
    dest->batch      = src->batch;
    dest->max_iter   = src->max_iter;
    dest->epsilon    = src->epsilon;
    dest->alpha      = src->alpha;
    dest->err_stable = src->err_stable;

    dest->trains_datas = NULL;
    dest->labels       = NULL;

    dest->active = src->active;
    dest->gradient_active = src->gradient_active;

    dest->output = src->output;
    dest->gradient_output = src->gradient_output;

    dest->loss = src->loss;
    dest->gradient_loss = src->gradient_loss;

    dest->znode_head.next = &dest->znode_head;
    dest->znode_head.prev = &dest->znode_head;

    znode_t* src_first = znode_first(src);
    while (src_first != znode_tail(src)) {
        
        
        // create and cpy the weight;
        znode_t* z = __znode_create(src_first->in_dimens, src_first->out_dimens, src_first->id, src_first->is_output);

        Mat2_cpy(z->z, src_first->z);
        Mat2_cpy(z->W, src_first->W);
        Mat2_cpy(z->x, src_first->x);
        Mat2_cpy(z->b, src_first->b);

        znode_t* dest_tail = znode_tail(dest);

        z->prev = dest_tail->prev;
        z->next = dest_tail;

        dest_tail->prev->next = z;
        dest_tail->prev       = z;

        src_first = src_first->next;

    }
    return 0;
}

/**
 * @brief 将 src 神经网络的参数复制到 dest 的神经网络。
 * 
 * @param dest 
 * @param src 
 * @return int 
 */
int nn_cpy_weight(nn_t* dest, nn_t* src)
{
    znode_t* dest_first = znode_first(dest);
    znode_t* src_first  = znode_first(src);

    while (src_first != znode_tail(src)) {

        Mat2_cpy(dest_first->W, src_first->W);
        Mat2_cpy(dest_first->b, src_first->b);

        src_first  = src_first->next;
        dest_first = dest_first->next;
    }

    return 0;
}