/*
 * @Author: your name
 * @Date: 2021-03-09 15:47:11
 * @LastEditTime: 2021-03-10 11:18:49
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/automatic_speech_recognition/compare_mfcc.c
 */
#include <math.h>
#include "ultra_array/ultra_router.h"
#include "ultra_array/ultra_array.h"
#include "compare_mfcc.h"

u_array_t compare_mfcc_cosine(u_array_t* mfcc1, u_array_t* mfcc2)
{
    size_t size_m1 = UA_shape_axis(mfcc1, 0);
    size_t size_m2 = UA_shape_axis(mfcc2, 0);
    
    size_t size_row = UA_shape_axis(mfcc1, 0) < UA_shape_axis(mfcc2, 0) ? UA_shape_axis(mfcc1, 0) : UA_shape_axis(mfcc2, 0);
    u_array_t scores = _UArray1d(size_row);
    
    for (size_t i=0; i<size_row; ++i) {

        ua_indicator_t* idx = __indicators_start_tail(NULL, i, i+1);
        u_array_t u1 = UArray_fission_with_indicators(mfcc1, idx);
        u_array_t u2 = UArray_fission_with_indicators(mfcc2, idx);
        u_array_t u3 = UA_dot(&u1, UA_T(&u2));
        UA_pow2(&u1);
        UA_pow2(&u2);

        double _dot = UA_get(&u3, 0, 0);
        UA_sum(&u1, 0);
        UA_sum(&u2, 0);

        double sum_u1 = UA_get(&u1, 0);
        double sum_u2 = UA_get(&u2, 0);
        double mod_u1 = sqrt(sum_u1);
        double mod_u2 = sqrt(sum_u2);
        double _score = _dot / mod_u1 / mod_u2;

        UA_set(&scores, _score, i);

        // clean up
        UArray_(&u1);
        UArray_(&u2);
        UArray_(&u3);
        UArray_indicator_release(idx);
    }

    return scores;
}

u_array_t compare_mfcc_distance(u_array_t* mfcc1, u_array_t* mfcc2)
{
    size_t row_mf1 = UA_shape_axis(mfcc1, 0);
    size_t row_mf2 = UA_shape_axis(mfcc2, 0);

    size_t row = UA_shape_axis(mfcc1, 0) < UA_shape_axis(mfcc2, 0) ? UA_shape_axis(mfcc1, 0) : UA_shape_axis(mfcc2, 0);
    size_t col = UA_shape_axis(mfcc1, 1);
    u_array_t scores = _UArray1d(row);
    
    double (*row_ptr_1)[col] = UA_data_ptr(mfcc1);
    double (*row_ptr_2)[col] = UA_data_ptr(mfcc2);
    
    for (size_t i=0; i<row; ++i) {

        double *_data_1 = row_ptr_1[i];
        double *_data_2 = row_ptr_2[i];
        double distance = 0.f;
        for (size_t j=0; j<col; ++j) {
           distance +=  (_data_2[j] - _data_1[j])*(_data_2[j] - _data_1[j]);
        }
        distance = sqrt(distance);
        UA_set(&scores, distance, i);
    }
    return scores;
}