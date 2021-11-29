/*
 * @Author: your name
 * @Date: 2021-11-15 16:49:19
 * @LastEditTime: 2021-11-29 11:21:45
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

int do_neural_test (void) {
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("Test neural Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }
    if (NULL == CU_add_test(pSuite, "test wb int", test_mlp_wb_init) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    // if (NULL == CU_add_test(pSuite, "test rand int", test_mlp_rand) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }
}