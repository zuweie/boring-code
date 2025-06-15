/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2022-12-08 09:41:04
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2025-06-15 10:50:22
 * @FilePath: /boring-code/src/leetcode/1775.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include "container/Cont.h"

static int fn(int target, int input[], int input_size, int* res, CN dp);

int min_operations(int* nums1, int nums1_size, int* nums2, int nums2_size)
{
    int min_of_longger = (nums1_size > nums2_size ? nums1_size : nums2_size);
    int max_of_shorter = (nums1_size < nums2_size ? nums1_size : nums2_size) * 6;

    if (min_of_longger > max_of_shorter) return -1;

    int arr[nums1_size + nums2_size];
    
    //memcpy(arr, nums1, sizeof(int) * nums1_size);
    int i;
    for (i=0; i<nums1_size; ++i) {
        arr[i] = nums1[i];
    }
    for (i=0; i<nums2_size; ++i) {
        arr[nums1_size+i] = -nums2[i];
    }

    //CN dp = CN_create(HASHMAP | customized_entity, )
    CN dp = CN_create(HASH_MAP|customized_entity, 3, 2, int_t, int_t, int_t);
    int res = 0;
    int ok = fn(0, arr, (nums1_size+nums2_size), &res, dp);

    CN_finalize(dp, NULL);
    return res;
}


static int fn(int target, int input[], int input_size, int* res, CN dp) 
{
    //printf("target:%d, input_size:%d\n", target, input_size);
    int ok = 0;
    int *val = CN_get(dp, target, input_size);

    if (val) {
        if (*val > 0) {
            *res = *val;
            ok = 1;
        } else {
            ok = 0;
        }

    }else if (input_size == 1 ) {
        ok = 1;
        if (target == input[0]){
            *res = 0;
        } else if ( (input[0] < 0 && target < 0 && target > -7) 
                ||  (input[0] > 0 && target > 0 && target < 7) ) {
            *res = 1;
        } else {
            ok = 0;
        }
    } else {
        int result = 0;
        int ret = INT_MAX;
        int i2;
        
        for (int i=6; i>=1; --i) {
            i2 = input[0] < 0 ? -i : i;

            if (fn(target-i2, &input[1], input_size-1, &result, dp)) {
                ok = 1;
                if (input[0] != i2) 
                    result += 1;

                if (ret > result) 
                    ret = result;
            }
            result = 0;
        } 
        *res = ret;
    }
    //printf("ok:%d\n", ok);
    if (ok) {
        //printf("target:%d, input_size:%d, res:%d, ok:%d \n", target, input_size, (*res), ok);
        CN_set(dp, target, input_size, (*res));
    } else {
        CN_set(dp, target, input_size, -1);
    }
    return ok;
}