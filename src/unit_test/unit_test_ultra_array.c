/*
 * @Author: your name
 * @Date: 2021-02-01 13:25:23
 * @LastEditTime: 2021-02-03 10:27:09
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/unit_test_ultra_array.c
 */
#include <stdio.h>
#include <CUnit/Basic.h>
#include "ultra_array/ultra_array.h"

#define PRINTF_ARRAY_AXIS(arr) \
    ({ \
        int axis_n = arr.axis_n; \
        printf("\n array axis_n %d \n", arr.axis_n);\
        for (int i=0; i<axis_n; ++i) { \
            size_t dimens = UA_shape_axis(arr, i); \
            printf(" axis(%d): %d ", i, dimens); \
        } \
        printf("\n"); \
    })
#define PRINTF_ARRAY_DATA(arr) \
    ({ \
        printf("\n array data :\n");\
        size_t number = 1; \
        for (int i=0; i<arr.axis_n; ++i) { \
            number *= UA_shape_axis(arr, i); \
        } \
        double* data = UA_data_ptr(arr); \
        for (int j=0; j<number; ++j) { \
            printf(" %.2f ", data[j]); \
        } \
        printf("\n"); \
    })
#define PRINTF_ARRAY(arr) \
    ({ \
        PRINTF_ARRAY_AXIS(arr);\
        PRINTF_ARRAY_DATA(arr);\
    })
static int  suite_success_init (void) 
{
    printf("\nUltra Array research suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("\nUltra Array research suite success clean\n");
}

static void test_uarray_create () 
{
    u_array_t arr = _UArray2d(NULL, 2, 3);
    double *data = UA_data_ptr(arr);
    
    for (int i=0; i<2*3; ++i) {
        data[i] = i+1;
    }
    printf("\n array axis: \n");
    PRINTF_ARRAY_AXIS(arr);
    printf("\n array data: \n");
    PRINTF_ARRAY_DATA(arr);
    
    u_array_t arr_sum = UA_sum(arr, 1);
    printf("\n array axis: \n");
    PRINTF_ARRAY_AXIS(arr_sum);
    printf("\n array data: \n");
    PRINTF_ARRAY_DATA(arr_sum);

    u_array_t arr_sum2 = UA_sum(arr, 0);
    printf("\n array axis: \n");
    PRINTF_ARRAY_AXIS(arr_sum2);
    printf("\n array data: \n");
    PRINTF_ARRAY_DATA(arr_sum2);
    
    /* test 3d */
    u_array_t arr3d = _UArray3d(NULL, 2, 3, 4);
    double* pdata = UA_data_ptr(arr3d);

    for (int i=0; i<2*3*4; ++i) {
        pdata[i] = i+1;
    }
    
    printf("\n array axis: \n");
    PRINTF_ARRAY_AXIS(arr3d);
    printf("\n array data: \n");
    PRINTF_ARRAY_DATA(arr3d);

    u_array_t arr3d_sum = UA_sum(arr3d, 2);

    printf("\n array axis: \n");
    PRINTF_ARRAY_AXIS(arr3d_sum);
    printf("\n array data: \n");
    PRINTF_ARRAY_DATA(arr3d_sum);

    u_array_t arr3d_sum_2 = UA_sum(arr3d, 1);
    printf("\n array axis: \n");
    PRINTF_ARRAY_AXIS(arr3d_sum_2);
    printf("\n array data: \n");
    PRINTF_ARRAY_DATA(arr3d_sum_2);

    UArray_(arr);
    UArray_(arr_sum);
    UArray_(arr_sum2);
    UArray_(arr3d);
    UArray_(arr3d_sum);
    UArray_(arr3d_sum_2);
}

static void test_coord_index (void) {
    size_t coord[] = {1, 2, 6};
    u_array_t arr_3d = _UArray3d(NULL, 2, 3, 8);
    size_t offset = UA_cover_coordinate(arr_3d, coord);

    CU_ASSERT_TRUE(offset == 46);

    size_t coord2[3];
    UA_cover_offset(arr_3d, 46, coord2);

    CU_ASSERT_TRUE(coord2[0] == 1);
    CU_ASSERT_TRUE(coord2[1] == 2);
    CU_ASSERT_TRUE(coord2[2] == 6);
    UArray_(arr_3d);
}

static void test_ultra_array_transform(void) 
{
    u_array_t arr = _UArray2d(NULL, 7, 3);
    UA_range(arr, 3*7);
    PRINTF_ARRAY(arr);
     
}

int do_ultra_array_test (void) 
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("Test Ultra Array Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    } 

    if (NULL == CU_add_test(pSuite, "test uarray create ", test_uarray_create) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if (NULL == CU_add_test(pSuite, "test uarray coord index ", test_coord_index) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    
    if (NULL == CU_add_test(pSuite, "test ultra array transform ", test_ultra_array_transform) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
}