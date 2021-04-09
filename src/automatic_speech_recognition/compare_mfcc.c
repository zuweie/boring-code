/*
 * @Author: your name
 * @Date: 2021-03-09 15:47:11
 * @LastEditTime: 2021-04-09 09:29:46
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/automatic_speech_recognition/compare_mfcc.c
 */
#include <math.h>
#include "ultra_array/ultra_router.h"
#include "ultra_array/ultra_array.h"
#include "compare_mfcc.h"
#include "vtype/vfloat_type.h"
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

        vfloat_t _dot = UA_get(&u3, 0, 0);
        UA_sum(&u1, 0);
        UA_sum(&u2, 0);

        vfloat_t sum_u1 = UA_get(&u1, 0);
        vfloat_t sum_u2 = UA_get(&u2, 0);
        vfloat_t mod_u1 = sqrt(sum_u1);
        vfloat_t mod_u2 = sqrt(sum_u2);
        vfloat_t _score = _dot / mod_u1 / mod_u2;

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
    
    vfloat_t (*row_ptr_1)[col] = UA_data_ptr(mfcc1);
    vfloat_t (*row_ptr_2)[col] = UA_data_ptr(mfcc2);
    
    for (size_t i=0; i<row; ++i) {

        vfloat_t *_data_1 = row_ptr_1[i];
        vfloat_t *_data_2 = row_ptr_2[i];
        vfloat_t distance = 0.f;
        for (size_t j=0; j<col; ++j) {
           distance +=  (_data_2[j] - _data_1[j])*(_data_2[j] - _data_1[j]);
        }
        distance = sqrt(distance);
        UA_set(&scores, distance, i);
    }
    return scores;
}

u_array_t compare_mfcc_pearson(u_array_t* mfcc1, u_array_t* mfcc2) 
{
    size_t row_mf1 = UA_shape_axis(mfcc1, 0);
    size_t row_mf2 = UA_shape_axis(mfcc2, 0);
    int need_free_pad = 0;
    u_array_t u1, u2;
    u_array_t scores = _UArray1d((row_mf1>row_mf2 ? row_mf1 :row_mf2));
    if (row_mf1 != row_mf2) {
        need_free_pad = 1;
        ua_pad_width_t pad_width[2] = {0};
        pad_width[0].after_n = abs(row_mf1 - row_mf2);

        u1 = UArray_padding((row_mf1 < row_mf2 ? mfcc1 : mfcc2), pad_width, ua_pad_mode_constanst, NULL);
        u2 = row_mf1 > row_mf2 ? *mfcc1 : *mfcc2;
        
    } else {
        u1 = *mfcc1;
        u2 = *mfcc2;
    }
    
    // 做余弦比较
    size_t col = UA_shape_axis(&u1, 1);
    size_t row = UA_shape_axis(&u1, 0);
    vfloat_t (*row_ptr1)[col] = UA_data_ptr(&u1);
    vfloat_t (*row_ptr2)[col] = UA_data_ptr(&u2);
    vfloat_t x = 0.f;
    for (size_t i=0; i<row; ++i) {
        
        vfloat_t *ptr1 = row_ptr1[i];
        vfloat_t *ptr2 = row_ptr2[i];

        vfloat_t arvg1 = 0.f;
        vfloat_t arvg2 = 0.f;

        for (int j=0; j<col; ++j) {
            arvg1 += ptr1[j];
            arvg2 += ptr2[j];
        }

        arvg1 = arvg1 / col;
        arvg2 = arvg2 / col;
        vfloat_t dot = 0.f;
        vfloat_t mod1 = 0.f;
        vfloat_t mod2 = 0.f;

        for (int k=0; k<col; ++k) {
            dot += (ptr1[k] - arvg1) * (ptr2[k] - arvg2);
            mod1 += ptr1[k] * ptr1[k];
            mod2 += ptr2[k] * ptr2[k];
        }
        vfloat_t v = 0.f;
        if (mod1 != 0.f && mod2 != 0.f) {
            v = dot / ( sqrt(mod1) * sqrt(mod2) );
        } else {
            v = -1 * pow(0.95, x++);
        }
        UA_set(&scores, v, i);
    }

    if (need_free_pad) {
        UArray_(&u1);
    }
    return scores;
}