/*
 * @Author: your name
 * @Date: 2021-02-01 13:25:23
 * @LastEditTime: 2021-02-10 11:32:59
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/unit_test_ultra_array.c
 */
#include <stdio.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "ultra_array/ultra_array.h"
#include "ultra_array/ultra_router.h"

#define PRINTF_ARRAY_AXIS(arr) \
    ({ \
        int axis_n = arr.axis_n; \
        printf("\n array axis_n %d \n", arr.axis_n);\
        for (int i=0; i<axis_n; ++i) { \
            size_t dimens = UA_shape_axis(&arr, i); \
            printf(" axis(%d): %d ", i, dimens); \
        } \
        printf("\n"); \
    })
#define PRINTF_ARRAY_DATA(arr) \
    ({ \
        printf("\n array data :\n");\
        size_t number = 1; \
        for (int i=0; i<arr.axis_n; ++i) { \
            number *= UA_shape_axis(&arr, i); \
        } \
        double* data = UA_data_ptr(&arr); \
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

#define PRINTF_ROUTE_LIST(router, plist) \
    ({ \
        route_node_t* ptr = plist; \
        printf("\n route list <%s>: \n", router); \
        while(ptr != NULL) { \
            printf("axis %d, axis_start %d, axis_tail %d \n", ptr->axis, ptr->axis_start, ptr->axis_tail); \
            ptr = ptr->next; \
        } \
        printf("\n"); \
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
    UA_arange(&arr, 2*3);

    u_array_t arr1 = _UArray2d(NULL, 2, 3);
    UA_arange(&arr1, 2*3);
    
    u_array_t arr3 = _UArray3d(NULL, 2, 3, 4);
    UA_arange(&arr3, 2*3*4);

    UA_sum(&arr, 1);
    PRINTF_ARRAY(arr);

    UA_sum(&arr1, 0);
    PRINTF_ARRAY(arr1);
    
    /* test 3d */
    UA_sum(&arr3, 2);
    PRINTF_ARRAY(arr3);

    UArray_(&arr);
    UArray_(&arr1);
    UArray_(&arr3);

}

static void test_coord_index (void) {
    size_t coord[] = {1, 2, 6};
    u_array_t arr_3d = _UArray3d(NULL, 2, 3, 8);
    size_t offset = UA_cover_coordinate(&arr_3d, coord);

    CU_ASSERT_TRUE(offset == 46);

    size_t coord2[3];
    UA_cover_offset(&arr_3d, 46, coord2);

    CU_ASSERT_TRUE(coord2[0] == 1);
    CU_ASSERT_TRUE(coord2[1] == 2);
    CU_ASSERT_TRUE(coord2[2] == 6);
    UArray_(&arr_3d);
}


static void test_ultra_array_transform(void) 
{

    u_array_t arr = _UArray2d(NULL, 7, 3);
    UA_arange(&arr, 7*3);

    double (*data_arr)[3] = UA_data_copy(&arr);
    UA_T(&arr);
    double (*data_trans_arr)[7] = UA_data_ptr(&arr);
    CU_ASSERT_TRUE( data_arr[6][2] == data_trans_arr[2][6] );

    u_array_t arr2 = _UArray3d(NULL, 2, 2, 4);
    UA_arange(&arr2, 2*2*4);
    //PRINTF_ARRAY(arr2);

    double (*data_arr2)[2][4] = UA_data_copy(&arr2);
    UA_T(&arr2);
    //PRINTF_ARRAY(arr2);
    
    double (*data_trans_arr2)[2][2] = UA_data_ptr(&arr2); 
    CU_ASSERT_TRUE( data_arr2[0][1][3] == data_trans_arr2[3][1][0] );

    free(data_arr2);
    free(data_arr);
    
    UArray_(&arr);
    UArray_(&arr2);
}   


static void test_array_dot (void) 
{
    u_array_t u1 = _UArray3d(NULL, 1, 2, 4);
    UA_arange(&u1, 1*2*4);
    u_array_t u2 = _UArray3d(NULL, 2, 4, 3);
    UA_arange(&u2, 2*4*3);
    
    u_array_t u3 = UA_dot(&u1, &u2);
    
    double* data = UA_data_ptr(&u3);
    size_t size_u3 = UA_size(&u3);
    CU_ASSERT_TRUE(data[0] == 42.f);
    CU_ASSERT_TRUE(data[size_u3-1] == 422.f);

    //PRINTF_ARRAY(u3); 
    UArray_(&u1);
    UArray_(&u2);
    UArray_(&u3);   
}
static void test_array_router (void) 
{
    u_array_t u1 = _UArray3d(NULL, 2, 3, 4);
    UA_arange(&u1, 2*3*4);

    route_node_t* list;
    char r1[] = "1,2,1:3";
    Router_parse(&u1,r1, &list);

    PRINTF_ROUTE_LIST(r1, list);

    route_node_t* list2;
    char r2[] = "1,,1:3";
    Router_parse(&u1, r2, &list2);
    PRINTF_ROUTE_LIST(r2, list2);

    route_node_t* list3;
    char r3[] = "1";
    Router_parse(&u1, r3, &list3);
    PRINTF_ROUTE_LIST(r3, list3);

    route_node_t* list4;
    char r4[] = "1,,2";
    Router_parse(&u1, r4, &list4);
    PRINTF_ROUTE_LIST(r4, list4);

    route_node_t* list5;
    char r5[] = ",,2";
    Router_parse(&u1, r5, &list5);
    PRINTF_ROUTE_LIST(r5, list5);

    route_node_t* list6;
    char r6[] = "0:1,,2";
    Router_parse(&u1, r6, &list6);
    PRINTF_ROUTE_LIST(r6, list6);

    route_node_t* list7;
    char r7[] = ",:-1,:-1";
    Router_parse(&u1, r7, &list7);
    PRINTF_ROUTE_LIST(r7, list7);

    Router_release(list);
    Router_release(list2);
    Router_release(list3);
    Router_release(list4);
    Router_release(list5);
    Router_release(list6);
    Router_release(list7);
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

    if (NULL == CU_add_test(pSuite, "test ultra array dot ", test_array_dot) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }


    if (NULL == CU_add_test(pSuite, "test ultra array router ", test_array_router) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
}