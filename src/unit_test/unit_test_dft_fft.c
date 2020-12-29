/*
 * @Author: your name
 * @Date: 2020-12-26 09:46:56
 * @LastEditTime: 2020-12-26 23:36:29
 * @LastEditors: Please set LastEditors
 * @Description: In User Settings Edit
 * @FilePath: /boring-code/src/unit_test/unit_test_complex.c
 */
#include <stdio.h>
#include <CUnit/Basic.h>
#include "test_data.h"
#include "complex/complex.h"

#define PRINTF_COMPLEX(complex) printf("<real: %f, image: %f>", complex.real, complex.image)
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
}