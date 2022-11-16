#include <stdlib.h>
#include <stdio.h>
#include "permutation_combination.h"

static int splitting(float* input, int n, int m, float* fill, int shape);
static int calculate_combine_number(int n, int m);

float** permutation_combination(float* input, int n, int m, int* row_size, int* col_size)
{
    *col_size = calculate_combine_number(n, m);
    *row_size = m;
    float (*mat)[*col_size] = malloc(sizeof(float) * (*row_size) * (*col_size));
    float* fill = mat[0];
    splitting(input, n, m, fill, *col_size);
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
