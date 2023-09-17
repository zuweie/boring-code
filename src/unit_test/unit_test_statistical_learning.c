/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2023-03-31 13:28:12
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2023-09-17 13:22:16
 * @FilePath: /boring-code/src/unit_test/unit_test_statistical_learning.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "statistical_learning/matrix2.h"
#include "statistical_learning/perceptron.h"
#include "statistical_learning/knn.h"
#include "statistical_learning/naive_bayes.h"
#include "statistical_learning/decision_tree.h"
#include "statistical_learning/adaboost.h"
#include "statistical_learning/counting.h"
#include "statistical_learning/svm.h"

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

    vfloat_t training_data[][2] = {
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
    
    CU_ASSERT_DOUBLE_EQUAL(_W_b->pool[0], 1.f, 0.0001f);
    CU_ASSERT_DOUBLE_EQUAL(_W_b->pool[1], 1.f, 0.0001f);
    CU_ASSERT_DOUBLE_EQUAL(_W_b->pool[2], -3.f, 0.0001f);

    //MAT2_INSPECT(_W_b);

    matrix2_t* _X_mat = Mat2_create(1,1);
    Mat2_load_on_shape(_X_mat, predict_x1, 1, 2);
    
    pct_predict(_X_mat, _W_b, &predict);

    /// printf("predict: %f\n", predict);

    CU_ASSERT_DOUBLE_EQUAL(predict, 1.f, 0.0000f);

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
        //MAT2_INSPECT(_input);
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

static void test_navie_bayes (void) 
{
    const char* train_csv_file = "/Users/zuweie/code/c-projects/boring-code/build/../src/unit_test/mnist/mnist_train.csv";
    const char* test_csv_file  = "/Users/zuweie/code/c-projects/boring-code/build/../src/unit_test/mnist/mnist_test.csv";

    matrix2_t* csv_mat    = Mat2_create(1,1);
    matrix2_t* train_mat  = Mat2_create(1,1);
    matrix2_t* train_label_mat  = Mat2_create(1,1);
    matrix2_t* test_label_mat   = Mat2_create(1,1);
    matrix2_t* test_mat = Mat2_create(1,1);
    matrix2_t* _X       = Mat2_create(1,1);

    Mat2_load_csv(csv_mat, train_csv_file);

    Mat2_slice_col_to(train_label_mat, csv_mat, 0);
    Mat2_slice_cols_to(train_mat, csv_mat, 1, csv_mat->cols);

    MAT2_POOL_PTR(train_mat, train_mat_ptr);

    // 先将数据进行简单处理一下。
    for (int i=0; i<train_mat->rows; ++i) {
        for (int j=0; j<train_mat->cols; ++j) {
            // if (train_mat_ptr[i][j] < 63) {
            //     train_mat_ptr[i][j] = 0;
            // } else if (train_mat_ptr[i][j] > 63 && train_mat_ptr[i][j] < 127) {
            //     train_mat_ptr[i][j] = 1;
            // } else if (train_mat_ptr[i][j] > 127 && train_mat_ptr[i][j] < 192) {
            //     train_mat_ptr[i][j] = 2;
            // } else {
            //     train_mat_ptr[i][j] = 3;
            // }

            if (train_mat_ptr[i][j] < 127) {
                train_mat_ptr[i][j] = 0;
            } else {
                train_mat_ptr[i][j] = 1;
            }
            
        }
    }

    Mat2_load_csv(csv_mat, test_csv_file);
    Mat2_slice_col_to(test_label_mat, csv_mat, 0);
    Mat2_slice_cols_to(test_mat, csv_mat, 1, csv_mat->cols);
    
    MAT2_POOL_PTR(test_mat, test_mat_ptr);

    //MAT2_INSPECT(test_label_mat);
    // 先将数据进行简单处理一下。
    for (int i=0; i<test_mat->rows; ++i) {
        for (int j=0; j<test_mat->cols; ++j) {
            // if (test_mat_ptr[i][j] < 63) {
            //     test_mat_ptr[i][j] = 0;
            // } else if (test_mat_ptr[i][j] > 63 && test_mat_ptr[i][j] < 127) {
            //     test_mat_ptr[i][j] = 1;
            // } else if (test_mat_ptr[i][j] > 127 && test_mat_ptr[i][j] < 192) {
            //     test_mat_ptr[i][j] = 2;
            // } else {
            //     test_mat_ptr[i][j] = 3;
            // }

            if (test_mat_ptr[i][j] < 127) {
                test_mat_ptr[i][j] = 0;
            } else {
                test_mat_ptr[i][j] = 1;
            }
        }
    }
    
    

    void* Py_counting;
    void* Pxy_counting_table;
    navie_bayes_counting(train_mat, train_label_mat, &Py_counting, &Pxy_counting_table);

    int correct = 0;
    char buff[1024];

    memset(buff, 0x0, sizeof(buff));
    int label;
    for (int i=0; i<test_mat->rows; ++i) {

        label = (int)test_label_mat->pool[i];

        Mat2_slice_row_to(_X, test_mat, i);

        //MAT2_INSPECT(_X);

        float predict;

        navie_bayes_predict(_X, Py_counting, Pxy_counting_table, 1, &predict);

        if ((int)predict == label) correct++; 

        sprintf(buff, "Process: %d / %d, predict: %d - %d, correct %.2f ... ", i+1, test_mat->rows, (int)predict, label, ((float) (correct) / (float) (test_mat->rows))*100);
        
        printf("%s\r", buff);

        fflush(stdout);

    }

    //printf("\n correct %.2f \n", (float) (correct) / (float) (test_mat->rows));

    double correctly = (float) (correct) / (float) (test_mat->rows);

    CU_ASSERT( (correctly * 100) > 84.f );
    

    free(Py_counting);
    counting_free_XY_table(Pxy_counting_table);

    Mat2_destroy(csv_mat);
    Mat2_destroy(train_mat);
    Mat2_destroy(train_label_mat);
    Mat2_destroy(test_label_mat); 
    Mat2_destroy(test_mat);
    Mat2_destroy(_X );
}

static void test_navie_bayes2(void) {

    matrix2_t* train_mat       = Mat2_create(1,1);
    matrix2_t* train_label_mat = Mat2_create(1,1);
    matrix2_t* test_mat        = Mat2_create(1,1);


    float train[][2] ={
        {1, 'S'},
        {1, 'M'},
        {1, 'M'},
        {1, 'S'},
        {1, 'S'},
        {2, 'S'},
        {2, 'M'},
        {2, 'M'},
        {2, 'L'},
        {2, 'L'},
        {3, 'L'},
        {3, 'M'},
        {3, 'M'},
        {3, 'L'},
        {3, 'L'}
    };

    float train_label[] = {
        -1, -1, 1, 1, -1, -1, -1, 1, 1, 1, 1, 1, 1, 1, -1
    };

    float test[][2] = {
        {2, 'S'}
    };

    Mat2_load_on_shape(train_mat, train, 15, 2);
    Mat2_load_on_shape(train_label_mat, train_label, 15, 1);
    Mat2_load_on_shape(test_mat, test, 1, 2);

    void* Py_counting;
    void* Pxy_counting_table;
    navie_bayes_counting(train_mat, train_label_mat, &Py_counting, &Pxy_counting_table);

    float predict;
    navie_bayes_predict(test_mat, Py_counting, Pxy_counting_table, 1, &predict);

    //printf(" predict: %0.2f \n", predict);
    CU_ASSERT_DOUBLE_EQUAL(predict, -1, 1e-5);
    //navie_bayes_release_counting(Py_counting, Pxy_counting_table);
    free(Py_counting);
    //navie_bayes_release_pxy_counting_table(Pxy_counting_table);
    counting_free_XY_table(Pxy_counting_table);

    Mat2_destroy(train_mat);
    Mat2_destroy(train_label_mat);
    Mat2_destroy(test_mat);

}

static void test_navies_bayes_mgd(void) {

    vfloat_t train_data[][3] = {
        {6, 180, 12},
        {5.92, 190, 11},
        {5.58, 170, 12},
        {5.92, 165, 10},
        {5, 100, 6},
        {5.5, 150, 8},
        {5.42, 130, 7},
        {5.75, 150, 9}
    };

    vfloat_t train_label[] = {
        1,1,1,1,2,2,2,2
    };

    vfloat_t test_data[] = {
        6, 130, 8
    };
    
    matrix2_t* train_mat = Mat2_create(1,1);
    matrix2_t* train_label_mat = Mat2_create(1,1);
    matrix2_t* _X = Mat2_create(1,1);

    Mat2_load_on_shape(train_mat, train_data, 8, 3);
    Mat2_load_on_shape(train_label_mat, train_label, 8, 1);
    Mat2_load_on_shape(_X, test_data, 1, 3);

    void* mus_table;
    void* sigma_table;
    void* Py_counting;
    navie_bayes_train_MGD_edit(train_mat, train_label_mat, &Py_counting, &mus_table, &sigma_table, NULL);

    vfloat_t predict;

    navie_bayes_predict_MGD_edit(_X, Py_counting, mus_table, sigma_table, &predict);

    CU_ASSERT_DOUBLE_EQUAL(predict, 1.f, 1e-5);

    free(Py_counting);
    Mat2_destroy(train_mat);
    Mat2_destroy(train_label_mat);
    Mat2_destroy(_X);
}


static void test_navies_bayes_mgd2(void) 
{
    vfloat_t train_data[][3] = {
        {6, 180, 12},
        {5.92, 190, 11},
        {5.58, 170, 12},
        {5.92, 165, 10},
        {5, 100, 6},
        {5.5, 150, 8},
        {5.42, 130, 7},
        {5.75, 150, 9}
    };

    vfloat_t train_label[] = {
        1,1,1,1,2,2,2,2
    };

    vfloat_t test_data[] = {
        6, 130, 8
    };
    
    matrix2_t* train_mat = Mat2_create(1,1);
    matrix2_t* train_label_mat = Mat2_create(1,1);
    matrix2_t* _X = Mat2_create(1,1);

    Mat2_load_on_shape(train_mat, train_data, 8, 3);
    Mat2_load_on_shape(train_label_mat, train_label, 8, 1);
    Mat2_load_on_shape(_X, test_data, 1, 3);

    void* mus_table;
    void* sigma_table;
    void* Py_counting;
    navie_bayes_train_MGD_edit(train_mat, train_label_mat, &Py_counting, &mus_table, &sigma_table, NULL);

    vfloat_t predict;

    navie_bayes_predict_MGD2_edit(_X, Py_counting, mus_table, sigma_table, &predict);

    CU_ASSERT_DOUBLE_EQUAL(predict, 1.f, 1e-5);
    
    free(Py_counting);
    Mat2_destroy(train_mat);
    Mat2_destroy(train_label_mat);
    Mat2_destroy(_X);
}

static void navies_bayes_training_progress (char* title, unsigned long step, unsigned long total) {
    
    char buffer[1024];
    memset(buffer, 0x0, sizeof(buffer));
    sprintf(buffer, "%s ,进度: %ld / %ld, %0.2f ", title, step, total, ((double)step / (double)total) * 100);
    printf("%s\r", buffer);
    fflush(stdout);

}

static void test_navies_bayes_mgd_big(void) 
{
    const char* train_csv_file = "/Users/zuweie/code/c-projects/boring-code/build/../src/unit_test/mnist/mnist_train.csv";
    const char* test_csv_file  = "/Users/zuweie/code/c-projects/boring-code/build/../src/unit_test/mnist/mnist_test.csv";

    matrix2_t* csv_mat    = Mat2_create(1,1);
    matrix2_t* train_mat  = Mat2_create(1,1);
    matrix2_t* train_label_mat  = Mat2_create(1,1);
    matrix2_t* test_label_mat = Mat2_create(1,1);
    matrix2_t* test_mat = Mat2_create(1,1);
    matrix2_t* _X       = Mat2_create(1,1);


    Mat2_load_csv(csv_mat, train_csv_file);
    Mat2_slice_col_to(train_label_mat, csv_mat, 0);
    Mat2_slice_cols_to(train_mat, csv_mat, 1, csv_mat->cols);

    Mat2_load_csv(csv_mat, test_csv_file);
    Mat2_slice_col_to(test_label_mat, csv_mat, 0);
    Mat2_slice_cols_to(test_mat, csv_mat, 1, csv_mat->cols);

    
    void* Py_counting;
    void* mus_table;
    void* sigma_table;
    printf(" \n ---- *** training .... *** ---- \n");
    navie_bayes_train_MGD_edit(train_mat, train_label_mat, &Py_counting, &mus_table, &sigma_table, navies_bayes_training_progress);

    int correct = 0;
    char buff[1024];
    memset(buff, 0x0, sizeof(buff));
    int label;

    printf(" \n ---- *** predicting ... *** ---- \n");

    for (int i=0; i<test_mat->rows; ++i) {

        label = (int)test_label_mat->pool[i];

        Mat2_slice_row_to(_X, test_mat, i);

        float predict;

        //navie_bayes_predict(_X, Py_counting, Pxy_counting_table, 1, &predict);
        navie_bayes_predict_MGD_edit(_X, Py_counting, mus_table, sigma_table, &predict);

        if ((int)predict == label) correct++; 

        sprintf(buff, "Process: %d / %d, predict: %d - %d, correct %.2f ... ", i+1, test_mat->rows, (int)predict, label, ((float) (correct) / (float) (test_mat->rows))*100);
        
        printf("%s\r", buff);

        fflush(stdout);

    }

    printf("\n correct %.2f \n", (float) (correct) / (float) (test_mat->rows));

    free(Py_counting);
    free(mus_table);
    free(sigma_table);

    Mat2_destroy(csv_mat);
    Mat2_destroy(train_mat);
    Mat2_destroy(train_label_mat);
    Mat2_destroy(test_label_mat); 
    Mat2_destroy(test_mat);
    Mat2_destroy(_X );
    return;
}

static void decision_tree_training_progress (char* title, unsigned long step, unsigned long total) 
{
    char buffer[1024];
    memset(buffer, 0x0, sizeof(buffer));
    //sprintf(buffer, "%s ,进度: %ld / %ld, %0.2f ", title, step, total, ((double)step / (double)total) * 100);
    if(total==0)
        sprintf(buffer, "%s, 进度: %ld ", title, step);
    else 
        sprintf(buffer, "%s, 剩余节点: %ld, 剩余数据：%ld ", title,  step, total);

    printf("%s\n", buffer);
    //fflush(stdout);
}

static void test_decision_tree_large (void) 
{
    const char* train_csv_file = "/Users/zuweie/code/c-projects/boring-code/build/../src/unit_test/mnist/mnist_train.csv";
    const char* test_csv_file  = "/Users/zuweie/code/c-projects/boring-code/build/../src/unit_test/mnist/mnist_test.csv";

    matrix2_t* csv_mat    = Mat2_create(1,1);
    matrix2_t* train_mat  = Mat2_create(1,1);
    matrix2_t* train_label_mat  = Mat2_create(1,1);
    matrix2_t* test_label_mat = Mat2_create(1,1);
    matrix2_t* test_mat = Mat2_create(1,1);
    matrix2_t* _X       = Mat2_create(1,1);


    Mat2_load_csv(csv_mat, train_csv_file);
    Mat2_slice_col_to(train_label_mat, csv_mat, 0);
    Mat2_slice_cols_to(train_mat, csv_mat, 1, csv_mat->cols);

    Mat2_load_csv(csv_mat, test_csv_file);
    Mat2_slice_col_to(test_label_mat, csv_mat, 0);
    Mat2_slice_cols_to(test_mat, csv_mat, 1, csv_mat->cols);

    // 将数据进行简单处理一下, 把 0 到 255 的数统一改为 1 或者零。
    MAT2_POOL_PTR(train_mat, train_mat_ptr);
    for (int i=0; i<train_mat->rows; ++i) {
        for (int j=0; j<train_mat->cols; ++j) {

            // if (train_mat_ptr[i][j] < 63) {
            //     train_mat_ptr[i][j] = 0;
            // } else if (train_mat_ptr[i][j] > 63 && train_mat_ptr[i][j] < 127) {
            //     train_mat_ptr[i][j] = 1;
            // } else if (train_mat_ptr[i][j] > 127 && train_mat_ptr[i][j] < 192) {
            //     train_mat_ptr[i][j] = 2;
            // } else {
            //     train_mat_ptr[i][j] = 3;
            // }

            if (train_mat_ptr[i][j] < 127) {
                train_mat_ptr[i][j] = 0;
            } else {
                train_mat_ptr[i][j] = 1;
            }
            
        }
    }

    MAT2_POOL_PTR(test_mat, test_mat_ptr);

    //MAT2_INSPECT(test_label_mat);
    // 将数据进行简单处理一下, 把 0 到 255 的数统一改为 1 或者零。
    for (int i=0; i<test_mat->rows; ++i) {
        for (int j=0; j<test_mat->cols; ++j) {
            // if (test_mat_ptr[i][j] < 63) {
            //     test_mat_ptr[i][j] = 0;
            // } else if (test_mat_ptr[i][j] > 63 && test_mat_ptr[i][j] < 127) {
            //     test_mat_ptr[i][j] = 1;
            // } else if (test_mat_ptr[i][j] > 127 && test_mat_ptr[i][j] < 192) {
            //     test_mat_ptr[i][j] = 2;
            // } else {
            //     test_mat_ptr[i][j] = 3;
            // }

            if (test_mat_ptr[i][j] < 127) {
                test_mat_ptr[i][j] = 0;
            } else {
                test_mat_ptr[i][j] = 1;
            }
        }
    }


    printf(" \n ---- *** training .... *** ---- \n");
    //train_mat->rows = 20;
    //train_label_mat->rows = 20;
    dc_tree_t tree;
    dct_term_t term = {
        .gain_esp = 0.1,
        .least_limit = 1000,
        .level_limit = 15
    };
    decision_tree_classification_train(train_mat, train_label_mat, &tree, &term, decision_tree_training_progress);

    printf(" \n ---- *** predicting ... *** ---- \n");
    //MAT2_INSPECT(test_label_mat);
    int correct;
    int label;
    char buff[1024];
    memset(buff, 0x0, sizeof(buff));
    int test_number = test_label_mat->rows;
    for (int i=0; i<test_number; ++i) {

        label = (int)test_label_mat->pool[i];

        Mat2_slice_row_to(_X, test_mat, i);

        float predict;

        //navie_bayes_predict_MGD_edit(_X, Py_counting, mus_table, sigma_table, &predict);
        decision_tree_classification_predict(_X, &tree, &predict);

        if ((int)predict == label) correct++; 

        sprintf(buff, "Process: %d / %d, predict: %d - %d, correct %.2f ... ", i+1, test_number, (int)predict, label, ((float) (correct) / (float) (test_number))*100);
        
        printf("%s\r", buff);

        fflush(stdout);
    }
    
    Mat2_destroy(csv_mat ); 
    Mat2_destroy(train_mat);
    Mat2_destroy(train_label_mat);
    Mat2_destroy(test_label_mat); 
    Mat2_destroy(test_mat); 
    Mat2_destroy(_X);
    decision_tree_release(&tree);
    
}

static void test_decision_tree_simple (void) 
{
    
    vfloat_t train_data[][4] = {
        {'y', 'n', 'n', 1},
        {'y', 'n', 'n', 2},
        {'y', 'y', 'n', 2},
        {'y', 'y', 'y', 1},
        {'y', 'n', 'n', 1},
        {'m', 'n', 'n', 1}, 
        {'m', 'n', 'n', 2},
        {'m', 'y', 'y', 2},
        {'m', 'n', 'y', 3},
        {'m', 'n', 'y', 3},
        {'o', 'n', 'y', 3},
        {'o', 'n', 'y', 2},
        {'o', 'y', 'n', 2}, 
        {'o', 'y', 'n', 3},
        {'o', 'n', 'n', 1}
    }; 
    vfloat_t train_label[] = {
        0, 0, 1, 1, 0, 0, 0, 1, 1, 1, 1, 1, 1, 1, 0
    };

    vfloat_t test_data1[4] = {
        'o', 'y', 'y', 1
    };

    vfloat_t test_data2[4] = {
        'y', 'n', 'n', 1
    };

    matrix2_t* train_mat       = Mat2_create(1,1);
    matrix2_t* train_label_mat = Mat2_create(1,1);
    matrix2_t* test_mat        = Mat2_create(1,1);
    vfloat_t predict;

    Mat2_load_on_shape(train_mat, train_data, 15, 4);
    Mat2_load_on_shape(train_label_mat, train_label, 15, 1);
    Mat2_load_on_shape(test_mat, test_data1, 1, 4);

    dc_tree_t tree;
    dct_term_t term = {
        .gain_esp = 0.1,
        .least_limit = 1,
        .level_limit = 1e+20,
    };

    decision_tree_classification_train(train_mat, train_label_mat, &tree, &term, NULL);
    decision_tree_classification_predict(test_mat, &tree, &predict);
    printf("\n test data 1 predict: %lf, \n", predict);
    

    Mat2_load_on_shape(test_mat, test_data2, 1, 4);
    decision_tree_classification_predict(test_mat, &tree, &predict);
    printf(" test data 2 predict: %lf, \n", predict);

    Mat2_destroy(train_mat);
    Mat2_destroy(train_label_mat);
    Mat2_destroy(test_mat);
    decision_tree_release(&tree);
}

static void test_decision_tree_simple2(void) {
        vfloat_t train_data[][5] = {
        {'y', 'l', 'n', 'n', 'f'},
        {'y', 'l', 'y', 'n', 'g'},
        {'y', 'm', 'y', 'n', 'g'},
        {'y', 'm', 'y', 'y', 'g'},
        {'y', 'h', 'y', 'y', 'g'},
        {'y', 'm', 'n', 'y', 'g'}, 
        {'m', 'l', 'y', 'y', 'e'},
        {'m', 'h', 'y', 'y', 'g'},
        {'m', 'l', 'n', 'y', 'g'},
        {'m', 'm', 'y', 'y', 'f'},
        {'m', 'h', 'y', 'y', 'e'},
        {'m', 'm', 'n', 'n', 'g'},
        {'o', 'l', 'n', 'n', 'g'}, 
        {'o', 'l', 'y', 'y', 'e'},
        {'o', 'l', 'y', 'n', 'e'},
        {'o', 'm', 'n', 'y', 'g'},
        {'o', 'l', 'n', 'n', 'e'},
        {'o', 'h', 'n', 'y', 'f'},
        {'o', 'h', 'y', 'y', 'e'}
    }; 
    vfloat_t train_label[] = {
       'n','n','y','y','y','n','y','y','n','n','y','n','n','y','y','n','n','n','y'
    };

    vfloat_t test_data1[] = {
        'm','h','y','n','f'
    };

    matrix2_t* train_mat       = Mat2_create(1,1);
    matrix2_t* train_label_mat = Mat2_create(1,1);
    matrix2_t* test_mat        = Mat2_create(1,1);
    vfloat_t predict;
    
    Mat2_load_on_shape(train_mat, train_data, 19, 5);
    Mat2_load_on_shape(train_label_mat, train_label, 19, 1);
    Mat2_load_on_shape(test_mat, test_data1, 1, 5);


    dc_tree_t tree;
    dct_term_t term = {

        .gain_esp = 0.1,
        .least_limit = 1,
        .level_limit = 1e+10,
    };
    decision_tree_classification_train(train_mat, train_label_mat, &tree, &term, NULL);
    decision_tree_classification_predict(test_mat, &tree, &predict);
    printf("\n test data 1 predict: %c, \n", (char)predict);

    Mat2_destroy(train_mat);
    Mat2_destroy(train_label_mat);
    Mat2_destroy(test_mat);
    decision_tree_release(&tree);
}

static void test_adaboost_tree_simple(void) {
    vfloat_t train_data[][5] = {
        {'y', 'l', 'n', 'n', 'f'},
        {'y', 'l', 'y', 'n', 'g'},
        {'y', 'm', 'y', 'n', 'g'},
        {'y', 'm', 'y', 'y', 'g'},
        {'y', 'h', 'y', 'y', 'g'},
        {'y', 'm', 'n', 'y', 'g'}, 
        {'m', 'l', 'y', 'y', 'e'},
        {'m', 'h', 'y', 'y', 'g'},
        {'m', 'l', 'n', 'y', 'g'},
        {'m', 'm', 'y', 'y', 'f'},
        {'m', 'h', 'y', 'y', 'e'},
        {'m', 'm', 'n', 'n', 'g'},
        {'o', 'l', 'n', 'n', 'g'}, 
        {'o', 'l', 'y', 'y', 'e'},
        {'o', 'l', 'y', 'n', 'e'},
        {'o', 'm', 'n', 'y', 'g'},
        {'o', 'l', 'n', 'n', 'e'},
        {'o', 'h', 'n', 'y', 'f'},
        {'o', 'h', 'y', 'y', 'e'}
    }; 

    // vfloat_t train_label[] = {
    //    'n','n','y','y','y','n','y','y','n','n','y','n','n','y','y','n','n','n','y'
    // };

    vfloat_t train_label[] = {
        1, 1, -1, -1, -1, 1, -1,-1, 1, 1, -1, 1, 1, -1, -1, 1, 1, 1, -1
    };

    vfloat_t test_data1[] = {
        'm','h','y','n','f'
    };

    matrix2_t* train_mat       = Mat2_create(1,1);
    matrix2_t* train_label_mat = Mat2_create(1,1);
    matrix2_t* test_mat        = Mat2_create(1,1);

    Mat2_load_on_shape(train_mat, train_data, 19, 5);
    Mat2_load_on_shape(train_label_mat, train_label, 19, 1);
    Mat2_load_on_shape(test_mat, test_data1, 1, 5);

    double* alphas_out;
    adaboost_gx_t* Gx_out;
    vfloat_t predict;

    int M = 4;
    adaboost_tree_train(train_mat, train_label_mat, M, &alphas_out,  &Gx_out, NULL);
    adaboost_tree_predict(test_mat, M, Gx_out, alphas_out, &predict);
    // expect is N.
    printf("\nadaboost predict: %f \n", predict);

    Mat2_destroy(train_mat);
    Mat2_destroy(train_label_mat);
    Mat2_destroy(test_mat);
    free(alphas_out);
    free(Gx_out);
    return;
}
static void adaboost_tree_training_progress (char* title, unsigned long step, unsigned long total) 
{
    char buffer[1024];
    memset(buffer, 0x0, sizeof(buffer));
    // if(total==0)
    //     sprintf(buffer, "%s, 进度: %ld ", title, step);
    // else 
    //     sprintf(buffer, "%s, 剩余节点: %ld, 剩余数据：%ld ", title,  step, total);

    //printf("%s\n", buffer);

    sprintf(buffer, "%s, step: %ld , total: %ld, percent: %lf ", title, step, total, (double) step / (double) total );
    printf("%s\r", buffer);
    fflush(stdout);
}

static void test_adaboost_tree_large(void) 
{
    const char* train_csv_file = "/Users/zuweie/code/c-projects/boring-code/build/../src/unit_test/mnist/mnist_train.csv";
    const char* test_csv_file  = "/Users/zuweie/code/c-projects/boring-code/build/../src/unit_test/mnist/mnist_test.csv";

    matrix2_t* csv_mat    = Mat2_create(1,1);
    matrix2_t* train_mat  = Mat2_create(1,1);
    matrix2_t* train_label_mat  = Mat2_create(1,1);
    matrix2_t* test_label_mat = Mat2_create(1,1);
    matrix2_t* test_mat = Mat2_create(1,1);
    matrix2_t* _X       = Mat2_create(1,1);


    Mat2_load_csv(csv_mat, train_csv_file);
    Mat2_slice_col_to(train_label_mat, csv_mat, 0);
    Mat2_slice_cols_to(train_mat, csv_mat, 1, csv_mat->cols);

    Mat2_load_csv(csv_mat, test_csv_file);
    Mat2_slice_col_to(test_label_mat, csv_mat, 0);
    Mat2_slice_cols_to(test_mat, csv_mat, 1, csv_mat->cols);

    MAT2_POOL_PTR(train_mat, train_mat_ptr);
    for (int i=0; i<train_mat->rows; ++i) {
        for (int j=0; j<train_mat->cols; ++j) {

            if (train_mat_ptr[i][j] < 127) {
                train_mat_ptr[i][j] = 0;
            } else {
                train_mat_ptr[i][j] = 1;
            }
            
        }
    }   

    MAT2_POOL_PTR(train_label_mat, train_label_mat_ptr);

    for (int i=0; i<train_label_mat->rows; ++i) {
        for (int j=0; j<train_label_mat->cols; ++j) {

            if (train_label_mat_ptr[i][j] == (vfloat_t)(0)) {
                train_label_mat_ptr[i][j] = 1;
            } else {
                train_label_mat_ptr[i][j] = -1;
            }
            
        }
    }   

    // MAT2_INSPECT(train_label_mat);
    // return;

    MAT2_POOL_PTR(test_mat, test_mat_ptr);
    for (int i=0; i<test_mat->rows; ++i) {
        for (int j=0; j<test_mat->cols; ++j) {
            if (test_mat_ptr[i][j] < 127) {
                test_mat_ptr[i][j] = 0;
            } else {
                test_mat_ptr[i][j] = 1;
            }
        }
    }

    MAT2_POOL_PTR(test_label_mat, test_label_mat_ptr);

    for (int i=0; i<test_label_mat->rows; ++i) {
        for (int j=0; j<test_label_mat->cols; ++j) {
            if (test_label_mat_ptr[i][j] == (vfloat_t)(0)) {
                test_label_mat_ptr[i][j] = 1;
            } else {
                test_label_mat_ptr[i][j] = -1;
            }
        }
    }


    double* alphas_out;
    adaboost_gx_t* Gx_out;
    int M = 40;

    printf("\n ---- *** training ... *** ----\n");
    adaboost_tree_train(train_mat, train_label_mat, M, &alphas_out,  &Gx_out, adaboost_tree_training_progress);

    printf(" \n ---- *** predicting ... *** ---- \n");
    //MAT2_INSPECT(test_label_mat);
    int correct = 0;
    int label;
    char buff[1024];
    memset(buff, 0x0, sizeof(buff));
    int test_number = test_label_mat->rows;

    for (int i=0; i<test_number; ++i) {

        label = (int)test_label_mat->pool[i];

        Mat2_slice_row_to(_X, test_mat, i);

        float predict;

        //decision_tree_classification_predict(_X, &tree, &predict);
        adaboost_tree_predict(_X, M, Gx_out, alphas_out, &predict);

        if ((int)predict == label) 
            correct++; 

        sprintf(buff, "Process: %d / %d, predict: %d - %d, correct %.2f ... ", i+1, test_number, (int)predict, label, ((float) (correct) / (float) (test_number))*100);
        
        printf("%s\r", buff);

        fflush(stdout);

        //printf("correct: %ld \n", correct);
    }
    
    free(alphas_out);
    free(Gx_out);
    Mat2_destroy(csv_mat);
    Mat2_destroy(train_mat);
    Mat2_destroy(train_label_mat);
    Mat2_destroy(test_label_mat); 
    Mat2_destroy(test_mat);
    Mat2_destroy(_X );

    return;
}

static void test_svm_simple (void) 
{

    vfloat_t X_data[][4] ={
    /************* S *************/
        {5.1f, 3.5f, 1.4f, 0.2f}, 
        {4.9f, 3.0f, 1.4f, 0.2f}, 
        {4.7f, 3.2f, 1.3f, 0.2f},

        {4.6f, 3.1f, 1.5f, 0.2f}, 
        {5.0f, 3.6f, 1.4f, 0.2f}, 
        {5.4f, 3.9f, 1.7f, 0.4f},

        {4.6f, 3.4f, 1.4f, 0.3f}, 
        {5.0f, 3.4f, 1.5f, 0.2f}, 
        {4.4f, 2.9f, 1.4f, 0.2f},

        {4.9f, 3.1f, 1.5f, 0.1f}, 
        {5.4f, 3.4f, 1.5f, 0.2f}, 
        {4.8f, 3.4f, 1.6f, 0.2f},

        {4.8f, 3.0f, 1.4f, 0.1f}, 
        {4.3f, 3.0f, 1.1f, 0.1f}, 
        {5.8f, 4.0f, 1.2f, 0.2f},

        {5.7f, 4.4f, 1.5f, 0.4f}, 
        {5.4f, 3.9f, 1.3f, 0.4f}, 
        {5.1f, 3.5f, 1.4f, 0.3f},

        {5.7f, 3.8f, 1.7f, 0.3f}, 
        {5.1f, 3.8f, 1.5f, 0.3f},
    /************* v ************/
        {7.0f, 3.2f, 4.7f, 1.4f}, 
        {6.4f, 3.2f, 4.5f, 1.5f}, 
        {6.9f, 3.1f, 4.9f, 1.5f},

        {5.5f, 2.3f, 4.0f, 1.3f}, 
        {6.5f, 2.8f, 4.6f, 1.5f}, 
        {5.7f, 2.8f, 4.5f, 1.3f}, 

        {6.3f, 3.3f, 4.7f, 1.6f}, 
        {4.9f, 2.4f, 3.3f, 1.0f}, 
        {6.6f, 2.9f, 4.6f, 1.3f},

        {5.2f, 2.7f, 3.9f, 1.4f}, 
        {5.0f, 2.0f, 3.5f, 1.0f}, 
        {5.9f, 3.0f, 4.2f, 1.5f}, 

        {6.0f, 2.2f, 4.0f, 1.0f}, 
        {6.1f, 2.9f, 4.7f, 1.4f}, 
        {5.6f, 2.9f, 3.6f, 1.3f}, 

        {6.7f, 3.1f, 4.4f, 1.4f}, 
        {5.6f, 3.0f, 4.5f, 1.5f}, 
        {5.8f, 2.7f, 4.1f, 1.0f},

        {6.2f, 2.2f, 4.5f, 1.5f}, 
        {5.6f, 2.5f, 3.9f, 1.1f},
    /*********** R **************/
        {6.3f, 3.3f, 6.0f, 2.5f}, 
        {5.8f, 2.7f, 5.1f, 1.9f}, 
        {7.1f, 3.0f, 5.9f, 2.1f},

        {6.3f, 2.9f, 5.6f, 1.8f}, 
        {6.5f, 3.0f, 5.8f, 2.2f}, 
        {7.6f, 3.0f, 6.6f, 2.1f},

        {4.9f, 2.5f, 4.5f, 1.7f}, 
        {7.3f, 2.9f, 6.3f, 1.8f}, 
        {6.7f, 2.5f, 5.8f, 1.8f}, 

        {7.2f, 3.6f, 6.1f, 2.5f}, 
        {6.5f, 3.2f, 5.1f, 2.0f}, 
        {6.4f, 2.7f, 5.3f, 1.9f}, 

        {6.8f, 3.0f, 5.5f, 2.1f}, 
        {5.7f, 2.5f, 5.0f, 2.0f}, 
        {5.8f, 2.8f, 5.1f, 2.4f},

        {6.4f, 3.2f, 5.3f, 2.3f}, 
        {6.5f, 3.0f, 5.5f, 1.8f}, 
        {7.7f, 3.8f, 6.7f, 2.2f}, 
        
        {7.7f, 2.6f, 6.9f, 2.3f}, 
        {6.0f, 2.2f, 5.0f, 1.5f} 
    };

    // static vfloat_t Y_data[]= {
    //     'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 
    //     'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S',
    //     'V', 'V', 'V', 'V', 'V', 'V', 'V', 'V', 'V', 'V', 
    //     'V', 'V', 'V', 'V', 'V', 'V', 'V', 'V', 'V', 'V', 
    //     'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 
    //     'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 
    // };

    static vfloat_t Y_data[]= {
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 
        1, 1, 1, 1, 1, 1, 1, 1, 1, 1,
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
        -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 
    };

    

}

static void test_svm_large(void) 
{
    
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

    // if (NULL == CU_add_test(pSuite, "test perceptron test", test_perceptron) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }


    // if (NULL == CU_add_test(pSuite, "test navie bayes", test_navie_bayes) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "test navie bayes", test_navie_bayes2) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "test navie bayes", test_navies_bayes_mgd) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "test navie bayes", test_navies_bayes_mgd_big) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "test navie bayes", test_navies_bayes_mgd2) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "test classification decision tree", test_decision_tree_large) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }


    // if (NULL == CU_add_test(pSuite, "test classification decision tree simple ", test_decision_tree_simple) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "test classification decision tree simple 2", test_decision_tree_simple2) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "test adaboost tree simple 2", test_adaboost_tree_simple) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "test adaboost tree large", test_adaboost_tree_large) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    if (NULL == CU_add_test(pSuite, "test adaboost tree large", test_svm_simple) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

}

