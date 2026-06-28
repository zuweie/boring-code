/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2026-06-13 14:20:17
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2026-06-28 17:30:25
 * @FilePath: /boring-code/src/unit_test/unit_test_cg_tensor.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>
#include <CUnit/Basic.h>
#include "deep_learning/compute_graph2/cg_allocator.h"
#include "deep_learning/compute_graph2/cg_tensor_elem_spec.h"
#include "deep_learning/compute_graph2/cg_tensor_shape.h"
#include "deep_learning/compute_graph2/cg_tensor.h"

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
    CU_ASSERT_EQUAL(SHAPE_AXES(shape, 2), 3);
    CU_ASSERT_EQUAL(SHAPE_DIMENS(shape, 3), 4);
    cg_tensor_shape_inspect(shape);
    
    // printf("\n");
    if (shape)
        cg_tensor_shape_recycle(shape);

    int input_axes1     = 3;
    int input_dimens1[] = {1,2,1};
    cg_tensor_shape_create(&shape, input_axes1, input_dimens1);

    CU_ASSERT_PTR_NOT_EQUAL(shape, NULL);
    CU_ASSERT_EQUAL(shape->axes, 3);
    CU_ASSERT_EQUAL(SHAPE_DIMENS(shape, 1), 2)

    cg_tensor_shape_inspect(shape);
    
    if (shape) {
        cg_tensor_shape_recycle(shape);
    }
}

void test_tensor_create(void) 
{
    cg_allocator_t alloc;
    cg_allocator_init(&alloc);
    
    cg_tensor_t* t1 = cg_tensor_create(&alloc, 3, 2, 3, 4);

    CU_ASSERT_EQUAL(AXIS_AXES(t1->shape), 3);
    CU_ASSERT_EQUAL(SHAPE_DIMENS(t1->shape, 0), 2);
    CU_ASSERT_EQUAL(SHAPE_DIMENS(t1->shape, 1), 3);
    CU_ASSERT_EQUAL(SHAPE_DIMENS(t1->shape, 2), 4);


    CU_ASSERT_EQUAL(SHAPE_STRIDE(t1->shape, 0), 12);
    CU_ASSERT_EQUAL(SHAPE_STRIDE(t1->shape, 1), 4);
    CU_ASSERT_EQUAL(SHAPE_STRIDE(t1->shape, 2), 1);

    CU_ASSERT_EQUAL(AXIS_NUMBER(t1->shape), 24);

    cg_tensor_fill(t1,  1.0);
    cg_tensor_inspect(t1);

    //CU_ASSERT_DOUBLE_EQUAL( ((cg_tensor_elem_type*)(t1->elems))[23], 1.f, 0.001 );

    cg_tensor_arange(t1, 0, 24);
    cg_tensor_inspect(t1);

    cg_tensor_t* t_slice = cg_tensor_slice(t1, 3, 1, 2, 1, 3, 2, 4);
    cg_tensor_inspect(t_slice);

    cg_tensor_t* t_slice2 = cg_tensor_slice(t1, 2, 1, 2, 1, 3);
    cg_tensor_inspect(t_slice2);

    cg_tensor_t* t_slice3 = cg_tensor_slice(t1, 1, 1, 2);
    cg_tensor_inspect(t_slice3);

    cg_tensor_t* t_padding = cg_tensor_padding(t1, pd_mode_edge, (cg_tensor_elem_type)0.f, 3, 1, 1, 1, 1, 1, 1);
    cg_tensor_inspect(t_padding);

    cg_tensor_t* t_padding2 = cg_tensor_padding(t1, pd_mode_edge, (cg_tensor_elem_type)0.f, 1, 1, 1);
    cg_tensor_inspect(t_padding2);
    
    cg_tensor_elem_type* v = cg_tensor_get(t1, 1, 1,2);
    CU_ASSERT_DOUBLE_EQUAL(*v, 18, 0.001);

    v = cg_tensor_get(t1, 0, 2, 3);
    CU_ASSERT_DOUBLE_EQUAL(*v, 11, 0.001);

    v = cg_tensor_get(t_slice, 0, 1, 0);
    CU_ASSERT_DOUBLE_EQUAL(*v, 22, 0.001);

    v = cg_tensor_get(t_padding, 2, 3, 1);
    CU_ASSERT_DOUBLE_EQUAL(*v, 16, 0.001);

    // cg_tensor_t* tt = cg_tensor_create(&alloc, 3, 2, 2, 3);
    // cg_tensor_arange(tt, 0, 2*2*3);
    // cg_tensor_inspect(tt);

    // cg_tensor_T(tt);
    // cg_tensor_inspect(tt);
    
    cg_tensor_recycle(t1);
    cg_tensor_recycle(t_slice);
    cg_tensor_recycle(t_slice2);
    cg_tensor_recycle(t_slice3);
    cg_tensor_recycle(t_padding);
    //cg_tensor_recycle(tt);

    cg_allocator_reset(&alloc);
    return;
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


    if (NULL == CU_add_test(pSuite, "test tensor dimens", test_tensor_create) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    return 0;
}