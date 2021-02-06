/*
 * @Author: your name
 * @Date: 2020-12-26 09:46:56
 * @LastEditTime: 2021-02-06 08:23:09
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/unit_test_complex.c
 */
#include <stdio.h>
#include <CUnit/Basic.h>
#include "test_data.h"
#include "complex/complex.h"
#include "fourier_transform/fourier_transform.h"

#define PRINTF_COMPLEX(complex) printf("<%f %fi, amplitude: %f>", complex.real, complex.image, sqrt(complex.real*complex.real + complex.image*complex.image))
#define PRINTF_BIT(v) do { \
    for (int __marco_i=(sizeof(v)*8)-1; __marco_i>=0; --__marco_i) { \
        printf("%d", ((v & (1<<__marco_i)) >> __marco_i)); \
    } \
} while (0)

static int  suite_success_init (void) 
{
    printf("\nfft suite success init\n");
}

static int suite_success_clean (void) 
{   
    printf("\nfft suite success clean\n");
}

void text_complex (void) {
    complex_t complex = _complex(1.2f, 3.5f);
    CU_ASSERT_TRUE(complex.real == 1.2f);
    CU_ASSERT_TRUE(complex.image == 3.5f);
}
void test_dft (void) {
    size_t N = 16;
    //float sequence[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    float sequence[] = {0, 1,2,3,4,5,6,7,12,45,-1,-2,-4,88,33,77};

    //float sequence[] = {0, 1, 2, 3};
    // size_t N = 8;
    // float sequence[] = {0, 1, 2, 3, 4, 5, 6, 7};
    
    complex_t out[N];
    Discrete_fourier_transform(sequence, N, out);
    printf("\n");
    for (int i=0; i<N; ++i) {
        PRINTF_COMPLEX(out[i]);
        printf("\n");
    }
}

void test_rfft (void) {
    size_t N = 16;
    complex_t sequence[] = {_complex(0,0), _complex(1,0), _complex(2,0), _complex(3, 0), _complex(4, 0), _complex(5, 0), _complex(6, 0), _complex(7, 0), _complex(8, 0), _complex(9,0), _complex(10,0), _complex(11, 0), _complex(12, 0), _complex(13, 0), _complex(14, 0), _complex(15,0)};
    //double sequence[] = {0, 1, 2, 3};
    complex_t y[N];
    Recursive_fast_fourier_transform(sequence, N, y, 0);
    
    printf("\n rfft: \n");
    for (int i=0; i<N; ++i) {
        PRINTF_COMPLEX(y[i]);
        printf("\n");
    }

}   
void test_ifft(void) {
    size_t N = 16;
    complex_t sequence[] = {_complex(0,0), _complex(1,0), _complex(2,0), _complex(3, 0), _complex(4, 0), _complex(5, 0), _complex(6, 0), _complex(7, 0), _complex(8, 0), _complex(9,0), _complex(10,0), _complex(11, 0), _complex(12, 0), _complex(13, 0), _complex(14, 0), _complex(15,0)};
    complex_t A[N];
    Iterative_fast_fourier_transform(sequence, N, A, 0);
    printf("\n ifft: \n");
    for (int i=0; i<N; ++i) {
        PRINTF_COMPLEX(A[i]);
        printf("\n");
    }
}

void test_ifft2(void) {
    size_t N = 16;
    complex_t sequence[] = {_complex(0,0), _complex(1,0), _complex(2,0), _complex(3, 0), _complex(4, 0), _complex(5, 0), _complex(6, 0), _complex(7, 0), _complex(8, 0), _complex(9,0), _complex(10,0), _complex(11, 0), _complex(12, 0), _complex(13, 0), _complex(14, 0), _complex(15,0)};
    Iterative_fast_fourier_transform2(sequence, N, 0);
    printf("\n ifft 2: \n");
    for (int i=0; i<N; ++i) {
        PRINTF_COMPLEX(sequence[i]);
        printf("\n");
    }
}
void test_reverse_rfft(void) 
{
    size_t N = 16;
    complex_t sequence[] = {_complex(0,0), _complex(1,0), _complex(2,0), _complex(3, 0), _complex(4, 0), _complex(5, 0), _complex(6, 0), _complex(7, 0), _complex(8, 0), _complex(9,0), _complex(10,0), _complex(11, 0), _complex(12, 0), _complex(13, 0), _complex(14, 0), _complex(15,0)};
    //double sequence[] = {0, 1, 2, 3};
    complex_t y[N];
    Recursive_fast_fourier_transform(sequence, N, y, 0);
    complex_t x[N];
    Reverse_recursive_fast_fourier_transorm(y, N, x);
    printf("\n resver_rfft: \n");
    for (int i=0; i<N; ++i) {
        PRINTF_COMPLEX(x[i]);
        printf("\n");
    }
}

void test_reverse_ifft(void) 
{
    size_t N = 16;
    complex_t sequence[] = {_complex(0,0), _complex(1,0), _complex(2,0), _complex(3, 0), _complex(4, 0), _complex(5, 0), _complex(6, 0), _complex(7, 0), _complex(8, 0), _complex(9,0), _complex(10,0), _complex(11, 0), _complex(12, 0), _complex(13, 0), _complex(14, 0), _complex(15,0)};
    complex_t A[N];
    Iterative_fast_fourier_transform(sequence, N, A, 0);
    complex_t x[N];
    Reverse_iterative_fast_fourier_transorm(A, N, x);
    printf("\n resver_ifft: \n");
    for (int i=0; i<N; ++i) {
        PRINTF_COMPLEX(x[i]);
        printf("\n");
    }

}

void test_reverse_ifft2(void)
{
    size_t N = 16;
    complex_t sequence[] = {_complex(0,0), _complex(1,0), _complex(2,0), _complex(3, 0), _complex(4, 0), _complex(5, 0), _complex(6, 0), _complex(7, 0), _complex(8, 0), _complex(9,0), _complex(10,0), _complex(11, 0), _complex(12, 0), _complex(13, 0), _complex(14, 0), _complex(15,0)};
    Iterative_fast_fourier_transform2(sequence, N, 0);
    Reverse_iterative_fast_fourier_transorm2(sequence, N);
    printf("\n resver_ifft 2: \n");
    for (int i=0; i<N; ++i) {
        PRINTF_COMPLEX(sequence[i]);
        printf("\n");
    }
}

void test_real_fft (void) {
    size_t N = 16;
    double x[] = {0, 1,2,3,4,5,6,7,12,45,-1,-2,-4,88,33,77};
    complex_t out[N/2+1];
    Real_fast_fourier_transform_complex(x,N,out);
    printf("\n real fft : \n");
    for (int i=0; i<N/2+1; ++i) {
        PRINTF_COMPLEX(out[i]);
        printf("\n");
    }
    printf("\n");
}

void test_bit_reverse(void)
{
    int N = 16;
    printf("\n");
    for (int i=0; i<16; ++i) {
        PRINTF_BIT(i);
        printf("  rv: ");
        int v = Bit_reverse_32(i, 3);
        PRINTF_BIT(v);
        printf("\n");
    }
   
}

// void test_array_bit_reverse(void) 
// {
//     int N = 16;
//     complex_t sequence[] = {_complex(0,0), _complex(1,0), _complex(2,0), _complex(3, 0), _complex(4, 0), _complex(5, 0), _complex(6, 0), _complex(7, 0), _complex(8, 0), _complex(9,0), _complex(10,0), _complex(11, 0), _complex(12, 0), _complex(13, 0), _complex(14, 0), _complex(15,0)};
//     __Bit_reverse(sequence, N);
//     printf("array bit reverse\n");
//     for (int i=0; i<N; ++i) {
//         PRINTF_COMPLEX(sequence[i]);
//         printf ("\n");
//     }
//     printf ("\n");
// }

int do_fft_test (void) 
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("Test fft Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }

    // if (NULL == CU_add_test(pSuite, "test complex", text_complex) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    if (NULL == CU_add_test(pSuite, "test dft", test_dft) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    // if (NULL == CU_add_test(pSuite, "test rfft", test_rfft) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    if (NULL == CU_add_test(pSuite, "test real fft", test_real_fft) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
    
    // if (NULL == CU_add_test(pSuite, "test ifft", test_ifft) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }


    // if (NULL == CU_add_test(pSuite, "test reverse fft", test_reverse_rfft) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "test reverse fft", test_reverse_ifft) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }
    
    // if (NULL == CU_add_test(pSuite, "test bit reverse", test_bit_reverse) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }
    
    // if (NULL == CU_add_test(pSuite, "test ifft 2", test_ifft2) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }

    // if (NULL == CU_add_test(pSuite, "test reverse fft 2", test_reverse_ifft2) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }
    // if (NULL == CU_add_test(pSuite, "test array bit reverse", test_array_bit_reverse) ) {
    //     CU_cleanup_registry();
    //     return CU_get_error();
    // }
}