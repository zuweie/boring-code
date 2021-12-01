/*
 * @Author: your name
 * @Date: 2021-11-15 16:49:19
 * @LastEditTime: 2021-12-01 16:22:17
 * @LastEditors: Please set LastEditors
 * @Description: 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 * @FilePath: /boring-code/src/unit_test/unit_test_neural_network.c
 */
#include <stdio.h>
#include <stdarg.h>
#include <CUnit/Basic.h>
#include "machine_learning/neural_network.h"
#include "ultra_array/ultra_array.h"
#include "vtype/vfloat_type.h"

#define x_data_row 60
#define x_data_col 4

#define y_data_row 60
#define y_data_col 3

static int  suite_success_init (void) 
{
    printf("\n neural suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("\n neural suite success clean\n");
}

static void test_mlp_rand(void) {
    for (int i=0; i<50; i++) {
        double v = ann_mpl_rand1(-1, 1);
        printf("v: %f ", v);
    }
}
static void test_mlp_wb_init (void)
{
    u_array_t layer = _UArray1d(3);
    float layer_data[3] = {4, 5, 3};
    UA_load(&layer, layer_data);
    
    ann_mpl_model_t* model = ann_mpl_model_create(&layer, 1);

    int l, k, wb;
    // int i = 0;
    // for (l=1; l<Nl_count(model); ++l) {
    //     for (k=0; k<Wm_k(model, l); ++k) {
    //         vfloat_t* wb_ptr = Wk_ptr(model, l, k);
    //         for (wb = 0; wb<Wm_h(model, l); ++wb) {
    //             wb_ptr[wb] = (float)i;
    //             //printf("wb[%d]: %p \n", wb, &wb_ptr[wb]);
    //         }
    //         i++;
    //     }
    // }

    for (l=1; l<Nl_count(model); ++l) {
        printf("layer %d\n",l);
        for (k=0; k<Wm_k(model, l); ++k){
            printf("vector %d\n", k);
            vfloat_t* wb_ptr = Wk_ptr(model, l, k);
            for (wb=0; wb<Wm_h(model,l); ++wb) {
                printf("wb[%d] %f, addr: %p\n", wb, wb_ptr[wb], &wb_ptr[wb]);
            }
        }
        printf("\n\n");
    }
}

static void test_mpl_training_predict(void)
{
    float trainingData[x_data_row][x_data_col] ={
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

    float response1[y_data_row][y_data_col] = {
        {1.,0.,0.},{1.,0.,0.},{1.,0.,0.},{1.,0.,0.},{1.,0.,0.},{1.,0.,0.},{1.,0.,0.},{1.,0.,0.},{1.,0.,0.},{1.,0.,0.},
        {1.,0.,0.},{1.,0.,0.},{1.,0.,0.},{1.,0.,0.},{1.,0.,0.},{1.,0.,0.},{1.,0.,0.},{1.,0.,0.},{1.,0.,0.},{1.,0.,0.},
        {0.,1.,0.},{0.,1.,0.},{0.,1.,0.},{0.,1.,0.},{0.,1.,0.},{0.,1.,0.},{0.,1.,0.},{0.,1.,0.},{0.,1.,0.},{0.,1.,0.},
        {0.,1.,0.},{0.,1.,0.},{0.,1.,0.},{0.,1.,0.},{0.,1.,0.},{0.,1.,0.},{0.,1.,0.},{0.,1.,0.},{0.,1.,0.},{0.,1.,0.},
        {0.,0.,1.},{0.,0.,1.},{0.,0.,1.},{0.,0.,1.},{0.,0.,1.},{0.,0.,1.},{0.,0.,1.},{0.,0.,1.},{0.,0.,1.},{0.,0.,1.},
        {0.,0.,1.},{0.,0.,1.},{0.,0.,1.},{0.,0.,1.},{0.,0.,1.},{0.,0.,1.},{0.,0.,1.},{0.,0.,1.},{0.,0.,1.},{0.,0.,1.}
    };
    u_array_t layer = _UArray1d(3);
    float layer_data[3] = {4, 5, 3};
    UA_load(&layer, layer_data);

    u_array_t X = _UArray2d(x_data_row, x_data_col);
    UA_load(&X, trainingData);

    u_array_t Y = _UArray2d(y_data_row, y_data_col);
    UA_load(&Y, response1);
    ann_mpl_param_t params;
    params.active = 0;
    params.epsilon = 0.0001;
    params.max_iter = 100000;
    params.param1 = 0.1;
    params.param2 = 0.1;
    ann_mpl_model_t* model = ann_mpl_training(&layer, &X, &Y, &params);
    
    u_array_t predict = _UArray1d(3);
    float sampleData [4] = {
        4.6f, 3.2f, 1.4f, 0.2f
    };
    u_array_t sample = _UArray1d(4);
    UA_load(&sample,sampleData);
    ann_mpl_predict(model, &sample, &predict);
    ann_mpl_model_finalize(model);
}

int do_neural_test (void) {
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("Test neural Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }
    // if (NULL == CU_add_test(pSuite, "test wb int", test_mlp_wb_init) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "test rand int", test_mlp_rand) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    if (NULL == CU_add_test(pSuite, "test train predict", test_mpl_training_predict) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
}