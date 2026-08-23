/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-08-23 09:19:58
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-08-23 11:05:05
 * @FilePath: /boring-code/src/deep_learning/dnn/dnn_optimizer.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#ifndef __DNN_OPTIMIZER_H__
#define __DNN_OPTIMIZER_H__

typedef struct dnn dnn_t;

typedef struct dnn_optimizer {

    dnn_t* model;
    int    batch;
    int    max_iter;
    int    err_stable;

    float  epsilon;
    float  alpha;
    
    cg_tensor_t* lables;
    cg_tensor_t* train_datas;

    // prepaer train branch
    int(*prepare)(dnn_optimizer_t* thiz);
    // end train loop indication
    int(*term)   (dnn_optimizer_t* thiz);
    // processing
    int(*processing) (dnn_optimizer_t* thiz);
    // set
    int(*step)(dnn_optimizer_t* thiz);
} dnn_optimizer_t;

static inline int dnn_optimizer_init( 
    dnn_optimizer_t* optimizer, 
    dnn_t* model, 
    int batch, 
    int max_iter, 
    int err_stable, 
    float epsilon, 
    float alpha, 
    void* lables,
    void* train_datas,
    int (*prepare)(dnn_optimizer_t*),
    int (*term)(dnn_optimizer_t*),
    int (*processing)(dnn_optimizer_t*),
    int (*step)(dnn_optimizer_t*)
) {
    *optimizer = (dnn_optimizer_t) {
        .model       = model,
        .batch       = batch,
        .max_iter    = max_iter,
        .err_stable  = err_stable,
        .epsilon     = epsilon,
        .lables      = lables,
        .train_datas = train_datas,
        .prepare     = prepare,
        .term        = term,
        .processing  = processing,
        .step        = step
    };
    return 0;
}
#endif