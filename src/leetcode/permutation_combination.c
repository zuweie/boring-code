/*
 * @Author: zuweie jojoe.wei@gmail.com
 * @Date: 2022-11-15 20:53:36
 * @LastEditors: zuweie jojoe.wei@gmail.com
 * @LastEditTime: 2022-11-21 18:16:16
 * @FilePath: /boring-code/src/leetcode/permutation_combination.c
 * @Description: 这是默认设置,请设置`customMade`, 打开koroFileHeader查看配置 进行设置: https://github.com/OBKoro1/koro1FileHeader/wiki/%E9%85%8D%E7%BD%AE
 */
#include <stdlib.h>
#include <stdio.h>
#include "permutation_combination.h"

static int splitting(float* input, int n, int m, float* fill, int shape);
static int back_tracking();

static int calculate_combine_number(int n, int m);

float** permutation_combination(float* input, int n, int m, int* ret_row, int* ret_col)
{
    *ret_col = calculate_combine_number(n, m);
    *ret_row = m;
    float (*mat)[*ret_col] = malloc(sizeof(float) * (*ret_row) * (*ret_col));
    float* fill = mat[0];
    splitting(input, n, m, fill, *ret_col);
    return mat;
}

float** permutation_combination2(float* input, int n, int m, int* ret_row, int* ret_col)
{
    *ret_row = calculate_combine_number(n, m);
    *ret_col = m;
    float (*mat)[*ret_col] = malloc(sizeof(float) * (*ret_row) * (*ret_col));
    float stack[*ret_col+1];
    int mat_row_index = 0;
    back_tracking(stack, 0, input, n, m, mat, (*ret_col), &mat_row_index);
    return mat;
}


static int splitting(float* input, int n, int m, float* fill,  int shape) 
{
    //if (m == 0) return 0;

    int offset = 0;
    
    for (int c=0; c<=n-m+1; ++c) {
        int k = calculate_combine_number(n-c-1,  m-1);
        for (int i=0; i<k; ++i) {
            fill[offset+i] = input[c];
        }

        // 填完当前一层, 填写下一层。
        if ( m-1 > 0) {
            float* next_fill = fill + offset + shape;
            splitting(&input[c+1], n-c-1, m-1, next_fill, shape);
        }

        offset += k;
    }
    return 0;
}

static int back_tracking (float* stack, int stack_pos, float* path, int n, int m, float** mat, int mat_shape, int* mat_row_index) {

    for (int c=0; c<n-m+1; ++c) {
        stack[stack_pos++] = path[c];

        if (m == 1) {
            // 直接收割数据。
            // 到了道路尽头
            float (*row_data)[mat_shape] = mat;
            for (int i=0; i<stack_pos; ++i) {
                row_data[*mat_row_index][i] = stack[i];
            }
            (*mat_row_index)++;
        } else {
            // 在递归搜索数据
            back_tracking(stack, stack_pos, &path[c+1], n-c-1, m-1, mat, mat_shape, mat_row_index);
        }
        stack_pos--;
    }
    return 0;
    
}

static int calculate_combine_number(int n, int m)
{
    if (n == m || m == 0) return 1;

    int r = n;
    for (int i=n-1; i>=n-m+1;i--) 
        r *= i;

    int f = m;
    for (int j=m-1; j>=1; j--) 
        f *= j;

    return (r / f);
}
