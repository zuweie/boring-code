/*
 * @Author: your name
 * @Date: 2021-02-01 13:25:23
 * @LastEditTime: 2024-08-31 10:24:19
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/unit_test_ultra_array.c
 */
#include <stdio.h>
#include <string.h>
#include <CUnit/Basic.h>
#include "ultra_array/ultra_array.h"
#include "ultra_array/ultra_router.h"
#include "ultra_array/ultra_data_chunk.h"
#include "ultra_array/ultra_padding.h"
#include "ultra_array/tensor.h"

#define PRINTF_SHAPE_AXIS(shape, axis_n) \
    ({ \
        printf("\n axis_n %d \n", axis_n); \
        for (int i=0; i<axis_n; ++i) { \
            printf(" axis(%d): %d ", i, shape[i]); \
        } \
        printf("\n"); \
    })

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
        printf("\n array data address %p \n", UA_data_ptr(&arr)); \
        printf("\n array data :\n");\
        size_t number = 1; \
        for (int i=0; i<arr.axis_n; ++i) { \
            number *= UA_shape_axis(&arr, i); \
        } \
        vfloat_t* data = UA_data_ptr(&arr); \
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

#define PRINTF_INDICATOR_LIST(router, plist) \
    ({ \
        ua_indicator_t* ptr = plist; \
        printf("\n route list <%s>: \n", router); \
        while(ptr != NULL) { \
            printf("axis %d, picked %d, axis_start %d, axis_tail %d \n", ptr->__axis, ptr->__picked, ptr->__start, ptr->__tail); \
            ptr = ptr->next; \
        } \
        printf("\n"); \
    })

#define PRINTF_CHUNK_MAP(map, pf_data) \
    ({  \
        ua_data_chunk_t* ptr = (map); \
        while(ptr != NULL) { \
            printf(" chunk address %p \n", ptr->chunk_addr); \
            if (pf_data) { \
                printf(" data: \n"); \
                vfloat_t* data_ptr = ptr->chunk_addr; \
                for (int i=0; i<(ptr->chunk_size / sizeof(vfloat_t)); ++i) { \
                    printf(" %0.2f \n", data_ptr[i]); \
                } \
            } \
            ptr = ptr->next; \
        } \
        printf("\n");   \
    })
    
#define PRINTF_FORMAT_BANK(b_n) \
    ({ \
        for (int i=0; i<b_n; ++i) { \
            printf("  "); \
        } \
    })

#define PRINTF_PAD_WIDTH(pad_width, size) \
    ({ \
        for (int i=0; i<size; ++i) { \
            printf(" before %d, after %d \n", pad_width[i].before_n, pad_width[i].after_n); \
        } \
    }) 
static void printf_uarr(u_array_t* arr, int axis, char* ptr, int bank_number) 
{
    bank_number++;
    if (axis == UA_axisn(arr) - 1) {
        vfloat_t* data = ptr;

        PRINTF_FORMAT_BANK(bank_number);

        printf("[");

        size_t chunk_number = UA_shape_axis(arr, axis);
        
        for (int i=0; i<chunk_number; ++i) {
            printf("%0.2f ", data[i]);
        }
        printf("],\n");
    } else {
        
        size_t chunk_number = UA_shape_axis(arr, axis);
        size_t chunk_size = UArray_axis_mulitply(arr, axis+1) * sizeof(vfloat_t);
        PRINTF_FORMAT_BANK(bank_number);
        printf("[\n");
        for (int i=0; i<chunk_number; ++i) {
            printf_uarr(arr, axis+1, ptr + i*chunk_size, bank_number);
        }
        PRINTF_FORMAT_BANK(bank_number);
        printf("],\n");
        
    }
    //bank_number--;
}
static int  suite_success_init (void) 
{
    printf("\nUltra Array research suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("\nUltra Array research suite success clean\n");
}

static void test_uarray_create (void) 
{
    u_array_t arr = _UArray2d(2, 3);
    UA_arange(&arr, 2*3);

    u_array_t arr1 = _UArray2d(2, 3);
    UA_arange(&arr1, 2*3);
    
    u_array_t arr3 = _UArray3d(2, 3, 4);
    UA_arange(&arr3, 2*3*4);

    UA_sum(&arr, 1);
    //PRINTF_ARRAY(arr);
    CU_ASSERT_TRUE(UA_axisn(&arr) == 1);
    CU_ASSERT_TRUE(UA_get(&arr, 0) == 3.f);
    CU_ASSERT_TRUE(UA_get(&arr, 1) == 12.f );

    UA_sum(&arr1, 0);
    //PRINTF_ARRAY(arr1);
    CU_ASSERT_TRUE(UA_axisn(&arr1) == 0);
    CU_ASSERT_TRUE(UA_get(&arr1, 0) == 15.f);
    
    /* test 3d */
    UA_sum(&arr3, 2);
    //PRINTF_ARRAY(arr3);
    CU_ASSERT_TRUE(UA_axisn(&arr3) == 2);
    CU_ASSERT_TRUE(UA_get(&arr3, 1, 2) == 86.f);
    
    UArray_(&arr);
    UArray_(&arr1);
    UArray_(&arr3);

}

static void test_coord_index (void) {
    size_t coord[] = {1, 2, 6};
    u_array_t arr_3d = _UArray3d(2, 3, 8);
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

    u_array_t arr = _UArray2d(7, 3);
    UA_arange(&arr, 7*3);

    vfloat_t (*data_arr)[3] = UA_data_copy(&arr);
    UA_T(&arr);
    vfloat_t (*data_trans_arr)[7] = UA_data_ptr(&arr);
    CU_ASSERT_TRUE( data_arr[6][2] == data_trans_arr[2][6] );

    u_array_t arr2 = _UArray3d(2, 2, 4);
    UA_arange(&arr2, 2*2*4);
    //PRINTF_ARRAY(arr2);

    vfloat_t (*data_arr2)[2][4] = UA_data_copy(&arr2);
    UA_T(&arr2);
    //PRINTF_ARRAY(arr2);
    
    vfloat_t (*data_trans_arr2)[2][2] = UA_data_ptr(&arr2); 
    CU_ASSERT_TRUE( data_arr2[0][1][3] == data_trans_arr2[3][1][0] );

    free(data_arr2);
    free(data_arr);
    
    UArray_(&arr);
    UArray_(&arr2);
}   


static void test_array_dot (void) 
{
    u_array_t u1 = _UArray3d(1, 2, 4);
    UA_arange(&u1, 1*2*4);
    u_array_t u2 = _UArray3d(2, 4, 3);
    UA_arange(&u2, 2*4*3);
    
    u_array_t u3 = UA_dot_new(&u1, &u2);
    
    vfloat_t* data = UA_data_ptr(&u3);
    size_t size_u3 = UA_length(&u3);
    CU_ASSERT_TRUE(data[0] == 42.f);
    CU_ASSERT_TRUE(data[size_u3-1] == 422.f);

    //PRINTF_ARRAY(u3); 
    UArray_(&u1);
    UArray_(&u2);
    UArray_(&u3);   
}

static void test_array_dot2 (void) 
{
    u_array_t u1 = _UArray1d(5);
    UA_arange(&u1, 5);
    u_array_t u2 = _UArray1d(5);
    UA_arange(&u2, 5);
    UArray_dot(&u1, &u2);

    vfloat_t* ptr = UA_data_ptr(&u1);

    CU_ASSERT_TRUE(ptr[0] == 30.f);
    
    UArray_(&u1);
    UArray_(&u2);
}

static void test_array_router (void) 
{
    u_array_t u1 = _UArray3d(3, 3, 4);
    UA_arange(&u1, 3*3*4);
    ua_chunk_note_t chunk_note;
//-----------------------------------------------
    #if 1
    //printf(" \nshape u1: 2 * 3 * 4 \n");
    //PRINTF_ARRAY(u1);
    printf("router:\n");
    ua_indicator_t* list;
    char r1[] = ":,:,0";
    UArray_indicator_parse_str(r1, &list);
    // PRINTF_INDICATOR_LIST(r1, list);
 
    UArray_indicator_analysis(list, &u1, &chunk_note);
    
    //PRINTF_CHUNK_MAP(chunk_note.chunk_map, 1);
    //PRINTF_SHAPE_AXIS(chunk_note.shape, chunk_note.axis_n);
    CU_ASSERT_TRUE(chunk_note.axis_n == 2);
    CU_ASSERT_TRUE(chunk_note.shape[1] == 3);

    //Router_release(list);
    UArray_indicator_release(list);
    UArray_chunk_note_finalize(&chunk_note);
    
    #endif
// -------------------------------------------

    #if 0
    route_node_t* list2;
    char r2[] = "1,,1:3";
    Router_parse(r2, &list2);
    //PRINTF_ROUTE_LIST(r2, list2);

    UArray_analysis_router(&u1, list2, &shape, &axis_n);
    //PRINTF_SHAPE_AXIS(shape, axis_n);

    CU_ASSERT_TRUE(axis_n == 2);
    CU_ASSERT_TRUE(shape[0] == 3);
    CU_ASSERT_TRUE(shape[1] == 2);
    if (shape) free(shape);
    Router_release(list2);

    #endif

// -------------------------------------------
    #if 0
    route_node_t* list3;
    char r3[] = "1";
    Router_parse(r3, &list3);
    //PRINTF_ROUTE_LIST(r3, list3);

    UArray_analysis_router(&u1, list3, &shape, &axis_n);
    //PRINTF_SHAPE_AXIS(shape, axis_n);

    CU_ASSERT_TRUE(axis_n == 2);
    CU_ASSERT_TRUE(shape[0] == 3);
    CU_ASSERT_TRUE(shape[1] == 4);
    Router_release(list3);

    if (shape) free(shape);
    #endif

// -------------------------------------------

    #if 0
    route_node_t* list4;
    char r4[] = "1,:,2";
    Router_parse(r4, &list4);
    //PRINTF_ROUTE_LIST(r4, list4);

    UArray_analysis_router(&u1, list4, &shape, &axis_n);
    //PRINTF_SHAPE_AXIS(shape, axis_n);

    CU_ASSERT_TRUE(axis_n == 1);
    CU_ASSERT_TRUE(shape[0] = 3);    
    if (shape) free(shape);
    Router_release(list4);
    #endif
// -------------------------------------------

    #if 0
    route_node_t* list5;
    char r5[] = ":,,2";
    Router_parse(r5, &list5);
    //PRINTF_ROUTE_LIST(r5, list5);

    UArray_analysis_router(&u1, list5, &shape, &axis_n);
    //PRINTF_SHAPE_AXIS(shape, axis_n);

    CU_ASSERT_TRUE( axis_n == 2);
    CU_ASSERT_TRUE(shape[0] == 2);
    CU_ASSERT_TRUE(shape[1] == 3); 
    if (shape) free(shape);
    Router_release(list5);
    #endif
// -------------------------------------------

    #if 0
    route_node_t* list6;
    char r6[] = "0:1,,2";
    Router_parse(r6, &list6);
    //PRINTF_ROUTE_LIST(r6, list6);

    UArray_analysis_router(&u1, list6, &shape, &axis_n);
    //PRINTF_SHAPE_AXIS(shape, axis_n);

    CU_ASSERT_TRUE(axis_n == 2);
    CU_ASSERT_TRUE(shape[0] == 1);
    CU_ASSERT_TRUE(shape[1] == 3);
    if (shape) free(shape);
    Router_release(list6);
    #endif
// -------------------------------------------
    #if 0
    route_node_t* list7;
    char r7[] = ",:-1,:-1";
    Router_parse(r7, &list7);
    //PRINTF_ROUTE_LIST(r7, list7);

    UArray_analysis_router(&u1, list7, &shape, &axis_n);
    //PRINTF_SHAPE_AXIS(shape, axis_n);


    CU_ASSERT_TRUE(axis_n == 3);
    CU_ASSERT_TRUE(shape[0] == 2);
    CU_ASSERT_TRUE(shape[1] == 2);
    CU_ASSERT_TRUE(shape[2] == 3);
    if (shape) free(shape);
    Router_release(list7);
    #endif
// -------------------------------------------

    #if 0
    route_node_t* list8;
    char r8[] = "";
    Router_parse(r8, &list8);
    //PRINTF_ROUTE_LIST(r8, list8);

    UArray_analysis_router(&u1, list8, &shape, &axis_n);
    //PRINTF_SHAPE_AXIS(shape, axis_n);

    CU_ASSERT_TRUE(axis_n == 3);
    CU_ASSERT_TRUE(shape[0] == 2);
    CU_ASSERT_TRUE(shape[1] == 3);
    CU_ASSERT_TRUE(shape[2] == 4);
    if (shape) free(shape);
    Router_release(list8);
    #endif
// -------------------------------------------
    UArray_(&u1);
}
static void test_fission(void) 
{
    // u_array_t u1 = _UArray3d(3,3,4);
    // UA_arange(&u1, 3*3*4);

    // u_array_t u2 = UA_fission(&u1, ":,:,1");
    // CU_ASSERT_TRUE(UA_get(&u1, 2, 2, 1) == UA_get(&u2, 2, 2));
    // UArray_(&u1);
    // UArray_(&u2);

    u_array_t u1 = _UArray3d(3,3,3);
    UA_arange(&u1, 3*3*3);

    u_array_t u2 = UA_fission(&u1, "0:2,0:1");
    //CU_ASSERT_TRUE(UA_get(&u1, 2, 2, 1) == UA_get(&u2, 2, 2));

    printf("u1: \n");
    UA_display(&u1);
    printf("\n u2: \n");
    UA_display(&u2);

    UArray_(&u1);
    UArray_(&u2);
}

static void test_fission2(void) {

    UA_scope_indicators(index, 3);
    UA_set_scope_indicators_selected(index, 0, 0, 0);
    UA_set_scope_indicators_selected(index, 1, 0, 0);
    UA_set_scope_indicators_picked(index, 2, 1);


    u_array_t u1 = _UArray3d(2,3,4);
    UA_arange(&u1, 2*3*4);

    printf_uarr(&u1, 0, UA_data_ptr(&u1), 0);

    u_array_t u2 = UArray_fission_with_indicators(&u1, index);
    printf_uarr(&u2, 0, UA_data_ptr(&u2), 0);
    
    UArray_(&u1);
    UArray_(&u2);
}

static void test_fission3(void) 
{
    UA_scope_indicators(index, 3);
    UA_set_scope_indicators_selected(index, 0, 0, 0);
    UA_set_scope_indicators_selected(index, 1, 0, 0);
    UA_set_scope_indicators_picked(index, 2, 1);

    u_array_t u1 = _UArray3d(2,3,4);
    UA_arange(&u1, 2*3*4);

    printf_uarr(&u1, 0, UA_data_ptr(&u1), 0);

    u_array_t u2 = _UArray1d(6);

    UA_fission_to_uar_by_indicators(&u1, &u2, index);

    UA_display(&u2);

    UArray_(&u1);
    UArray_(&u2);
    
}

static void test_slice(void) {
    u_array_t u1 = _UArray3d(2,3,4);
    UA_arange(&u1, 2*3*4);

    printf_uarr(&u1, 0, UA_data_ptr(&u1), 0);
    //u_array_t u2 = UA_slice(&u1, 3, 0, 2, 0, 3, 1, 2);
    u_array_t u2 = UA_slice(&u1, 3, _sc(0,2), _sc(0, 3), _pk(1));
    UA_display(&u2);

    UArray_(&u1);
    UArray_(&u2);
}   

static void test_assimilate(void) 
{
    u_array_t u1 = _UArray3d(3,3,4);
    UA_ones(&u1, -10);
    //PRINTF_ARRAY(u1);
    
    u_array_t u2  = _UArray1d(3);
    UA_arange(&u2, 3);
    //PRINTF_ARRAY(u2);

    UA_assimilate(&u1, ":,:,1", &u2);

    //PRINTF_ARRAY(u1);

    CU_ASSERT_TRUE(UA_get(&u1, 2,0,1) == 0.f);
    CU_ASSERT_TRUE(UA_get(&u1, 2,1,1) == 1.f);
    CU_ASSERT_TRUE(UA_get(&u1, 2,2,1) == 2.f);


    u_array_t u3 = _UArray3d(3,3,4);
    UA_ones(&u3, -9);
    //PRINTF_ARRAY(u3);

    u_array_t u4 = _UArray1d(4);
    UA_arange(&u4, 4);
    UA_assimilate(&u3, "2,2", &u4);

    //PRINTF_ARRAY(u3);
    CU_ASSERT_TRUE(UA_get(&u3, 2,2,0) == 0.f);
    CU_ASSERT_TRUE(UA_get(&u3, 2,2,1) == 1.f);
    CU_ASSERT_TRUE(UA_get(&u3, 2,2,2) == 2.f);
    CU_ASSERT_TRUE(UA_get(&u3, 2,2,3) == 3.f);
    
    u_array_t u5 = _UArray3d(3,4,7);
    u_array_t u6 = _UArray1d(11);


    u_array_t* result = UA_assimilate(&u5, "", &u6);
    CU_ASSERT_FALSE(result);

    UArray_(&u1);
    UArray_(&u2);
    UArray_(&u3);
    UArray_(&u4);
    UArray_(&u5);
    UArray_(&u6);
}

static void test_printf_uarray(void) 
{
    u_array_t u1 = _UArray3d(3,3,4);
    UA_ones(&u1, 0);
    printf("\n");
    printf_uarr(&u1, 0, UA_data_ptr(&u1), 0);
    UArray_(&u1);
}

static void test_ua_cover_padn_to_router(void) 
{
    char buffer[256];
    ua_pad_width_t pad_ns[3];
    pad_ns[0].before_n = 1;
    pad_ns[0].after_n = 1;

    pad_ns[1].before_n = 1;
    pad_ns[1].after_n = 1;

    pad_ns[2].before_n = 1;
    pad_ns[2].after_n = 1;

    UArray_cover_pad_width_to_router_str(pad_ns, 3, buffer);
    
    ua_indicator_t* indicators;
    UArray_indicator_parse_str(buffer, &indicators);

    printf("\nrouter %s\n", buffer);
    PRINTF_INDICATOR_LIST(buffer, indicators);
    UArray_indicator_release(indicators);
}

static void test_ua_pad(void) 
{
    u_array_t u1 = _UArray3d(2,3,3);
    UA_arange(&u1,2*3*3);
    printf_uarr(&u1, 0, UA_data_ptr(&u1), 0);
    
    ua_pad_width_t padn[3];
    padn[0].after_n = 1;
    padn[0].before_n = 3;
    padn[1].after_n = 3;
    padn[1].before_n = 1;
    padn[2].after_n = 2;
    padn[2].before_n = 8;

    //u_array_t u2 = UArray_padding(&u1, padn, 2);
    u_array_t u2 = UArray_pad(&u1, "((1,3),(3,1),(1,2))", ua_pad_mode_edge, NULL);
    printf("\n");
    printf_uarr(&u2, 0, UA_data_ptr(&u2), 0);

    UArray_(&u1);
    UArray_(&u2);
}

static void test_ua_pad_const(void) 
{
    u_array_t u1 = _UArray3d(2,3,3);
    UA_arange(&u1,2*3*3);
    printf_uarr(&u1, 0, UA_data_ptr(&u1), 0);
    
    ua_pad_width_t padn[3];
    padn[0].after_n = 1;
    padn[0].before_n = 3;
    padn[1].after_n = 3;
    padn[1].before_n = 1;
    padn[2].after_n = 2;
    padn[2].before_n = 1;
    vfloat_t consts[3];
    consts[0] = 3;
    consts[1] = 2;
    consts[2] = 1;
    u_array_t u2 = UArray_pad(&u1, "((1,3),(3,1),(1,2))", ua_pad_mode_constanst, consts);
    //printf("\n");
    //printf_uarr(&u2, 0, UA_data_ptr(&u2), 0);

    UArray_(&u1);
    UArray_(&u2);
}

static void test_ua_pad_width_parse(void) 
{
    ua_pad_width_t pad_width[3] = {0};
    printf("\n");
    int ret = UArray_parse_pad_width_str("((1,3),(3,1),(1,2))", pad_width, 3);
    CU_ASSERT_TRUE(ret == 0);
    CU_ASSERT_TRUE(pad_width[0].before_n == 1);
    CU_ASSERT_TRUE(pad_width[1].before_n == 3);
    //PRINTF_PAD_WIDTH(pad_width, 3);
}

static void test_ua_scope(void) 
{
    u_array_t u1 = _UArray1d(5);
    UA_scope(&u1, -2, 2+1);
    printf("\n:ua scope\n");
    //PRINTF_ARRAY(u1);
    CU_ASSERT_TRUE(UA_get(&u1, 0) == -2.f);
    CU_ASSERT_TRUE(UA_get(&u1, 4) == 2.f);
}

static void test_ua_mean(void)
{
    u_array_t u1 = _UArrayXd(4, 2,3,4,5);
    u_array_t u2 = _UArrayXd(4, 2,3,4,5);
    u_array_t u3 = _UArrayXd(4, 2,3,4,5);
    u_array_t u4 = _UArrayXd(4, 2,3,4,5);

    UA_arange(&u1, 2*3*4*5);
    UA_arange(&u2, 2*3*4*5);
    UA_arange(&u3, 2*3*4*5);
    UA_arange(&u4, 2*3*4*5);

    UA_mean(&u1, 0);
    UA_mean(&u2, 1);
    UA_mean(&u3, 2);
    UA_mean(&u4, 3);
    
    printf("\n mean 0\n");
    printf_uarr(&u1, 0, UA_data_ptr(&u1), 0);
    printf("\n mean 1\n");
    printf_uarr(&u2, 0, UA_data_ptr(&u2), 0);
    printf("\n mean 2\n");
    printf_uarr(&u3, 0, UA_data_ptr(&u3), 0);
    printf("\n mean 3\n");
    printf_uarr(&u4, 0, UA_data_ptr(&u4), 0);

    UArray_(&u1);
    UArray_(&u2);
    UArray_(&u3);
    UArray_(&u4);
}

static void test_ua_operations(void) 
{
    u_array_t u1 = _UArray2d(2,3);
    UA_ones(&u1, 1);
    UA_sum_var(&u1, 2);
    printf_uarr(&u1, 0, UA_data_ptr(&u1), 0);
    
    vfloat_t arr[3] = {1,2,3};
    UA_mulitply_arr(&u1, arr, 3);
    printf_uarr(&u1, 0, UA_data_ptr(&u1), 0);

    u_array_t u2 = _UArray1d(3);
    UA_ones(&u2, -1.f);
    UA_sum_uar(&u1, &u2);
    printf_uarr(&u1, 0, UA_data_ptr(&u1), 0);
    
    UArray_(&u1);
    UArray_(&u2);
}

static void test_ua_pool_size(void) 
{
    u_array_t u1 = _UArray1d(3);
    printf("ua pool size %d \n", UA_pool_size(&u1));
    size_t new_shape[2] = {3, 4};
    UA_reshape(&u1, new_shape, 2);
    printf("ua pool size %d \n", UA_pool_size(&u1));
    new_shape[0] = 1;
    new_shape[1] = 22;
    UA_reshape(&u1, new_shape, 2);
    printf("ua pool size %d \n", UA_pool_size(&u1));
    UArray_(&u1);

}

static void test_xarr_arange(void) {
    tensor_t* arr = tensor_create(3, 8, 9, 4);
    tensor_arange(arr, 1, 8*9*4 + 1);
    printf("\n\n");
    tensor_display(arr);
    tensor_recycle(arr);
}

static void test_xarr_slice(void) 
{ 
    tensor_t* arr = tensor_create(3, 8, 9, 4);
    tensor_arange(arr, 1, 8*9*4 + 1);
    printf("\n\n");
    tensor_display(arr);
    tensor_t* slice_arr = tensor_slice(arr, 1, 4, 3, 6, 2, 3 );
    printf("\n\n");
    tensor_display(slice_arr);
    tensor_recycle(arr);
    tensor_recycle(slice_arr);

}

static void test_xarr_padding(void) 
{
    tensor_t* arr = tensor_create(3, 2, 3, 4);
    tensor_arange(arr, 1, 2*3*4 + 1);
    tensor_display(arr);

    tensor_t* padding_arr = tensor_padding(arr, 0.0f, 1, 1, 1, 2, 1, 3);
    printf("\n\n");
    tensor_display(padding_arr);
    tensor_recycle(arr);
    tensor_recycle(padding_arr);

}

int do_ultra_array_test (void) 
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("Test Ultra Array Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    } 
    
    #if 0
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

    if (NULL == CU_add_test(pSuite, "test ultra array dot2 ", test_array_dot2) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test ultra array router ", test_array_router) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    
    if (NULL == CU_add_test(pSuite, "test ultra array fission ", test_fission) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    #endif

    // if (NULL == CU_add_test(pSuite, "test ultra array slice ", test_slice) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    #if 0
    if (NULL == CU_add_test(pSuite, "test ultra array fission2 ", test_fission2) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if (NULL == CU_add_test(pSuite, "test ultra array fission3 ", test_fission3) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test ultra array assimlate ", test_assimilate) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test ultra array tostring ", test_printf_uarray) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test ultra cover padns to router ", test_ua_cover_padn_to_router) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test ultra pad ", test_ua_pad) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test pad width parse ", test_ua_pad_width_parse) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if (NULL == CU_add_test(pSuite, "test ua scope ", test_ua_scope) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    if (NULL == CU_add_test(pSuite, "test ultra pad const", test_ua_pad_const) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test uarray mean", test_ua_mean) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test uarray operations", test_ua_operations) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test uarray pool size", test_ua_pool_size) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    #endif

    // if (NULL == CU_add_test(pSuite, "test x array arange ", test_xarr_arange) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "test x slice ", test_xarr_slice) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    if (NULL == CU_add_test(pSuite, "test x slice ", test_xarr_padding) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
}