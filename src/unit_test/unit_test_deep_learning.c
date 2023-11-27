#include <stdio.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "vtype/vfloat_type.h"
#include "deep_learning/ann.h"
#include "deep_learning/active.h"

static int  suite_success_init (void) 
{
    printf("\ndeep learning suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("\ndeep learning suite success clean\n");
}

static void ann_train_progress(char* title, unsigned long step, unsigned long total, double error) 
{
    char buffer[1024];
    memset(buffer, 0x0, sizeof(buffer));
    sprintf(buffer, "%s, step: %ld , total: %ld, percent: %lf, error: %lf", title, step, total, (double) step / (double) total, fabs(error) > 100.f ? 100.f : error);
    printf("%s\r", buffer);
    fflush(stdout);
}

static void test_ann(void) {
    

    #define x_data_row 60
    #define x_data_col 4

    #define y_data_row 60
    #define y_data_col 3

    vfloat_t trainingData[x_data_row][x_data_col] ={
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

    vfloat_t response1[y_data_row][y_data_col] = {
        {1.,0.,0.},{1.,0.,0.},{1.,0.,0.},{1.,0.,0.},{1.,0.,0.},{1.,0.,0.},{1.,0.,0.},{1.,0.,0.},{1.,0.,0.},{1.,0.,0.},
        {1.,0.,0.},{1.,0.,0.},{1.,0.,0.},{1.,0.,0.},{1.,0.,0.},{1.,0.,0.},{1.,0.,0.},{1.,0.,0.},{1.,0.,0.},{1.,0.,0.},
        {0.,1.,0.},{0.,1.,0.},{0.,1.,0.},{0.,1.,0.},{0.,1.,0.},{0.,1.,0.},{0.,1.,0.},{0.,1.,0.},{0.,1.,0.},{0.,1.,0.},
        {0.,1.,0.},{0.,1.,0.},{0.,1.,0.},{0.,1.,0.},{0.,1.,0.},{0.,1.,0.},{0.,1.,0.},{0.,1.,0.},{0.,1.,0.},{0.,1.,0.},
        {0.,0.,1.},{0.,0.,1.},{0.,0.,1.},{0.,0.,1.},{0.,0.,1.},{0.,0.,1.},{0.,0.,1.},{0.,0.,1.},{0.,0.,1.},{0.,0.,1.},
        {0.,0.,1.},{0.,0.,1.},{0.,0.,1.},{0.,0.,1.},{0.,0.,1.},{0.,0.,1.},{0.,0.,1.},{0.,0.,1.},{0.,0.,1.},{0.,0.,1.}
    };

    vfloat_t _sample [4] = {
        4.6f, 3.2f, 1.4f, 0.2f
    };

    matrix2_t* data = Mat2_create(1,1);
    matrix2_t* label = Mat2_create(1,1);
    matrix2_t* _Input = Mat2_create(1,1);
    matrix2_t* predict = Mat2_create(1,1);

    Mat2_load_on_shape(data, trainingData, x_data_row, x_data_col);
    Mat2_load_on_shape(label, response1, y_data_row, y_data_col);
    Mat2_load_on_shape(_Input, _sample, 4, 1);

    double symmetric_sigmoid_params[] = { 2./3, 1.7159};
    ann_param_t params;
    params.max_iter = 100000;
    params.learning_rate = 0.3f;
    params.batch = 40;
    params.trim_epsilon = 1e-6; // 0.001
    params.lambda = 1e-3;
    params.act.active = ann_symmetric_sigmoid;
    params.act.active_params = symmetric_sigmoid_params;
    params.d_act.d_active = ann_d_symmetric_sigmoid;
    params.d_act.d_active_params = symmetric_sigmoid_params;
    int hidden_layer_cells_number[] = {5};
    int hidden_layer_length = sizeof(hidden_layer_cells_number) / sizeof (int);
    matrix2_t* out_Wbs;
    int out_Wbs_length;

    // 开训
    ann_train(data, label, hidden_layer_cells_number, hidden_layer_length, &params, &out_Wbs, &out_Wbs_length, ann_train_progress);

    // 
    ann_predict(_Input, out_Wbs, out_Wbs_length, &params.act, predict);

    MAT2_INSPECT(predict);
    
    Mat2_destroy(data);
    Mat2_destroy(label);
    Mat2_destroy(_Input);
    Mat2_destroy(predict);

    ann_recycle(out_Wbs, out_Wbs_length);

    return;
}
int do_deep_learning_test (void) 
{

    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("The deep learning Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "ann test", test_ann) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

}