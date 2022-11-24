/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2022-11-23 07:19:32
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2022-11-24 11:00:43
 * @FilePath: /boring-code/src/leetcode/891.h
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <limits.h>
#include "891.h"

static int cmp(const void *pa, const void *pb) {
    return *(int *)pa - *(int *)pb;
}

static int fn1 (int* input, int n, int m, int* left, int* right, long* widths) 
{
    for (int c=0; c<n-m+1; ++c) {
        int cur_left  = *left;
        int cur_right = *right;

        if ( *left  > input[c] ) *left  = input[c];
        if ( *right < input[c] ) *right = input[c];
        
        if (m == 1) {
            *widths += *right - *left;
        } else {
            fn1(&input[c+1], n-c-1, m-1, left, right, widths);
        }
        *left  = cur_left;
        *right = cur_right; 
    }
    return 0;
} 


int sum_sub_seq_widths(int* input, int input_size)
{

    int right = INT_MIN;
    int left = INT_MAX;
    long widths = 0;
    long total = 0;
    for (int i=2; i<=input_size; ++i) {
        fn1(input, input_size, i, &left, &right, &widths);
        total += widths;
        widths = 0;
    }
    return total;
}

int sum_sub_seq_widths2(int* nums, int numsSize)
{
    qsort(nums, numsSize, sizeof(int), cmp);
    long long res = 0, mod = 1e9 + 7;
    long long x = nums[0], y = 2;
    for (int j = 1; j < numsSize; j++) {
        res = (res + nums[j] * (y - 1) - x) % mod;
        x = (x * 2 + nums[j]) % mod;
        y = y * 2 % mod;
    }
    return (res + mod) % mod;
}



