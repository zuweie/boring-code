/*
 * @Author: your name
 * @Date: 2020-12-26 09:46:56
 * @LastEditTime: 2021-01-02 09:45:32
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/unit_test_complex.c
 */
#include <stdio.h>
#include <CUnit/Basic.h>
#include "test_data.h"
#include "complex/complex.h"
#include "fourier_transform/fourier_transform.h"

#define PRINTF_COMPLEX(complex) printf("<real: %f, image: %f, amplitude: %f>", complex.real, complex.image, sqrt(complex.real*complex.real + complex.image*complex.image))
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
    float sequence[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15};
    complex_t out[N];
    discrete_fourier_transform(sequence, N, out);
    printf("\n");
    for (int i=0; i<N; ++i) {
        PRINTF_COMPLEX(out[i]);
        printf("\n");
    }
}

int do_fft_test (void) 
{
    CU_pSuite pSuite = NULL;
    pSuite = CU_add_suite("Test fft Suite", suite_success_init, suite_success_clean);
    if (NULL == pSuite){
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test complex", text_complex) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }

    if (NULL == CU_add_test(pSuite, "test complex", test_dft) ) {
        CU_cleanup_registry();
        return CU_get_error();
    }
}