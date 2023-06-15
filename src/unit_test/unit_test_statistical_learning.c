/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-03-31 13:28:12
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-06-15 14:10:41
 * @FilePath: /boring-code/src/unit_test/unit_test_statistical_learning.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>
#include <CUnit/Basic.h>
#include "statistical_learning/matrix2.h"
#include "statistical_learning/perceptron.h"
#include "statistical_learning/knn.h"

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
        {3, 3},
        {4, 3},
        {1, 1}
    };

    vfloat_t training_label[3] = {
        1, 1, -1
    };

    vfloat_t predict_x1[2] = {
        5, 5
    };
    vfloat_t predict;
    matrix2_t* training_mat = Mat2_create(1,1);
    Mat2_load_on_shape(training_mat, training_data, 3, 2);

    matrix2_t* training_label_mat = Mat2_create(1,1);
    Mat2_load_on_shape(training_label_mat, training_label, 3, 1);
    
    matrix2_t* _W_b;
    pct_train(training_mat, training_label_mat, &_W_b, 1.f, 1000000);
    
    MAT2_INSPACT(_W_b);

    matrix2_t* _X_mat = Mat2_create(1,1);
    Mat2_load_on_shape(_X_mat, predict_x1, 1, 2);
    
    pct_predict(_X_mat, _W_b, &predict);

    printf("predict: %f\n", predict);

    Mat2_destroy(_W_b);
    Mat2_destroy(_X_mat);
    Mat2_destroy(training_mat);
    Mat2_destroy(training_label_mat);
}

static void test_knn_predict(void) 
{
    // TODO : 测试 knn tree 的预测能力。
    // TODO 1: 建立数据。
    // ../src/unit_test/mnist/mnist_train.csv
    const char* train_csv_file = "../src/unit_test/mnist/mnist_train.csv";
    const char* test_csv_file  = "../src/unit_test/mnist/mnist_test.csv";

    matrix2_t* csv_mat = Mat2_create(1,1);
    matrix2_t* train_mat = Mat2_create(1,1);
    matrix2_t* train_label_mat = Mat2_create(1,1);
    matrix2_t* test_mat = Mat2_create(1,1);
    matrix2_t* test_label_mat = Mat2_create(1,1);
    matrix2_t* _input = Mat2_create(1,1);

    Mat2_load_csv(csv_mat, train_csv_file);
    
    Mat2_slice_col_to(train_label_mat, csv_mat, 0);
    Mat2_slice_cols_to(train_mat, csv_mat, 1, csv_mat->cols);
    
    Mat2_load_csv(csv_mat, test_csv_file);
    
    Mat2_slice_col_to(test_label_mat, csv_mat, 0);
    Mat2_slice_cols_to(test_mat, csv_mat, 1, csv_mat->cols);

    int test_count = 300;
    int predict;
    int correct = 0;
    char buff[1024];
    printf("\n\n calculating ... \n\n");
    for (int i=0; i<test_count; ++i) {
        Mat2_slice_row_to(_input, test_mat, i);
        //MAT2_INSPACT(_input);
        knn_predict(_input, train_mat, train_label_mat, 5, &predict);

        if (predict == (int) test_label_mat->pool[i]) 
            correct++;
        
        sprintf(buff, "Process: %d / %d", i, test_count);
        printf("%s\r", buff);
        fflush(stdout);
    }

    printf("Knn predict correct: %.3f \n", (float)correct / (float) test_count );

    Mat2_destroy(csv_mat);
    Mat2_destroy(train_mat);
    Mat2_destroy(train_label_mat);
    Mat2_destroy(test_mat);
    Mat2_destroy(test_label_mat);
    Mat2_destroy(_input);

}

int do_statistical_learning_test (void) 
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("Test machine learning 2 Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }

    // if (NULL == CU_add_test(pSuite, "test knn predict", test_knn_predict) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    if (NULL == CU_add_test(pSuite, "test perceptron test", test_perceptron) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

}

