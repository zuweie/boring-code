/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-08-16 22:01:02
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-08-30 14:08:44
 * @FilePath: /boring-code/src/unit_test/unit_test_dl_cg2_nn.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "unit_test.h"

#include "deep_learning/compute_graph2/cg_tensor.h"
#include "deep_learning/dnn/dnn.h"
#include "deep_learning/dnn/dnn_optimizer.h"
#include "deep_learning/compute_graph2/cg_debug.h"


static int  suite_success_init (void) 
{
    printf("\ndl_cg2_nn suite success init\n");
    return 0;
}

static int suite_success_clean (void) 
{   
    printf("\ndl_cg2_nn suite success clean\n");
    return 0;
}

static int __dnn_prepare(dnn_optimizer_t* thiz)
{
    CG_DEBUG("dnn prepare: update batch training data\n");
    return 0;
}

static int __dnn_term(dnn_optimizer_t* thiz)
{
    return thiz->iter > thiz->max_iter;
}

static int __dnn_processing(dnn_optimizer_t* thiz)
{
    thiz->iter++;
    CG_DEBUG("dnn processing: %d round trainning\n", thiz->iter);
    return 0;
}

static int __dnn_step(dnn_optimizer_t* thiz)
{
    CG_DEBUG("dnn step, update nn`s weigth\n");
    return 0;
}

static void test_dnn (void) 
{
    printf(" \n build a simple neural network with computer graph 2\n");
    
    cg_tensor_elem_type train_datas[] = {};
    cg_tensor_elem_type labels[]      = {};
    cg_tensor_elem_type feature[]     = {};
    
    dnn_t simple_nn;
    dnn_optimizer_t optimizer;
    dnn_init(&simple_nn);

    cg_tensor_t* tensor_train_datas = cg_tensor_create(&simple_nn.alloc, train_datas, 2, 4, 60);
    cg_tensor_t* tensor_labels      = cg_tensor_create(&simple_nn.alloc, labels, 2, 3, 60);
    cg_tensor_t* tensor_feature     = cg_tensor_create(&simple_nn.alloc, feature, 2, 4, 1);
    
    dnn_optimizer_init(
        &optimizer,
        &simple_nn,
        20, 
        1000,
        3,
        0.001f,
        0.05,
        tensor_labels,
        tensor_train_datas,
        __dnn_prepare,
        __dnn_term,
        __dnn_processing,
        __dnn_step
    );

    // build the neural network 4 * 5 * 3 的神经网络。
    dnn_linear(&simple_nn, 4, 5);
    dnn_relu(&simple_nn);
    dnn_linear(&simple_nn, 5, 3);
    dnn_softcrx(&simple_nn);
    
    // training
    dnn_fit(&optimizer);
    
    // 推理
    cg_tensor_t* predict = dnn_predict(&simple_nn, tensor_feature);

    cg_tensor_inspect(predict);

    //clean up
    cg_tensor_recycle(tensor_train_datas);
    cg_tensor_recycle(tensor_labels);
    cg_tensor_recycle(tensor_feature);
    
    dnn_reset(&simple_nn);
    
    return;
}



int do_dl_cg2_nn_test(void)
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("The dl cg2 nn Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test dnn", test_dnn) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    return 0;
}