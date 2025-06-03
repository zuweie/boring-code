/*
 * @Author: your name
 * @Date: 2021-07-02 14:26:30
 * @LastEditTime: 2025-06-02 16:44:04
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/unit_test_svm.c
 */
#include <stdio.h>
#include <CUnit/Basic.h>
//#include "container/List.h"
#include "container/cn.h"
#include "ultra_array/ultra_array.h"
#include "machine_learning/svm/support_vector_machines.h"
#include "machine_learning/svm/svm_problem.h"

#define x_data_row 60
#define x_data_col 4

#define y_data_row 60

static vfloat_t X_data[x_data_row][x_data_col] ={
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

static vfloat_t Y_data[y_data_row]= {
        'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 
        'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S', 'S',
        'V', 'V', 'V', 'V', 'V', 'V', 'V', 'V', 'V', 'V', 
        'V', 'V', 'V', 'V', 'V', 'V', 'V', 'V', 'V', 'V', 
        'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 
        'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 'R', 
    };

static vfloat_t sample_data[4] = {
    6.7f, 3.1f, 4.7f, 1.5f
};

static int  suite_success_init (void) 
{
    printf("\nSVM suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("\nSVM suite success clean\n");
}

static void 
test_sample_classify_problems() 
{
    // svm 多分类的效果。
    u_array_t X = _UArray2d(x_data_row, x_data_col);
    u_array_t Y = _UArray1d(y_data_row);

    UA_load(&X, X_data);
    UA_load(&Y, Y_data);

    CN problems = CN_create(LIST, ptr_t);
    int class_nr = svm_classify_problem(&X, &Y, problems);

    printf("\n");
    for (Iter first=CN_first(problems); !It_equal(first, CN_tail(problems)); first = It_next(first)) {

        svm_classify_problem_t* problem = It_ptr(first);

        printf(" %c, %c ", (int)problem->tagA, (int)problem->tagB);
        printf("\n");

        for (Iter a=CN_first(problem->class_ls_A); !It_equal(a, CN_tail(problem->class_ls_A)); a = It_next(a)) {
            printf(" %d ", It_int(a));
        }
        printf("\n");
        for (Iter b=CN_first(problem->class_ls_B); !It_equal(b, CN_tail(problem->class_ls_B)); b = It_next(b) ){
            printf(" %d ", It_int(b));
        }
        printf("\n");
    }

    svm_classify_problem_finalize(problems);

    UArray_(&X);
    UArray_(&Y);
}

static void test_c_svc_solve (void)
{
    u_array_t X = _UArray2d(x_data_row, x_data_col);
    u_array_t Y = _UArray1d(y_data_row);
    u_array_t sample = _UArray1d(4);

    UA_load(&X, X_data);
    UA_load(&Y, Y_data);
    UA_load(&sample, sample_data);
    //List list = _List(NULL);
    CN models = CN_create(LIST, ptr_t);

    // int svm_solve_c_svc( \
    //     u_array_t* X, u_array_t* Y, \
    //     SVM_kernel svm_kernel, \
    //     double C, double _gammer, \ 
    //     double _coef, double _degree, \
    //     double eps, \
    //     int max_iter,\
    //     List* classify_models)

    svm_solve_c_svc(
        &X, &Y, RBF, 10.f, 8.0f, 0.0f, 0.0f, 0.0001, 300, models
    );

    double r = svm_c_svc_predict(models, &sample);
    CU_ASSERT_EQUAL((char)r, 'V');
    // Debug:
    //printf(" winner type is %c value is %f\n", (char)r, r);

    // svm_models_finalize(&list);
    
    #if 0 
    // model report
    for (It first=CN_first(models); !It_equal(first, CN_tail(models)); It_next(first)) {

        svm_model_t* model = It_ptr(first);
        
        size_t len_Alpha    = UA_length(&model->_star_alpha);
        size_t len_Y        = UA_length(&model->_star_Y);
        size_t len_Xr       = UA_shape_axis(&model->_star_X, 0);
        
        vfloat_t* Alpha_ptr = UA_data_ptr(&model->_star_alpha);
        vfloat_t* Y_ptr     = UA_data_ptr(&model->_star_Y);
        size_t len_Xc       = UA_shape_axis(&model->_star_X, 1);
        vfloat_t (*X_ptr)[len_Xc] = UA_data_ptr(&model->_star_X);

        printf(" \n\n ... model report ... \n");
        printf(" support vectors : %d, \n", model->sv_count);
        printf(" rho: %lf \n", model->_star_rho);
        printf(" tagA: %lf, %c\n", model->tagA, (char)model->tagA);
        printf(" tagB: %lf, %c \n", model->tagB, (char)model->tagB);

        printf(" star alpha : \n");
        for (int i=0; i<len_Alpha; ++i) {
            printf("alpha[%d]: %lf , *y[%d], %lf \n", i, Alpha_ptr[i], i, Y_ptr[i]);
        }
        //printf("\n\n");

        //printf(" star Y :\n");
        //for (int j=0; j<len_Y; ++j) {
            //printf(" Y[%d]: %lf ", j, Y_ptr[j]);
        //}
        printf("\n\n");
        printf(" star X: \n");
        for (int k=0; k<len_Xr; k++){
            for (int l=0; l<len_Xc; l++) {
                printf("X[%d][%d]: %f ", k, l, X_ptr[k][l]);
            }
            printf("\n");
        }

        svm_model_finalize(model);
        free(model);
    }
    #endif
    // free models
    CN_finalize(models, NULL);
}


static void test_nu_svc_solve(void) 
{
    u_array_t X = _UArray2d(x_data_row, x_data_col);
    u_array_t Y = _UArray1d(y_data_row);
    u_array_t sample = _UArray1d(4);

    UA_load(&X, X_data);
    UA_load(&Y, Y_data);
    UA_load(&sample, sample_data);
    //List list = _List(NULL);
    CN models = CN_create(LIST, ptr_t);


// int svm_solve_nu_svc(        
//         u_array_t* X, 
//         u_array_t* Y, 
//         SVM_kernel svm_kernel, 
//         double nu,
//         double _gammer, 
//         double _coef, 
//         double _degree, 
//         double eps, 
//         int max_iter, 
//         CN classify_models 
// )
    svm_solve_nu_svc(
        &X, &Y, RBF, 0.05f, 8.0f, 0.0f, 0.0f, 0.0001, 300, models
    );
    double r = svm_nu_svc_predict(models, &sample);
    CU_ASSERT_EQUAL((char)r, 'V');
    #if 0
    // model report
    for (It first=CN_first(models); !It_equal(first, CN_tail(models)); It_next(first)) {

        svm_model_t* model = It_ptr(first);
        
        size_t len_Alpha    = UA_length(&model->_star_alpha);
        size_t len_Y        = UA_length(&model->_star_Y);
        size_t len_Xr       = UA_shape_axis(&model->_star_X, 0);
        
        vfloat_t* Alpha_ptr = UA_data_ptr(&model->_star_alpha);
        vfloat_t* Y_ptr     = UA_data_ptr(&model->_star_Y);
        size_t len_Xc       = UA_shape_axis(&model->_star_X, 1);
        vfloat_t (*X_ptr)[len_Xc] = UA_data_ptr(&model->_star_X);

        printf(" \n\n ... model report ... \n");
        printf(" support vectors : %d, \n", model->sv_count);
        printf(" rho: %lf \n", model->_star_rho);
        printf(" r: %f \n", model->_star_r);
        printf(" tagA: %lf, %c\n", model->tagA, (char)model->tagA);
        printf(" tagB: %lf, %c \n", model->tagB, (char)model->tagB);

        printf(" star alpha : \n");
        for (int i=0; i<len_Alpha; ++i) {
            printf("alpha[%d]: %lf , *y[%d], %lf \n", i, Alpha_ptr[i], i, Y_ptr[i]);
        }
        //printf("\n\n");
        printf("\n\n");
        printf(" star X: \n");
        for (int k=0; k<len_Xr; k++){
            for (int l=0; l<len_Xc; l++) {
                printf("X[%d][%d]: %f ", k, l, X_ptr[k][l]);
            }
            printf("\n");
        }

        svm_model_finalize(model);
        free(model);
    }
    #endif
}

int do_svm_test (void) 
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("Test Toutiao Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test svm sample classify", test_sample_classify_problems) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test svm c svc solver", test_c_svc_solve) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (NULL == CU_add_test(pSuite, "test svm nu svc solver", test_nu_svc_solve) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
}