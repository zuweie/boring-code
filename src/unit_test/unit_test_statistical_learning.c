/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-03-31 13:28:12
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-06-13 09:30:53
 * @FilePath: /boring-code/src/unit_test/unit_test_statistical_learning.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>
#include <CUnit/Basic.h>
#include "statistical_learning/matrix2.h"
#include "statistical_learning/perceptron.h"

#define PRINTF_DOUBLES(x) printf("%lf ", (x));

static int  suite_success_init (void) 
{
    printf("\n machine learning2 suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("\n machine learning 2 suite success clean\n");
}



static void test_perceptron(void) 
{

    vfloat_t training_data[][3] = {
        {3, 3, 1},
        {4, 3, 1},
        {1, 1, -1}
    };

    vfloat_t perdict_x1[2] = {
        5, 5
    };
    vfloat_t perdict;
    matrix2_t* training_mat = Mat2_create(1,1);
    Mat2_load_on_shape(training_mat, training_data, 3, 3);
    matrix2_t* _W_b;
    pct_train(training_mat, &_W_b, 1.f, 1000000);
    
    matrix2_t* _X_mat = Mat2_create(1,1);
    Mat2_load_on_shape(_X_mat, perdict_x1, 1, 2);
    
    pct_perdict(_X_mat, _W_b, &perdict);

    printf("perdict: %f\n", perdict);

    Mat2_destroy(_W_b);
    Mat2_destroy(_X_mat);
}

static void test_knn_tree(void)
{
    // TODO : 测试 KD tree 的建立。
}

static void test_knn_perdict(void) 
{
    // TODO : 测试 knn tree 的预测能力。
}

int do_statistical_learning_test (void) 
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("Test machine learning 2 Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test dots_params", test_knn_tree) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test dots_params", test_knn_perdict) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

}

