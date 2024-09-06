#include <stdio.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "vtype/vfloat_type.h"
#include "deep_learning/ann.h"
#include "deep_learning/active.h"
#include "deep_learning/rnn.h"

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
    // fucking data
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
    params.term_epsilon = 1e-4; // 0.001
    params.lambda = 0.f;

    params.hidden_act.active = sigmoid;
    params.hidden_act.active_params = symmetric_sigmoid_params;
    params.hidden_d_act.d_active = d_sigmoid;
    params.hidden_d_act.d_active_params = symmetric_sigmoid_params;

    int hidden_layer_cells_number[] = {5};
    int hidden_layer_length = sizeof(hidden_layer_cells_number) / sizeof (int);
    matrix2_t* out_Wbs;
    int out_Wbs_length;

    // 开训
    ann_train(data, label, hidden_layer_cells_number, hidden_layer_length, &params, &out_Wbs, &out_Wbs_length, ann_train_progress);

    // 
    ann_predict(_Input, out_Wbs, out_Wbs_length, &params.hidden_act, predict);

    MAT2_INSPECT(predict);
    
    Mat2_destroy(data);
    Mat2_destroy(label);
    Mat2_destroy(_Input);
    Mat2_destroy(predict);

    ann_recycle(out_Wbs, out_Wbs_length);

    return;
}

static void rnn_train_progress(char* title, unsigned long step, unsigned long total, double error) 
{
    char buffer[1024];
    memset(buffer, 0x0, sizeof(buffer));
    sprintf(buffer, "%s, step: %ld , total: %ld, percent: %lf, error: %lf", title, step, total, (double) step / (double) total, fabs(error) > 100.f ? -1.f : error);
    printf("%s\r", buffer);
    fflush(stdout);
}



static void test_rnn (void) 
{
    #define train_cols 3
    #define test_cols 3
    #define label_cols 3
    // vfloat_t train_data[][6] = {
    //     {0,0,0,0,0,0},
    //     {1,1,1,1,1,1},
    //     {2,2,2,2,2,2},
    //     {3,3,3,3,3,3},
    //     {4,4,4,4,4,4},
    //     {5,5,5,5,5,5},
    //     {6,6,6,6,6,6},
    //     {7,7,7,7,7,7},
    //     {8,8,8,8,8,8},
    //     {9,9,9,9,9,9},
    //     {2,2,2,2,2,2},
    //     {3,3,3,3,3,3},
    //     {4,4,4,4,4,4},
    //     {0,0,0,0,0,0},
    //     {1,1,1,1,1,1},
    //     {2,2,2,2,2,2},
    //     {3,3,3,3,3,3},
    //     {4,4,4,4,4,4},
    //     {5,5,5,5,5,5},
    //     {6,6,6,6,6,6},
    //     {7,7,7,7,7,7},
    //     {8,8,8,8,8,8},
    //     {9,9,9,9,9,9},
    //     {2,2,2,2,2,2},
    //     {3,3,3,3,3,3},
    //     {4,4,4,4,4,4},
        
    // };
    
    // vfloat_t train_data[][6] = {
    //     {0,0,0,0,0,0},
    //     {0,0,0,0,0,0.1},
    //     {0,0,0,0,0,0.2},
    //     {0,0,0,0,0,0.3},
    //     {0,0,0,0,0,0.4},
    //     {0,0,0,0,0,0.5},
    //     {0,0,0,0,0,0.6},
    //     {0,0,0,0,0,0.7},
    //     {0,0,0,0,0,0.8},
    //     {0,0,0,0,0,0.9},
    //     {0,0,0,0,0,0.2},
    //     {0,0,0,0,0,0.3},
    //     {0,0,0,0,0,0.4},
    //     {0,0,0,0,0,0},
    //     {0,0,0,0,0,0.1},
    //     {0,0,0,0,0,0.2},
    //     {0,0,0,0,0,0.3},
    //     {0,0,0,0,0,0.4},
    //     {0,0,0,0,0,0.5},
    //     {0,0,0,0,0,0.6},
    //     {0,0,0,0,0,0.7},
    //     {0,0,0,0,0,0.8},
    //     {0,0,0,0,0,0.9},
    //     {0,0,0,0,0,0.2},
    //     {0,0,0,0,0,0.3},
    //     {0,0,0,0,0,0.4},
    // };

    // vfloat_t train_data[][6] = {
    //     {'0','0','0','0','0','0'},
    //     {'1','1','1','1','1','1'},
    //     {'2','2','2','2','2','2'},
    //     {'3','3','3','3','3','3'},
    //     {'4','4','4','4','4','4'},
    //     {'5','5','5','5','5','5'},
    //     {'6','6','6','6','6','6'},
    //     {'7','7','7','7','7','7'},
    //     {'8','8','8','8','8','8'},
    //     {'9','9','9','9','9','9'},
    //     {'2','2','2','2','2','2'},
    //     {'3','3','3','3','3','3'},
    //     {'4','4','4','4','4','4'},
    //     {'0','0','0','0','0','0'},
    //     {'1','1','1','1','1','1'},
    //     {'2','2','2','2','2','2'},
    //     {'3','3','3','3','3','3'},
    //     {'4','4','4','4','4','4'},
    //     {'5','5','5','5','5','5'},
    //     {'6','6','6','6','6','6'},
    //     {'7','7','7','7','7','7'},
    //     {'8','8','8','8','8','8'},
    //     {'9','9','9','9','9','9'},
    //     {'2','2','2','2','2','2'},
    //     {'3','3','3','3','3','3'},
    //     {'4','4','4','4','4','4'},
    // };

    // vfloat_t label_data [][10] = {
    //     {1,0,0,0,0,0,0,0,0,0},
    //     {0,1,0,0,0,0,0,0,0,0},
    //     {0,0,1,0,0,0,0,0,0,0},
    //     {0,0,0,1,0,0,0,0,0,0},
    //     {0,0,0,0,1,0,0,0,0,0},
    //     {0,0,0,0,0,1,0,0,0,0},
    //     {0,0,0,0,0,0,1,0,0,0},
    //     {0,0,0,0,0,0,0,1,0,0},
    //     {0,0,0,0,0,0,0,0,1,0},
    //     {0,0,0,0,0,0,0,0,0,1},
    //     {0,0,1,0,0,0,0,0,0,0},
    //     {0,0,0,1,0,0,0,0,0,0},
    //     {0,0,0,0,1,0,0,0,0,0},
    //     {1,0,0,0,0,0,0,0,0,0},
    //     {0,1,0,0,0,0,0,0,0,0},
    //     {0,0,1,0,0,0,0,0,0,0},
    //     {0,0,0,1,0,0,0,0,0,0},
    //     {0,0,0,0,1,0,0,0,0,0},
    //     {0,0,0,0,0,1,0,0,0,0},
    //     {0,0,0,0,0,0,1,0,0,0},
    //     {0,0,0,0,0,0,0,1,0,0},
    //     {0,0,0,0,0,0,0,0,1,0},
    //     {0,0,0,0,0,0,0,0,0,1},
    //     {0,0,1,0,0,0,0,0,0,0},
    //     {0,0,0,1,0,0,0,0,0,0},
    //     {0,0,0,0,1,0,0,0,0,0},
    // };

    // vfloat_t train_data[][6] = {
    //     {'0','0','0','0','0','0'},
    //     {'1','1','1','1','1','1'},
    //     {'2','2','2','2','2','2'},
    //     {'3','3','3','3','3','3'},
    //     {'4','4','4','4','4','4'},
    //     {'5','5','5','5','5','5'},
    //     {'6','6','6','6','6','6'},
    //     {'7','7','7','7','7','7'},
    //     {'8','8','8','8','8','8'},
    //     {'9','9','9','9','9','9'},
    // };

    vfloat_t train_data[][train_cols] = {
        {'1','1','1'},
        {'2','2','2'},
        {'3','3','3'},
        {'1','1','1'},
        {'2','2','2'},
        {'3','3','3'},
        {'2','2','2'},
        {'3','3','3'},
    };

    // vfloat_t label_data [][10] = {
    //     {1,0,0,0,0,0,0,0,0,0},
    //     {0,1,0,0,0,0,0,0,0,0},
    //     {0,0,1,0,0,0,0,0,0,0},
    //     {0,0,0,1,0,0,0,0,0,0},
    //     {0,0,0,0,1,0,0,0,0,0},
    //     {0,0,0,0,0,1,0,0,0,0},
    //     {0,0,0,0,0,0,1,0,0,0},
    //     {0,0,0,0,0,0,0,1,0,0},
    //     {0,0,0,0,0,0,0,0,1,0},
    //     {0,0,0,0,0,0,0,0,0,1} 
    // };

    // vfloat_t label_data [][label_cols] = {
    //     {0,1,0,0,0,0,0,0,0,0},
    //     {0,0,1,0,0,0,0,0,0,0},
    //     {0,0,0,1,0,0,0,0,0,0},
    //     {0,1,0,0,0,0,0,0,0,0},
    //     {0,0,1,0,0,0,0,0,0,0},
    //     {0,0,0,1,0,0,0,0,0,0},
    //     {0,0,1,0,0,0,0,0,0,0},
    //     {0,0,0,1,0,0,0,0,0,0},
    // };

    vfloat_t label_data [][label_cols] = {
        {1,0,0},
        {0,1,0},
        {0,0,1},
        {1,0,0},
        {0,1,0},
        {0,0,1},
        {0,1,0},
        {0,0,1},
    };

    vfloat_t test_data [][test_cols] = {
        {'1','1','1'},
        {'2','2','2'},
        {'3','3','3'},
        {'1','1','1'},
    };

    
    matrix2_t* seq_data  = Mat2_create(1,1);
    matrix2_t* seq_label = Mat2_create(1,1);
    matrix2_t* seq_test  = Mat2_create(1,1);

    matrix2_t* _W_xh;
    matrix2_t* _W_hh;
    matrix2_t* _W_hy;

    matrix2_t* _outputs;

    Mat2_load_on_shape(seq_data, train_data, sizeof(train_data) / (sizeof(vfloat_t) * train_cols), train_cols);
    Mat2_load_on_shape(seq_label, label_data, sizeof(label_data) / (sizeof(vfloat_t) * label_cols), label_cols);
    Mat2_load_on_shape(seq_test, test_data, sizeof(test_data) / (sizeof (vfloat_t) * test_cols),  test_cols);

    Mat2_normalize_on_col(seq_data);
    Mat2_normalize_on_col(seq_test);

    rnn_param_t rnn_params;
    rnn_params.max_iter = 10000;
    rnn_params.term_epsilon = 0.5f;
    rnn_params.learning_rate = 0.01;
    rnn_params.hidden_layer_cells_numbers = 3;

    rnn_params.hidden_act.active = tanh1;
    rnn_params.hidden_act.params = NULL;
    rnn_params.hidden_d_act.d_active = d_tanh1;
    rnn_params.hidden_d_act.params = NULL;

    rnn_params.output_act.active = softmax;
    rnn_params.output_act.params = NULL;
    rnn_params.output_d_act.d_active = d_softmax;
    rnn_params.output_d_act.params = NULL;

    //Mat2_normalize_on_col(seq_data);
    //Mat2_normalize_on_col(seq_test);

    rnn_train(seq_data, seq_label, &rnn_params, &_W_xh, &_W_hh, &_W_hy, rnn_train_progress);

    //MAT2_INSPECT(_W_xh);
    //MAT2_INSPECT(_W_hh);
    //MAT2_INSPECT(_W_hy);

    rnn_predict(seq_test, _W_xh, _W_hh, _W_hy, &rnn_params.hidden_act, &rnn_params.output_act, &_outputs);

    MAT2_INSPECT(_outputs);

    Mat2_destroy(seq_data);
    Mat2_destroy(seq_label);
    Mat2_destroy(seq_test);

    Mat2_destroy(_W_xh);
    Mat2_destroy(_W_hh);
    Mat2_destroy(_W_hy);

    Mat2_destroy(_outputs);
}

static void test_ann_cg_simulation(void) 
{

}

int do_deep_learning_test (void) 
{

    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("The deep learning Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }

    // if (NULL == CU_add_test(pSuite, "ann test", test_ann) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "rnn test", test_rnn) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    if (NULL == CU_add_suite(pSuite, "ann cg simulation", test_ann_cg_simulation)) {
        CU_cleanup_registry();
        return CU_get_error();
    }

}