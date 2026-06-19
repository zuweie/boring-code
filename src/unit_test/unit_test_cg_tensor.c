/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-06-13 14:20:17
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-06-19 13:51:12
 * @FilePath: /boring-code/src/unit_test/unit_test_cg_tensor.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>
#include <CUnit/Basic.h>
#include "deep_learning/compute_graph2/cg_tensor_shape.h"

static int  suite_success_init (void) 
{
    printf("\ncg tensor suite success init\n");
    return 0;
}

static int suite_success_clean (void) 
{   
    printf("\ncg tensor suite success clean\n");
    return 0;
}


void test_tensor_dimension (void) 
{
    int input_axes = 5;
    int input_dimens[] = {1, 1, 2, 4, 3};
    
    cg_tensor_axis_t* shape;
    cg_tensor_shape_create(&shape, input_axes, input_dimens);

    CU_ASSERT_PTR_NOT_EQUAL(shape, NULL);
    CU_ASSERT_EQUAL(shape->axes, 5);
    
    cg_tensor_shape_inspect(shape);
    
    
    // printf("\n");
    if (shape)
        cg_tensor_shape_recycle(shape);

    int input_axes1     = 3;
    int input_dimens1[] = {1,2,1};
    cg_tensor_shape_create(&shape, input_axes1, input_dimens1);

    CU_ASSERT_PTR_NOT_EQUAL(shape, NULL);
    CU_ASSERT_EQUAL(shape->axes, 3);
    CU_ASSERT_EQUAL(cg_tensor_shape_dimens(shape, 1), 2)

    cg_tensor_shape_inspect(shape);
    
    if (shape) {
        cg_tensor_shape_recycle(shape);
    }
}


int do_cg_tensor_test (void) 
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("Test tensor Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }

     if (NULL == CU_add_test(pSuite, "test tensor dimens", test_tensor_dimension) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
}