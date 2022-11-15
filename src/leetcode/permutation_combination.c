#include <stdlib.h>
#include <stdio.h>
#include "permutation_combination.h"

static int splitting(float* input, int n, int m, float** ret);
static int calculate_combine_number(int n, int m);

float** permutation_combination(float* input, int n, int m, int* row_size, int* col_size)
{
    *row_size = calculate_combine_number(n, m);
    *col_size = m;
    float** ret = malloc(sizeof(float) * (*row_size) * (*col_size));
    splitting(input, n, m, ret);
    return ret;
}

static int splitting(float* input, int n, int m, float** ret) {
    
    for (int c=0; c<=n-m+1; c++) {
        
    }

}

static int calculate_combine_number(int n, int m)
{
    int r = n;
    for (int i=n-1; i>=n-m+1;i--) 
        r *= i;

    int f = m;
    for (int j=m-1; j>=1; j--) 
        f *= j;

    return (r / f);
}
